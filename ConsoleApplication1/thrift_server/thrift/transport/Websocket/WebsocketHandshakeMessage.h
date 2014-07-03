#pragma once

#include <map>
#include <string>

#include "WebsocketMessage.h"

using  namespace std;

class WebsocketHandshakeMessage : public WebsocketMessage {
public:
	WebsocketHandshakeMessage(const char* pRaw, int nSize);
	WebsocketHandshakeMessage();
	~WebsocketHandshakeMessage(void);

	bool Parse();
	
	string GetField(string key) const;
	void SetField(string key, string val);
	
	string Serialize();


	/// safari use old websocket protocol, it is outdate at 2011.2.
	string m_hashMD5;
	string m_responsebuf;		
	string m_websocketVersion;

	bool isOldVersion() const { return m_websocketVersion.empty(); }
	string uri() const { return m_uri; }

private:
	char* pRaw;
	int nSize;
	string m_method;
	string m_uri;
	string m_version;
	std::map<string, string> fields;
};


