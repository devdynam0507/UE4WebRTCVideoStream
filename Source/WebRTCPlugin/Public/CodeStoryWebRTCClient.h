#pragma once
#include "CodeStoryPeerConnectionObserver.h"
#include "WebSocketWrapper.h"
#include "CodeStoryVideoStreamReceiver.h"
#include "CodeStoryWebRTCBridge.h"
#include "WebRTCThread.h"

class CodeStoryWebRTCBridge;

class WEBRTCPLUGIN_API CodeStoryWebRTCClient : public WebSocketObserver, public webrtc::PeerConnectionObserver, public webrtc::CreateSessionDescriptionObserver
{
public:
	CodeStoryWebRTCClient(
		rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> &PeerConnectionFactory,
		TSharedRef<CodeStoryVideoStreamReceiver> &VideoReceiver)
		: PeerConnectionFactory(PeerConnectionFactory),
	      VideoReceiver(VideoReceiver)
	{}

	virtual void OnConnect() override;
	virtual void OnConnectionError(const FString& Error) override;
	virtual void OnClose(int32 StatusCode, const FString& Reason, bool bWasClean) override;
	virtual void OnMessage(const FString & Message) override;
	virtual void OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining) override;
	virtual void OnMessageSent(const FString& MessageString) override;

	virtual void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
	// 시그널링이 트랜시버가 수신 할것임을 나타낼 때 호출된다. Remote Endpoint의 미디어이다. 이것은 SetRemoteDescription()이 호출될 때 trigger 된다.
	virtual void OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams) override;
	virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;

	virtual void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state) override;
	virtual void OnRenegotiationNeeded() override;
	virtual void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel) override;
	virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state) override;

	// CreateOffer(), CreateAnswer()가 호출되었을 때 trigger 된다.
	virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc) override;
	virtual void OnFailure(webrtc::RTCError error) override;

	virtual void AddRef() const override;
	virtual rtc::RefCountReleaseStatus Release() const override;

	void CreatePeerConnection();
	void CreateOfferSdp();
	void SetAnswerSdp(const FString& AnswerSdp);
	void SetLocalDescription(webrtc::SessionDescriptionInterface* desc);
	void SetWebRTCBridge(TSharedPtr<CodeStoryWebRTCBridge, ESPMode::ThreadSafe> Bridge);

	// Proxy 서버로부터 온 iceCandidate 데이터를 추가합니다.
	void AddIceCandidateFromProxyServer(FString iceCandidate);
	
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> GetPeerConnection();

private:
	rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> PeerConnectionFactory;
	TSharedRef<CodeStoryVideoStreamReceiver> VideoReceiver;
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> PeerConnection = nullptr;
	TSharedPtr<CodeStoryWebRTCBridge, ESPMode::ThreadSafe> Bridge;
	webrtc::VideoTrackInterface* VideoTrack;
};
