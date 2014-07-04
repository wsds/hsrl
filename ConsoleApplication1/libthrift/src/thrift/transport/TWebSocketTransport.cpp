/*
* Licensed to the Apache Software Foundation (ASF) under one
* or more contributor license agreements. See the NOTICE file
* distributed with this work for additional information
* regarding copyright ownership. The ASF licenses this file
* to you under the Apache License, Version 2.0 (the
* "License"); you may not use this file except in compliance
* with the License. You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing,
* software distributed under the License is distributed on an
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
* KIND, either express or implied. See the License for the
* specific language governing permissions and limitations
* under the License.
*/

#include "Websocket\WebsocketProtocol.h"
#include "Websocket\WebsocketDataMessage.h"
#include "Websocket\WebsocketHandshakeMessage.h"

#include "TWebsocketTransport.h"

namespace apache { namespace thrift { namespace transport {

using namespace std;

// Yeah, yeah, hacky to put these here, I know.
const char* TWebsocketTransport::CRLF = "\r\n";
const int TWebsocketTransport::CRLF_LEN = 2;
	

TWebsocketTransport::TWebsocketTransport(boost::shared_ptr<TTransport> transport) 
	: transport_(transport),
	protocolHandler_(new WebsocketProtocol),	
	httpBuf_(NULL),
	httpPos_(0),
	httpBufLen_(0),
	httpBufSize_(1024) {
		init();
}

void TWebsocketTransport::init() {
	httpBuf_ = (char*)std::malloc(httpBufSize_+1);
	if (httpBuf_ == NULL) {
		throw std::bad_alloc();
	}
	httpBuf_[httpBufLen_] = '\0';
}

TWebsocketTransport::~TWebsocketTransport() {
	if (httpBuf_ != NULL) {
		std::free(httpBuf_);
	}
}

void TWebsocketTransport::setOldVersion(bool old) { protocolHandler_->setVersion(old); }
bool TWebsocketTransport::isOldVersion() const { return protocolHandler_->isOldVersion(); }

bool TWebsocketTransport::handshake()
{
	return readMoreData() == 0;	
}

uint32_t TWebsocketTransport::read(uint8_t* buf, uint32_t len) {
	if (readBuffer_.available_read() == 0) {
		readBuffer_.resetBuffer();
		uint32_t got = readMoreData();
		if (got == 0) {
			return 0;
		}
	}
	return readBuffer_.read(buf, len);
}

uint32_t TWebsocketTransport::readEnd() {
	return 0;//getValidData();
}

uint32_t TWebsocketTransport::readMoreData() {
	shift();
	// Get more data!   	
	refill();

	return getValidData();	
}


/// if is the firest package, should handle shakehand
/// else ret rid of package header.
uint32_t TWebsocketTransport::getValidData()   /// read more data
{
	assert(httpBufLen_ >= httpPos_);

	WebsocketMessage* pPacket = NULL;    
    unsigned int uExtractedBytes = 0;        
	int iResult = protocolHandler_->deframeIncomingPacket(httpBuf_+httpPos_, httpBufLen_-httpPos_, pPacket, uExtractedBytes);	
	if (iResult == WebsocketProtocol::Success)
	{	
		assert(httpBufLen_-httpPos_ >= uExtractedBytes);
		httpPos_ += uExtractedBytes;

		if(protocolHandler_->isHandshaking())
		{	if (httpPos_ != httpBufLen_) throw TTransportException(TTransportException::END_OF_FILE, "web socket header error.");
			/// 要解决数据没有读够的问题
			assert(pPacket != NULL);
			handleHttpHeader(*pPacket);
			uExtractedBytes = 0;
		} 
		else	/// for handling data
		{
			const string& data = ((WebsocketDataMessage*)pPacket)->GetEncodedData();
			readBuffer_.write((uint8_t*)data.c_str(), data.size());
			uExtractedBytes = data.size();
		}

		delete pPacket;
	}
	else if (iResult == WebsocketProtocol::eIncompletePacket)
	{
		assert(pPacket == NULL);

		refill();
		uExtractedBytes = 0;
	}

	return uExtractedBytes;
}



void TWebsocketTransport::handleHttpHeader(WebsocketMessage& packet)
{		
	assert(packet.getType() == WebsocketMessage::Handshake);

	WebsocketHandshakeMessage& request = (WebsocketHandshakeMessage&) packet;
	if (!request.Parse())
	{
		//packet and lpContext are not good.		
		//should close the connect
		assert(false);
		return;
	}
	/// to support old version.
	protocolHandler_->setVersion(request.isOldVersion());

	WebsocketHandshakeMessage *pResponse = new WebsocketHandshakeMessage();
	pResponse->m_websocketVersion = request.m_websocketVersion;
	if (WebsocketProtocol::ProcessHandshake(request, *pResponse))
	{  
		protocolHandler_->setConnected();
	} else {
		/// alwayse ok
		assert(false);
		return;
	}

	// ok, now send the response
	char outData[10*1024];
	unsigned int nWrittenBytes;
	int iResult = protocolHandler_->frameOutgoingPacket(*pResponse, outData, 10*1024, nWrittenBytes);
	if (iResult == WebsocketProtocol::Success)
	{
		transport_->write((uint8_t*)outData, nWrittenBytes);
		transport_->flush();		
	}		

	delete pResponse;
}



void TWebsocketTransport::shift() {
	assert(httpBufLen_ >= httpPos_);
		
	/// do nothing if available buffer is more than 75% of http buffer size.
	if ((httpBufSize_ - httpBufLen_) >= (httpBufSize_ / 4))
		return;	

	if (httpBufLen_ > httpPos_) {
		// Shift down remaining data and read more
		uint32_t length = httpBufLen_ - httpPos_;
		memmove(httpBuf_, httpBuf_+httpPos_, length);
		httpBufLen_ = length;
	} else {
		httpBufLen_ = 0;
	}
	httpPos_ = 0;
	httpBuf_[httpBufLen_] = '\0';
}

void TWebsocketTransport::refill() {
	/// double the buffer size,if the available buffer is little to 25% of current buffer size.
	uint32_t avail = httpBufSize_ - httpBufLen_;
	if (avail <= (httpBufSize_ / 4)) {
		httpBufSize_ *= 2;
		httpBuf_ = (char*)std::realloc(httpBuf_, httpBufSize_+1);
		if (httpBuf_ == NULL) {
			throw std::bad_alloc();
		}
	}

	// Read more data
	uint32_t got = transport_->read((uint8_t*)(httpBuf_+httpBufLen_), httpBufSize_-httpBufLen_);
	httpBufLen_ += got;
	httpBuf_[httpBufLen_] = '\0';

	if (got == 0) {
		/// when connection is disconnected, will come to here.
		throw TTransportException("Could not refill buffer");
	}
}


void TWebsocketTransport::write(const uint8_t* buf, uint32_t len) {
	writeBuffer_.write(buf, len);
}

void TWebsocketTransport::flush() {
	// Fetch the contents of the write buffer
	uint8_t* buf;
	uint32_t len;
	writeBuffer_.getBuffer(&buf, &len);

	WebsocketDataMessage response((char*)buf, len);

	// ok, now send the response
	char outData[30*1024];
	unsigned int nWrittenBytes;
	int iResult = protocolHandler_->frameOutgoingPacket(response, outData, 30*1024, nWrittenBytes);
	if (iResult == WebsocketProtocol::Success)
	{
		transport_->write((uint8_t*)outData, nWrittenBytes);
		transport_->flush();		
	}

	// Reset the buffer
	writeBuffer_.resetBuffer();  
}

}}}
