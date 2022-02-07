#include "WebRTCInitializer.h"
#include "WebSocketsModule.h" // Module definition
#include "IWebSocket.h"       // Socket definition

// Signaling server connection thread
static auto g_signal_thread = rtc::Thread::CreateWithSocketServer();

// Peerconnection factory ptr
static rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> g_peer_connection_factory = nullptr;

std::vector<webrtc::SdpVideoFormat> GetVideoFormats()
{
	// Only support H264 Codec
	std::vector<webrtc::SdpVideoFormat> VideoFormats;
	VideoFormats.push_back(webrtc::CreateH264Format(webrtc::H264::kProfileConstrainedBaseline, webrtc::H264::kLevel3_1, "1"));

	return VideoFormats;
}

rtc::scoped_refptr<webrtc::PeerConnectionInterface> GetDefaultPeerConnection(
	rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> factory,
	rtc::Thread* signalingThread, webrtc::PeerConnectionObserver* observer 
)
{
	webrtc::PeerConnectionInterface::RTCConfiguration config;
	config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
	config.enable_dtls_srtp = true;

	auto PeerConnection = factory -> CreatePeerConnection(
		config, nullptr, nullptr, observer
	);

	return PeerConnection;
}

void InitializeWebRTC()
{
	const FString ServerURL = TEXT("ws://45.32.249.81:8080/call"); // Your server URL. You can use ws, wss or wss+insecure.
	const FString ServerProtocol = TEXT("ws");              // The WebServer protocol you want to use.
    
	TSharedPtr<IWebSocket> Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	// We bind all available events
	Socket->OnConnected().AddLambda([]() -> void {
		// This code will run once connected.
		UE_LOG(LogTemp, Log, TEXT("Connected"))

		const TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
		const TSharedPtr<FJsonValue> ViewerJsonValue = MakeShareable(new FJsonValueString("viewer"));
		
		JsonObject->SetField("id", ViewerJsonValue);
		
	});
    
	Socket->OnConnectionError().AddLambda([](const FString & Error) -> void {
		// This code will run if the connection failed. Check Error to see what happened.
		UE_LOG(LogTemp, Log, TEXT("Error ssibal %s"), *Error)
	});
    
	Socket->OnClosed().AddLambda([](int32 StatusCode, const FString& Reason, bool bWasClean) -> void {
		// This code will run when the connection to the server has been terminated.
		// Because of an error or a call to Socket->Close().
	});
    
	Socket->OnMessage().AddLambda([](const FString & Message) -> void {
		// This code will run when we receive a string message from the server.
	});
    
	Socket->OnRawMessage().AddLambda([](const void* Data, SIZE_T Size, SIZE_T BytesRemaining) -> void {
		// This code will run when we receive a raw (binary) message from the server.
	});
    
	Socket->OnMessageSent().AddLambda([](const FString& MessageString) -> void {
		// This code is called after we sent a message to the server.
	});
    
	// And we finally connect to the server. 
	Socket->Connect();
	
	g_signal_thread->Start();

}

