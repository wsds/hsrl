#pragma once

#include <string>

#include "WebsocketMessage.h"

class WebsocketDataMessage : public WebsocketMessage
{
public:
	WebsocketDataMessage(const char* buffer, unsigned len) 
		: WebsocketMessage(DataMessage), 
		raw_(buffer, len){}
	~WebsocketDataMessage(void) {}
	
    bool Encode() { return true; }
	bool Decode() { return true; }
	const std::string& GetEncodedData() { Encode(); return raw_;}

private:
	std::string raw_;
};

