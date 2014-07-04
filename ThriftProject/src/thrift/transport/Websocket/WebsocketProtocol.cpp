// WebsocketProtocol.cpp : Defines the exported functions for the DLL application.
//

#include "base64/base64.h"
#include "sha1/sha1.h"
#include "md5/md5.h"

#include "WebsocketHandshakeMessage.h"
#include "WebsocketDataMessage.h"
#include "WebsocketProtocol.h"
#include <WinSock2.h>


char* createHash(const char* key1, const char* key2, const char* challenge);
bool server_negotiate_websockets_handshake(SOCKET connectfd);
long long longParse(char* str);
char* createMD5Buffer(int result1, int result2, const char challenge[8]);


// This is the constructor of a class that has been exported.
// see WebsocketProtocol.h for the class definition
WebsocketProtocol::WebsocketProtocol()
 : m_state(HandshakeStage)
{
	return;
}
/**
	将收到的数据转换成握手请求（第一次收到）或者普通数据包
	@param[in]	buffer: data received to be decode
	@param[in]	bufferLen: buffer len  
	@param[out] pPacket: decoded data packet
	@param[out]	nExtractedBytes: used buffer data len at this time.
*/
int WebsocketProtocol::deframeIncomingPacket( const char* buffer,
	unsigned int bufferLen,
	WebsocketMessage*& pPacket,
	unsigned int& nExtractedBytes )
{
    if ( bufferLen == 0)
        return eIncompletePacket;

	if (m_state == HandshakeStage)
	{
		pPacket = new WebsocketHandshakeMessage(buffer, bufferLen);
		nExtractedBytes = bufferLen;
		return Success;
	}

	/// for old protocol
	if (isOldVersion())
	{
		if (bufferLen < 3)
			return eUndefinedFailure;

		char framed_data[BUFSIZ*50];	
		char end = 0xFF;
		for (int i = 1; i < bufferLen; i++)
		{
			if (i==bufferLen-1 && buffer[i] == end)
				break; 
			framed_data[i - 1] = buffer[i];	
		}
		pPacket = new WebsocketDataMessage(framed_data, bufferLen-2);
		nExtractedBytes = bufferLen;
		return Success;
	}

	//In the other cases, we should expect a data message : 
    int nMinExpectedSize = 6;
    if (bufferLen < nMinExpectedSize)
        return eIncompletePacket;

    BYTE payloadFlags = buffer[0];
    if (payloadFlags != 129)
        return eUndefinedFailure;

    BYTE basicSize = buffer[1] & 0x7F;
    unsigned __int64 payloadSize;
    int masksOffset;

    if (basicSize <= 125)
    {
        payloadSize = basicSize;
        masksOffset = 2;
    }
    else if (basicSize == 126)
    {
        nMinExpectedSize += 2;
        if (bufferLen < nMinExpectedSize)
            return eIncompletePacket;
        payloadSize = ntohs( *(u_short*) (buffer + 2) );
        masksOffset = 4;
    }
    else if (basicSize == 127)
    {
        nMinExpectedSize += 8;
        if (bufferLen < nMinExpectedSize)
            return eIncompletePacket;
        payloadSize = ntohl( *(u_long*) (buffer + 2) );
        masksOffset = 10;
    }
    else
        return eUndefinedFailure;

    nMinExpectedSize += payloadSize;
    if (bufferLen < nMinExpectedSize)
        return eIncompletePacket;

    BYTE masks[4];
    memcpy(masks, buffer + masksOffset, 4);

    char* payload = new char[payloadSize + 1];
    memcpy(payload, buffer + masksOffset + 4, payloadSize);
    for (unsigned __int64 i = 0; i < payloadSize; i++) {
        payload[i] = (payload[i] ^ masks[i%4]);
    }
	payload[payloadSize] = '\0';
  
	pPacket = new WebsocketDataMessage(payload, payloadSize);	
	nExtractedBytes = nMinExpectedSize;
	
	delete []payload;
	return Success;
}
/**
	将要发送的包放到buffer中
	@param[out] pPacket: 
	@param[in]	buffer: data received to be decode
	@param[in]	bufferLen: buffer len  
	@param[out]	nWrittenBytes: used buffer data len at this time.
*/
int WebsocketProtocol::frameOutgoingPacket( WebsocketMessage& message,
	char* buffer,
	unsigned int bufferLen,
	unsigned int& nWrittenBytes )
{
	if (message.getType() == WebsocketMessage::Handshake)
	{
		WebsocketHandshakeMessage& handshake = (WebsocketHandshakeMessage&) message;

        string strRaw = handshake.Serialize();
        if(strRaw.size() > bufferLen)
            return eInsufficientBuffer;

        memcpy(buffer, (char*)strRaw.c_str(), strRaw.size());
        nWrittenBytes = strRaw.size();
        return Success;
	}

	if (message.getType() == WebsocketMessage::DataMessage)
	{
		WebsocketDataMessage& dataMessage = (WebsocketDataMessage&) message;
        string strData = dataMessage.GetEncodedData();
		
		/// for old protocol
		if (isOldVersion())
		{			
			buffer[0] = 0x00;
			memcpy(buffer, strData.c_str(), strData.size());			
			buffer[1+strData.size()] = 0xFF;		
			nWrittenBytes = strData.size() + 2;
			return Success;
		}

		unsigned __int64 payloadSize = strData.size();

		int expectedSize = payloadSize + 1; //flags byte.
		if(payloadSize <= 125  && payloadSize <= 65535 )
			expectedSize += 1;
		else if(payloadSize > 125  && payloadSize <= 65535)
			expectedSize += 3;
		else
			expectedSize += 9;

		if (expectedSize > bufferLen)
			return eInsufficientBuffer;

		//create the flags byte
		char payloadFlags = 129;
		buffer[0] = payloadFlags; buffer++;

		//create the length byte
		if (payloadSize <= 125)
		{
			buffer[0] = payloadSize; buffer++;
		}
		else if (payloadSize > 125 & payloadSize <= 65535)
		{
			buffer[0] = 126; buffer++;

			buffer[0] = ( payloadSize >> 8 ) & 255;
			buffer[1] = ( payloadSize ) & 255;
			buffer += 2;
		}
		else
		{
			buffer[0] = 127; buffer++;

			buffer[0] = ( payloadSize >> 56 ) & 255;
			buffer[1] = ( payloadSize >> 48  ) & 255;
			buffer[2] = ( payloadSize >> 40 ) & 255;
			buffer[3] = ( payloadSize >> 32  ) & 255;
			buffer[4] = ( payloadSize >> 24 ) & 255;
			buffer[5] = ( payloadSize >> 16  ) & 255;
			buffer[6] = ( payloadSize >> 8 ) & 255;
			buffer[7] = ( payloadSize ) & 255;
			buffer += 8;
		}

        memcpy(buffer, strData.c_str(), strData.size());
        nWrittenBytes = expectedSize;
        return Success;
	}
	
	return eUndefinedFailure;
}

