#include "WebRTCInitializer.h"

#include "CodeStoryPeerConnectionObserver.h"
#include "WebSocketsModule.h" // Module definition
#include "IWebSocket.h"       // Socket definition
#include "FAudioCapturer.h"

#define RTC_

// Signaling server connection thread
static auto g_signal_thread = rtc::Thread::CreateWithSocketServer();
static const auto  WorkerThread = rtc::Thread::Create();

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

webrtc::AudioProcessing* SetupAudioProcessingModule()
{
	webrtc::AudioProcessing* AudioProcessingModule = webrtc::AudioProcessingBuilder().Create();
	webrtc::AudioProcessing::Config Config;

	// Enabled multi channel audio capture/render
	Config.pipeline.multi_channel_capture = false;
	Config.pipeline.multi_channel_render = false;
	Config.pipeline.maximum_internal_processing_rate = 48000;
	// Turn off all other audio processing effects in UE's WebRTC. We want to stream audio from UE as pure as possible.
	Config.pre_amplifier.enabled = false;
	Config.high_pass_filter.enabled = false;
	Config.echo_canceller.enabled = false;
	Config.noise_suppression.enabled = false;
	Config.transient_suppression.enabled = false;
	Config.voice_detection.enabled = false;
	Config.gain_controller1.enabled = false;
	Config.gain_controller2.enabled = false;
	Config.residual_echo_detector.enabled = false;
	Config.level_estimation.enabled = false;
	// Apply the config.

	AudioProcessingModule->ApplyConfig(Config);

	return AudioProcessingModule;
}

void InitializeWebRTC()
{
	g_signal_thread->Start();
	WorkerThread->Start();
	
	const FString ServerURL = TEXT("ws://45.32.249.81:8080/call"); // Your server URL. You can use ws, wss or wss+insecure.
	const FString ServerProtocol = TEXT("ws");              // The WebServer protocol you want to use.
    
	TSharedPtr<IWebSocket> Socket = FWebSocketsModule::Get().CreateWebSocket(ServerURL, ServerProtocol);

	auto a = RTC_CHECK_MSG_ENABLED;

	CodeStoryPeerConnectionObserver* Observer = new CodeStoryPeerConnectionObserver;
	rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> PeerConnectionFactory = webrtc::CreatePeerConnectionFactory(
		nullptr,
		WorkerThread.get(),
		g_signal_thread.get(),
		rtc::scoped_refptr<FAudioCapturer>(),
		webrtc::CreateAudioEncoderFactory<webrtc::AudioEncoderOpus>(),
		webrtc::CreateAudioDecoderFactory<webrtc::AudioDecoderOpus>(),
		nullptr,
		nullptr,
		nullptr,
		SetupAudioProcessingModule()
	);
	//rtc::scoped_refptr<webrtc::PeerConnectionInterface> LocalPeerConnection = GetDefaultPeerConnection(PeerConnectionFactory, nullptr, Observer);
	
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
		UE_LOG(LogTemp, Log, TEXT("Error %s"), *Error)
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
}

