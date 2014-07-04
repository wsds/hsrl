#pragma once

class WebsocketMessage;
class WebsocketHandshakeMessage;

// This class is exported from the WebsocketProtocol.dll
class WebsocketProtocol {
public:
	enum Result
    {
        Success = 0,
        eInsufficientBuffer,
        eIncompletePacket,
        eCorruptPacket,
        eEncodingFailure,
        eDecodingFailure,
        eUndefinedFailure,
        ePacketizingError,
    };

	WebsocketProtocol(void);
	
	/**
		将收到的数据转换成握手请求（第一次收到）或者普通数据包
		@param[in]	buffer: data received to be decode
		@param[in]	bufferLen: buffer len  
		@param[out] pPacket: decoded data packet
		@param[out]	nExtractedBytes: used buffer data len at this time.
	*/
    virtual int deframeIncomingPacket( const char* buffer,
		unsigned int bufferLen,
		WebsocketMessage*& pPacket,
		unsigned int& nExtractedBytes );

	/**
		将要发送的包放到buffer中
		@param[out] pPacket: 
		@param[in]	buffer: data received to be decode
		@param[in]	bufferLen: buffer len  
		@param[out]	nWrittenBytes: used buffer data len at this time.
	*/
    virtual int frameOutgoingPacket( WebsocketMessage& packet,
		char* buffer,
		unsigned int bufferLen,
		unsigned int& nWrittenBytes );
    

    static bool ProcessHandshake( const WebsocketHandshakeMessage& request, WebsocketHandshakeMessage& response );
	
	bool isHandshaking() const { return m_state == HandshakeStage; }
	void setConnected() { m_state = ConnectedStage; }

	/// add for safari to support old version websocket.
	bool isOldVersion() const { return isOldVersion_;}
	void setVersion(bool isOld) { isOldVersion_ = isOld; }

protected:
	bool isOldVersion_;

	enum
	{
		HandshakeStage = 0,	
		ConnectedStage
	}	m_state;
};

