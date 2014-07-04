

#pragma once


// This class is exported from the WebsocketProtocol.dll
class WebsocketMessage {
public:
	enum
	{
		Handshake = 0,
		DataMessage,
	};

	WebsocketMessage(int t) : type_(t) {}
	~WebsocketMessage(void)	{}

	int getType() const { return type_; }

private:
	int type_;
};
