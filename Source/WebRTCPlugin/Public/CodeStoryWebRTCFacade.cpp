#include "CodeStoryWebRTCFacade.h"

#include "FAudioCapturer.h"
#include "VideoCodecFactory.h"
#include "WebRTCLogging.h"

CodeStoryWebRTCClient CodeStoryWebRTCFacade::CreateClient(
	TSharedRef<CodeStoryVideoStreamReceiver> VideoCallbackImpl
)
{
	RedirectRTCLogToUE4(rtc::LS_VERBOSE);
	rtc::InitializeSSL();
	
	// Start WebRTC peer Thread
	CodeStoryWebRTCThread::SIGNALING_THREAD->Start();
	CodeStoryWebRTCThread::WORKER_THREAD->Start();

	// Create the PeerConnectionFactory
	rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> PeerConnectionFactory = webrtc::CreatePeerConnectionFactory(
	nullptr,
	CodeStoryWebRTCThread::WORKER_THREAD.get(),
	CodeStoryWebRTCThread::SIGNALING_THREAD.get(),
	rtc::scoped_refptr<FAudioCapturer>(new FAudioCapturer),
	webrtc::CreateBuiltinAudioEncoderFactory(), 
	webrtc::CreateBuiltinAudioDecoderFactory(),
	CreateVideoEncoderFactory(),
	CreateVideoDecoderFactory(),
	nullptr,
	SetupAudioProcessingModule()
	);

	webrtc::PeerConnectionFactoryInterface::Options option;
	option.disable_encryption = false;
	PeerConnectionFactory->SetOptions(option);
	
	// Create WebRTC Client Ref 
	return CodeStoryWebRTCClient(
			PeerConnectionFactory,
			VideoCallbackImpl
	);
}

WebSocketWrapper* CodeStoryWebRTCFacade::CreateWebSocket(const FString& wsHost, const FString& wsProtocol)
{
	WebSocketWrapper WSWrapper(wsHost, wsProtocol);

	return new WebSocketWrapper(wsHost, wsProtocol);
}


webrtc::AudioProcessing* CodeStoryWebRTCFacade::SetupAudioProcessingModule()
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

CodeStoryWebRTCBridge CodeStoryWebRTCFacade::CreateWebRTC(CodeStoryWebRTCClient& Client, WebSocketWrapper& Socket)
{
	return CodeStoryWebRTCBridge(Client, Socket);
}
