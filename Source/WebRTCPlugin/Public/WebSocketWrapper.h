#pragma once
#include "WebSocketsModule.h" // Module definition
#include "IWebSocket.h"       // Socket definition
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"

class WEBRTCPLUGIN_API WebSocketObserver
{
public:
	virtual void OnConnect() = 0;
	virtual void OnConnectionError(const FString& Error) = 0;
	virtual void OnClose(int32 StatusCode, const FString& Reason, bool bWasClean) = 0;
	virtual void OnMessage(const FString & Message) = 0;
	virtual void OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining) = 0;
	virtual void OnMessageSent(const FString& MessageString) = 0;
};

class WEBRTCPLUGIN_API WebSocketWrapper
{
public:
	WebSocketWrapper(const FString& wsHost, const FString& wsProtocol)
		: wsHost(wsHost), wsProtocol(wsProtocol)
	{}
	
	TSharedPtr<IWebSocket> CreateSocket();
	void Send(TSharedRef<FJsonObject> JsonObject);
	TSharedPtr<IWebSocket> GetSocket();
private:
	FString wsHost;
	FString wsProtocol;
	TSharedPtr<IWebSocket> Socket;
};
