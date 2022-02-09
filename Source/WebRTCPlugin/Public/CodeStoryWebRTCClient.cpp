#include "CodeStoryWebRTCClient.h"

DECLARE_LOG_CATEGORY_EXTERN(CodeStoryWebRTCClientLog, Log, All);
DEFINE_LOG_CATEGORY(CodeStoryWebRTCClientLog);

// ============================================================================================ //
//
//							WebSocket Observer Implementations!
//
// ============================================================================================ //

void CodeStoryWebRTCClient::OnConnect()
{
	
}

void CodeStoryWebRTCClient::OnMessage(const FString& Message)
{
	UE_LOG(LogTemp, Log, TEXT("Response Message %s"), *Message);
}

void CodeStoryWebRTCClient::OnConnectionError(const FString& Error)
{
	
}

void CodeStoryWebRTCClient::OnMessageSent(const FString& MessageString)
{
	
}

void CodeStoryWebRTCClient::OnClose(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	
}

void CodeStoryWebRTCClient::OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	
}

// ============================================================================================ //
//
//							Peer Connection Function Implementations!
//
// ============================================================================================ //

void CodeStoryWebRTCClient::CreatePeerConnection()
{
	webrtc::PeerConnectionInterface::RTCConfiguration config;
	config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
	config.enable_dtls_srtp = true;

	PeerConnection = PeerConnectionFactory -> CreatePeerConnection(
		config, nullptr, nullptr, this
	);
}

void CodeStoryWebRTCClient::CreateOfferSdp()
{
	CodeStoryWebRTCThread::WORKER_THREAD->Invoke<void>(RTC_FROM_HERE, [this]()
	{
		int offer_to_receive_video = 1; // 1로 설정시 receive
		int offer_to_receive_audio = 0; // 0으로 설정시 send only
		bool voice_activity_detection = false;
		bool ice_restart = true;
		bool use_rtp_mux = true;
		
		PeerConnection->CreateOffer(this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions(
			offer_to_receive_video,
			offer_to_receive_audio,
			voice_activity_detection,
			ice_restart,
			use_rtp_mux
		));	
	});
}


// ============================================================================================ //
//
//							Peer Connection Observer Implementations!
//
// ============================================================================================ //

void CodeStoryWebRTCClient::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
	UE_LOG(LogTemp, Log, TEXT("On Add Stream id : %d"))
}

void CodeStoryWebRTCClient::OnAddTrack(
	rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
	const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams
)
{
	const auto Track = receiver->track().get();

	if(Track->kind() == "video")
	{
		const auto CastedTrack = static_cast<webrtc::VideoTrackInterface*>(Track);

		CastedTrack->AddOrUpdateSink(&VideoReceiver.Get(), rtc::VideoSinkWants());
	}
}

void CodeStoryWebRTCClient::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
	PeerConnection->AddIceCandidate(candidate);
	std::string CandidateName;

	candidate->ToString(&CandidateName);
	// TODO: Sending ice to remote
}

void CodeStoryWebRTCClient::ConnectSignalingServer()
{
	WebSocket.Connect();
}

void CodeStoryWebRTCClient::OnSuccess(webrtc::SessionDescriptionInterface* desc)
{
	std::string sid = desc->session_id();
	std::string OfferSdp;
	desc->ToString(&OfferSdp);
	CodeStoryPeerSetSessionDescriptionObserver* PeerSetObserver = CodeStoryPeerSetSessionDescriptionObserver::CreateObserver();
	PeerConnection->SetLocalDescription(PeerSetObserver, desc);
	
	auto OfferSdpInstance = webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, OfferSdp, nullptr);
	
	// TODO: Send Kurento Application Server
	TSharedRef<FJsonObject> OfferJson = MakeShareable(new FJsonObject);
	OfferJson->SetStringField("id", "viewer");
	OfferJson->SetStringField("offerSdp", UTF8_TO_TCHAR(OfferSdp.c_str()));
	
	FString ToJsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&ToJsonString);

	if(FJsonSerializer::Serialize(OfferJson, JsonWriter))
	{
		UE_LOG(LogTemp, Log, TEXT("Send data: \r\n%s"), *ToJsonString);
		WebSocket.GetSocket().Get()->Send(ToJsonString);	
	}
}

void CodeStoryWebRTCClient::OnFailure(webrtc::RTCError error)
{
	UE_LOG(LogTemp, Log, TEXT("Description Error"));
}

// ============================================================================================ //
//
//							       Not use peer connection event!
//
// ============================================================================================ //

void CodeStoryWebRTCClient::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
{
}

void CodeStoryWebRTCClient::OnRenegotiationNeeded()
{
}

void CodeStoryWebRTCClient::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel)
{
}

void CodeStoryWebRTCClient::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
}

void CodeStoryWebRTCClient::AddRef() const
{
	
}

rtc::RefCountReleaseStatus CodeStoryWebRTCClient::Release() const
{
	return rtc::RefCountReleaseStatus::kDroppedLastRef;
}

