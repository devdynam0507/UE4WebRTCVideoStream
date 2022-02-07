#pragma once
#include "CodeStoryVideoStreamReceiver.h"

class CodeStoryPeerSetSessionDescriptionObserver : public webrtc::SetSessionDescriptionObserver
{
public:
	static CodeStoryPeerSetSessionDescriptionObserver* CreateObserver();

	virtual void OnSuccess() override;
	virtual void OnFailure(webrtc::RTCError error) override;
};

class CodeStoryPeerConnectionObserver : public webrtc::PeerConnectionObserver, public webrtc::CreateSessionDescriptionObserver
{
public:
	CodeStoryPeerConnectionObserver();
	
private:
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> PeerConnection_ = nullptr;
	CodeStoryVideoStreamReceiver* VideoReceiver_ = nullptr;

protected:
	virtual void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
	virtual void OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams) override;
	virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;

	virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override;
	virtual void OnRenegotiationNeeded() override;
	virtual void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;
	virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override;

	virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;

	virtual void OnFailure(webrtc::RTCError error) override;

	virtual void AddRef() const override;
	virtual rtc::RefCountReleaseStatus Release() const override;
};