bool WebsocketProtocol::ProcessHandshake(const WebsocketHandshakeMessage& request, WebsocketHandshakeMessage& response )
{
	if (!request.isOldVersion()) {
		std::string server_key = request.GetField("Sec-WebSocket-Key");
		server_key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

		SHA1        sha;
		unsigned int    message_digest[5];

		sha.Reset();
		sha << server_key.c_str();

		sha.Result(message_digest);
		// convert sha1 hash bytes to network byte order because this sha1
		//  library works on ints rather than bytes
		for (int i = 0; i < 5; i++) {
			message_digest[i] = htonl(message_digest[i]);
		}

		server_key = base64_encode(
			reinterpret_cast<const unsigned char*>
			(message_digest),20
			);


		response.SetField("Upgrade", "websocket");
		response.SetField("Connection", "Upgrade");
		response.SetField("Sec-WebSocket-Accept", server_key);
	} else {
		string key1 = request.GetField("Sec-WebSocket-Key1");
		string key2 = request.GetField("Sec-WebSocket-Key2");
		string origin = request.GetField("Origin");
		string challenge = request.GetField("Challenge");
		if (key1.empty() || key2.empty())
			return false;		
		char* hash = createHash(key1.c_str(), key2.c_str(), challenge.c_str());

		string hashMd5(MD5_SIZE, 0);
		for (int i=0; i<MD5_SIZE; i++) {
			hashMd5[i] = hash[i];
		}
		
		string location = "ws://" + request.GetField("Host") + request.uri();

		char* handshake = (char*)calloc(BUFSIZ,sizeof(char));
		char * handshakeFormat = "HTTP/1.1 101 Web Socket Protocol Handshake\r\n"
			"Upgrade: websocket\r\n"
			"Connection: Upgrade\r\n"
			"Sec-WebSocket-Origin: %s\r\n" 
			"Sec-WebSocket-Location: %s\r\n\r\n";
		sprintf(handshake, handshakeFormat, origin.c_str(), location.c_str());
				
		response.m_responsebuf.resize(strlen(handshake)+MD5_SIZE);
		memcpy((char*)response.m_responsebuf.data(), handshake, strlen(handshake));
		memcpy((char*)response.m_responsebuf.data()+strlen(handshake), hash, MD5_SIZE);		
	}
    return true;
}







