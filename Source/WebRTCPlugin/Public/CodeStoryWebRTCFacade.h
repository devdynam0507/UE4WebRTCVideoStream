#pragma once
#include "CodeStoryWebRTCClient.h"
#include "WebRTCThread.h"

namespace CodeStoryWebSocket
{
	enum ProtocolType
	{
		WS, WSS
	};

	inline FString EnumToString(ProtocolType WSProtocolType)
	{
		switch (WSProtocolType)
		{
		case WSS:
			return TEXT("wss");
			
		case WS:
		default:
			return TEXT("ws");
		}
	}
}

class CodeStoryWebRTCFacade
{
public:
	static CodeStoryWebRTCClient CreateClient(const FString& SignalingHost, CodeStoryWebSocket::ProtocolType SignalingProtocol, TSharedRef<CodeStoryVideoStreamReceiver> VideoCallbackImpl);

protected:
	static webrtc::AudioProcessing* SetupAudioProcessingModule();
	static WebSocketWrapper CreateWebSocket(const FString& wsHost, const FString& wsProtocol);
	
};