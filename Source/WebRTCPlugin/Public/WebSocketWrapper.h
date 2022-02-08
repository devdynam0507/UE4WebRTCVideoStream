#pragma once
#include "CodeStoryWebRTCClient.h"
#include "WebSocketsModule.h" // Module definition
#include "IWebSocket.h"       // Socket definition

class WebSocketObserver
{
public:
	virtual void OnConnect() = 0;
	virtual void OnConnectionError(const FString& Error) = 0;
	virtual void OnClose(int32 StatusCode, const FString& Reason, bool bWasClean) = 0;
	virtual void OnMessage(const FString & Message) = 0;
	virtual void OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining) = 0;
	virtual void OnMessageSent(const FString& MessageString) = 0;
};

class WebSocketWrapper
{
public:
	WebSocketWrapper(const FString& wsHost, const FString& wsProtocol, WebSocketObserver* Observer)
		: wsHost(wsHost), wsProtocol(wsProtocol), Observer(Observer)
	{}
	
	WebSocketWrapper(const FString& wsHost, const FString& wsProtocol)
		: wsHost(wsHost), wsProtocol(wsProtocol), Observer(nullptr)
	{}
	
	void Connect();
	void Subscribe(WebSocketObserver* Observer);
private:
	FString wsHost;
	FString wsProtocol;
	WebSocketObserver* Observer;
};
