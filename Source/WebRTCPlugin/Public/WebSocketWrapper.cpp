#include "WebSocketWrapper.h"


TSharedPtr<IWebSocket> WebSocketWrapper::GetSocket()
{
	return Socket;
}

void WebSocketWrapper::Send(TSharedRef<FJsonObject> JsonObject)
{
	if(Socket -> IsConnected())
	{
		FString ToJsonString;
		TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&ToJsonString);

		if(FJsonSerializer::Serialize(JsonObject, JsonWriter))
		{
			UE_LOG(LogTemp, Log, TEXT("Send data: \r\n%s"), *ToJsonString);
			Socket.Get()->Send(ToJsonString);	
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Socket is not connected!"))
	}
}

TSharedPtr<IWebSocket> WebSocketWrapper::CreateSocket()
{
	const FString ServerURL = wsHost; // Your server URL. You can use ws, wss or wss+insecure.
	const FString ServerProtocol = wsProtocol;              // The WebServer protocol you want to use.
    
	Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);
	UE_LOG(LogTemp, Log, TEXT("WebSocket Connect TargetUrl %s %s"), *ServerURL, *ServerProtocol);

	return Socket;
}