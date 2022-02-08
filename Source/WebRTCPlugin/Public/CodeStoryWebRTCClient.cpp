#include "CodeStoryWebRTCClient.h"

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

void CodeStoryWebRTCClient::OnSuccess(webrtc::SessionDescriptionInterface* desc)
{
	PeerConnection->SetLocalDescription(CodeStoryPeerSetSessionDescriptionObserver::CreateObserver(), desc);

	std::string OfferSdp;
	desc->ToString(&OfferSdp);

	auto OfferSdpInstance = webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, OfferSdp, nullptr);
	// TODO: Send Kurento Application Server
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

