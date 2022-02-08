#include "WebSocketWrapper.h"

void WebSocketWrapper::Subscribe(WebSocketObserver* Observer_)
{
	this->Observer = Observer_;
}

void WebSocketWrapper::Connect()
{
	const FString ServerURL = wsHost; // Your server URL. You can use ws, wss or wss+insecure.
	const FString ServerProtocol = wsProtocol;              // The WebServer protocol you want to use.
    
	const TSharedPtr<IWebSocket> Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	if(Observer != nullptr)
	{
		WebSocketObserver* O = Observer;
		Socket->OnConnected().AddLambda([=]() -> void
		{
			O->OnConnect();
		});
		Socket->OnConnectionError().AddLambda([=](const FString& Error) -> void
		{
			O->OnConnectionError(Error);
		});
		Socket->OnClosed().AddLambda([=](int32 StatusCode, const FString& Reason, bool bWasClean) -> void
		{
			O->OnClose(StatusCode, Reason, bWasClean);
		});
		Socket->OnMessage().AddLambda([=](const FString& Message) -> void
		{
			O->OnMessage(Message);
		});
		Socket->OnMessageSent().AddLambda([=](const FString& Message) -> void
		{
			O->OnMessageSent(Message);
		});
		Socket->OnRawMessage().AddLambda([=](const void* Data, SIZE_T Size, SIZE_T ByteRemaining) -> void
		{
			O->OnRawMessage(Data, Size, ByteRemaining);
		});
	}

	Socket->Connect();
}