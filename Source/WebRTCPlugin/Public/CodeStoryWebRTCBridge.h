#pragma once
#include "CodeStoryWebRTCClient.h"
#include "WebSocketWrapper.h"

class WEBRTCPLUGIN_API CodeStoryWebRTCObserver
{
public:
	virtual void OnSuccessCreatedOffer(std::string OfferSdp) = 0;
	virtual void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) = 0;
	// 시그널링이 트랜시버가 수신 할것임을 나타낼 때 호출된다. Remote Endpoint의 미디어이다. 이것은 SetRemoteDescription()이 호출될 때 trigger 된다.
	virtual void OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams) = 0;
	virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) = 0;
};

class WEBRTCPLUGIN_API CodeStoryWebRTCBridge : public WebSocketObserver, public CodeStoryWebRTCObserver
{
public:
	CodeStoryWebRTCBridge(CodeStoryWebRTCClient &Client, WebSocketWrapper &Socket)
		: Client(Client), Socket(Socket)
	{
	}

	void ConnectToSignalingProxyServer();
	void CreatePeerConnection();

	virtual void OnSuccessCreatedOffer(std::string OfferSdp) override;
	virtual void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream) override;
	// 시그널링이 트랜시버가 수신 할것임을 나타낼 때 호출된다. Remote Endpoint의 미디어이다. 이것은 SetRemoteDescription()이 호출될 때 trigger 된다.
	virtual void OnAddTrack(
		rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams) override;
	virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate) override;

	virtual void OnConnect();
	virtual void OnConnectionError(const FString& Error);
	virtual void OnClose(int32 StatusCode, const FString& Reason, bool bWasClean);
	virtual void OnMessage(const FString & Message);
	virtual void OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining);
	virtual void OnMessageSent(const FString& MessageString);
private:
	CodeStoryWebRTCClient& Client;
	WebSocketWrapper& Socket;
};