char* createHash(const char *key1, const char *key2, const char *challenge)
{
	int spaces1 = 0;
	int spaces2 = 0;
	char* digits1 = (char*)calloc(64,sizeof(char));
	char *digits2 = (char*)calloc(64,sizeof(char));
	int d1 = 0, d2 = 0;
	//string digits1, digits2;
	int result1, result2;
	for (int i = 0; i < strlen(key1); i++)
	{
		if (key1[i] == 0x20)
			spaces1++;
	}
	for (int i = 0; i < strlen(key2); i++)
	{
		if (key2[i] == 0x20)
			spaces2++;
	}

	for (int i = 0; i < strlen(key1); i++)
	{
		if (isdigit(key1[i]))
		{
			//digits1 += key1[i];
			digits1[d1++] = key1[i];
		}			
	}
	for (int i = 0; i < strlen(key2); i++)
	{
		if (isdigit(key2[i]))
		{
			//digits2 += key2[i];
			digits2[d2++] = key2[i];
		}			
	}
	result1 = longParse(digits1) / spaces1;
	result2 = longParse(digits2) / spaces2;

	char* raw_answer = createMD5Buffer(result1,result2,challenge);	

	/* calculate the sig */
	char * sig = (char*)calloc(MD5_SIZE,sizeof(char));

	md5_buffer(raw_answer, MD5_SIZE, sig);	//sig is the MD5 hash

	//debug
	/*for (int i = 0; i < MD5_SIZE; i++)
	{
	printf("%d %d\n", raw_answer[i], (unsigned char)sig[i]);
	}*/    
	/* convert from the sig to a string rep */
	//char* str = (char*)calloc(64, sizeof(char));    
	//md5_sig_to_string(sig, str, sizeof(str));

	return sig;
}
long long longParse(char *str)
{
	long long result = 0;
	for (int i = 0; i < strlen(str); i++)
	{
		unsigned digit = str[strlen(str) - 1 - i] - '0';
		result += digit * pow((double)10, i);
	}
	return result;
}



char* createMD5Buffer(int result1, int result2, const char challenge[8])
{	
	char* raw_answer = (char*)calloc(MD5_SIZE,sizeof(char));
	raw_answer[0] = ((unsigned char*)&result1)[3];
	raw_answer[1] = ((unsigned char*)&result1)[2];
	raw_answer[2] = ((unsigned char*)&result1)[1];
	raw_answer[3] = ((unsigned char*)&result1)[0];
	raw_answer[4] = ((unsigned char*)&result2)[3];
	raw_answer[5] = ((unsigned char*)&result2)[2];
	raw_answer[6] = ((unsigned char*)&result2)[1];
	raw_answer[7] = ((unsigned char*)&result2)[0];
	for (int i = 0; i < 8; i++)
	{				
		raw_answer[8 + i] = challenge[i];		
	}
	//display: (debugging)
	/*printf("raw answser bytes: ");
	for (int i = 0; i < MD5_SIZE; i++)		
	printf(" %d", (unsigned char)raw_answer[i]);	
	printf("\n\n");*/
	return raw_answer;
}