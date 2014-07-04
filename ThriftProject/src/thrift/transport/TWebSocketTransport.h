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

#ifndef _THRIFT_TRANSPORT_WEBSOCKET_H_
#define _THRIFT_TRANSPORT_WEBSOCKET_H_ 1

#include "TVirtualTransport.h"
#include "TBufferTransports.h"

class WebsocketProtocol;
class WebsocketMessage;

namespace apache { namespace thrift { namespace transport {

	/**
	* HTTP implementation of the thrift transport. This was irritating
	* to write, but the alternatives in C++ land are daunting. Linking CURL
	* requires 23 dynamic libraries last time I checked (WTF?!?). All we have
	* here is a VERY basic HTTP/1.1 client which supports HTTP 100 Continue,
	* chunked transfer encoding, keepalive, etc. Tested against Apache.
	*/
	class TWebsocketTransport : public TVirtualTransport<TWebsocketTransport> {
	public:
		TWebsocketTransport(boost::shared_ptr<TTransport> transport);

		virtual ~TWebsocketTransport();

		bool handshake();

		void open() {
			transport_->open();
		}

		bool isOpen() {
			return transport_->isOpen();
		}

		bool peek() {
			return transport_->peek();
		}

		void close() {
			transport_->close();
		}

		uint32_t read(uint8_t* buf, uint32_t len);

		uint32_t readEnd();

		void write(const uint8_t* buf, uint32_t len);
		virtual void flush();

		void setOldVersion(bool old);
		bool isOldVersion() const;
				
	protected:

		boost::shared_ptr<TTransport>		transport_;

		boost::shared_ptr<WebsocketProtocol> protocolHandler_;	/** websocket protocol handler; */


		/// 下发的数据放到writeBuffer_上，调用flush后发出
		TMemoryBuffer writeBuffer_;
		TMemoryBuffer readBuffer_;
				
		/// 直接读到的数据放在httpBuf_上，整理好给上层的数据后放到readBuffer_上，
		char* httpBuf_;			/// http buffer
		uint32_t httpPos_;		/// data used position
		uint32_t httpBufLen_;	/// data in buffer len
		uint32_t httpBufSize_;	/// all the buffer size

		virtual void init();

		/** @title readMoreData
		*	@brief 从底层（socket的数据缓存）读取数据，将websocket的包装去掉，只要用户数据
		*	@param[out] pPacket: 
		*	@param[out]	nWrittenBytes: used buffer data len at this time.
		*/
		uint32_t readMoreData();
		
		uint32_t getValidData();   /// read more data		
		void handleHttpHeader(WebsocketMessage& packet);
			

		// read more data, fill up the buffer or get all the data received.
		// only this function can read data from transport.
		void refill();
		/// loop to use the http buffer
		void shift();

		static const char* CRLF;
		static const int CRLF_LEN;
	};

	
	/**
	 * Wraps a transport into HTTP protocol
	 */
	class TWebsocketTransportFactory : public TTransportFactory {
	 public:
	  TWebsocketTransportFactory() {}
	  virtual ~TWebsocketTransportFactory() {}

	  /**
	   * Wraps the transport into a buffered one.
	   */
	  virtual boost::shared_ptr<TTransport> getTransport(boost::shared_ptr<TTransport> trans) {
		return boost::shared_ptr<TTransport>(new TWebsocketTransport(trans));
	  }
	};

}}} // apache::thrift::transport

#endif // #ifndef _THRIFT_TRANSPORT_THTTPCLIENT_H_
