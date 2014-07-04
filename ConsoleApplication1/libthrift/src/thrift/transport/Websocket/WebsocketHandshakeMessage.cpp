
#include <iostream>  
#include <sstream>  

#include "WebsocketHandshakeMessage.h"


WebsocketHandshakeMessage::WebsocketHandshakeMessage(const char* raw, int size)
	: WebsocketMessage(Handshake)
{
	this->nSize = size;
	this->pRaw = new char[size+1];
	memcpy(this->pRaw, raw, size);
    this->pRaw[nSize] = '\0';
}

WebsocketHandshakeMessage::WebsocketHandshakeMessage()
	:WebsocketMessage(Handshake)
{
    //HTTP/1.1 101 Switching Protocols
   m_version = "HTTP/1.1";
    
	pRaw = NULL;
	nSize = 0;
}


WebsocketHandshakeMessage::~WebsocketHandshakeMessage(void)
{
	delete[] pRaw;
}

bool WebsocketHandshakeMessage::Parse()
{
    std::istringstream s(pRaw);
    //std::istream s(pRaw);

	std::string request;

	// get status line
	std::getline(s, request);

	if (request[request.size()-1] == '\r') {
		request.erase(request.end()-1);

		std::stringstream ss(request);
		std::string val;

		ss >> m_method;
		ss >> m_uri;
		ss >> m_version;
	} else {
		return false;
	}

	std::string header;
	std::string::size_type end;

	// get headers
	while (std::getline(s, header) && header != "\r") {
		if (header[header.size()-1] != '\r') {
			continue; // ignore malformed header lines?
		} else {
			header.erase(header.end()-1);
		}

		end = header.find(": ",0);

		if (end != std::string::npos) {
			string key = header.substr(0,end);
			string val = header.substr(end+2);

			fields[key] = val;
		}
	}

	/// to support old version, add by shangrong.
	if (fields.find("Sec-WebSocket-Version") != fields.end())
		m_websocketVersion = fields["Sec-WebSocket-Version"];

	if (isOldVersion()) {
		string challenge(8, 0);
		for (int i = 0; i < 8; i++) {
			challenge[i] = pRaw[strlen(pRaw) - 8 + i];
		}	
		fields["Challenge"] = challenge;
	}

	return true;
}



std::string WebsocketHandshakeMessage::GetField( string key ) const
{
	std::map<string, string>::const_iterator i = fields.find(key);
	return (i != fields.end()) ? i->second : std::string();
}

void WebsocketHandshakeMessage::SetField( string key, string val )
{
	fields[key] = val;
}


string WebsocketHandshakeMessage::Serialize( )
{
	std::stringstream raw;

	//raw << m_method << " " << m_uri << " " << m_version << "\r\n";
    raw << "HTTP/1.1 101 Switching Protocols\r\n";

	map<string, string>::iterator it;
	for (it = fields.begin(); it != fields.end(); it++) {
		raw << it->first << ": " << it->second << "\r\n";
	}

	raw << "\r\n";

    //return raw.str(); 	
	return isOldVersion() ? m_responsebuf : raw.str();
}
