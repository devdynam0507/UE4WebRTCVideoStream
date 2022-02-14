#pragma once
#include "CodeStoryWebRTCBridge.h"
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

class WEBRTCPLUGIN_API CodeStoryWebRTCFacade
{
public:
	static CodeStoryWebRTCClient CreateClient(TSharedRef<CodeStoryVideoStreamReceiver> VideoCallbackImpl);
	static webrtc::AudioProcessing* SetupAudioProcessingModule();
	static WebSocketWrapper* CreateWebSocket(const FString& wsHost, const FString& wsProtocol);

	static CodeStoryWebRTCBridge CreateWebRTC(CodeStoryWebRTCClient& Client, WebSocketWrapper& Socket);
	
};