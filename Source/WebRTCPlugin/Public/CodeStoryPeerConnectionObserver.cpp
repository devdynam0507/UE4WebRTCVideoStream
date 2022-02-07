#include "CodeStoryPeerConnectionObserver.h"

// ========================================================================================================================= //

CodeStoryPeerSetSessionDescriptionObserver* CodeStoryPeerSetSessionDescriptionObserver::CreateObserver()
{
	return new rtc::RefCountedObject<CodeStoryPeerSetSessionDescriptionObserver>();
}

void CodeStoryPeerSetSessionDescriptionObserver::OnSuccess()
{
	UE_LOG(LogTemp, Log, TEXT("Success set local description"));
}

void CodeStoryPeerSetSessionDescriptionObserver::OnFailure(webrtc::RTCError error)
{
	UE_LOG(LogTemp, Log, TEXT("Failure set local description"))
}

// ========================================================================================================================= //

CodeStoryPeerConnectionObserver::CodeStoryPeerConnectionObserver()
{
	this->VideoReceiver_ = new CodeStoryVideoStreamReceiver;	
}

void CodeStoryPeerConnectionObserver::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
	UE_LOG(LogTemp, Log, TEXT("On Add Stream id : %d"))
}

void CodeStoryPeerConnectionObserver::OnAddTrack(
	rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
	const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams
)
{
	const auto Track = receiver->track().get();

	if(Track->kind() == "video" && VideoReceiver_)
	{
		auto CastedTrack = static_cast<webrtc::VideoTrackInterface*>(Track);

		CastedTrack->AddOrUpdateSink(VideoReceiver_, rtc::VideoSinkWants());
	}
}

void CodeStoryPeerConnectionObserver::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
	PeerConnection_->AddIceCandidate(candidate);.
//	std::string CandidateName;

//	candidate->ToString(&CandidateName);
	// TODO: Sending ice to remote
}

void CodeStoryPeerConnectionObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc)
{
	PeerConnection_->SetLocalDescription(CodeStoryPeerSetSessionDescriptionObserver::CreateObserver(), desc);

//	std::string OfferSdp;
//	desc->ToString(&OfferSdp);

//	auto OfferSdpInstance = webrtc::CreateSessionDescription(webrtc::SdpType::kOffer, OfferSdp, nullptr);
	// TODO: Send Kurento Application Server
}

void CodeStoryPeerConnectionObserver::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
{
}

void CodeStoryPeerConnectionObserver::OnRenegotiationNeeded()
{
}

void CodeStoryPeerConnectionObserver::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel)
{
}

void CodeStoryPeerConnectionObserver::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
}
