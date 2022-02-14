#include "CodeStoryWebRTCBridge.h"

void CodeStoryWebRTCBridge::OnConnect()
{
	// Lambda에서 참조로 함수 호출 X
	this->Client.OnConnect();
}

void CodeStoryWebRTCBridge::OnMessage(const FString& Message)
{
	this->Client.OnMessage(Message);
}

void CodeStoryWebRTCBridge::OnConnectionError(const FString& Error)
{
	this->Client.OnConnectionError(Error);	
}

void CodeStoryWebRTCBridge::OnMessageSent(const FString& MessageString)
{
	this->Client.OnMessageSent(MessageString);
}

void CodeStoryWebRTCBridge::OnClose(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	this->Client.OnClose(StatusCode, Reason, bWasClean);
}

void CodeStoryWebRTCBridge::OnRawMessage(const void* Data, SIZE_T Size, SIZE_T BytesRemaining)
{
	this->Client.OnRawMessage(Data, Size, BytesRemaining);
}

void CodeStoryWebRTCBridge::ConnectToSignalingProxyServer()
{
	Socket.CreateSocket();
	
	Socket.GetSocket().Get()->OnConnected().AddLambda([this]() -> void { OnConnect(); });
	Socket.GetSocket().Get()->OnConnectionError().AddLambda([this](const FString& Error) -> void { OnConnectionError(Error); });
	Socket.GetSocket().Get()->OnMessage().AddLambda([this](const FString& Message) -> void { OnMessage(Message); });
	Socket.GetSocket().Get()->OnRawMessage().AddLambda([this](const void* Data, SIZE_T Size, SIZE_T ByteRemaining) -> void { OnRawMessage(Data, Size, ByteRemaining); });
	Socket.GetSocket().Get()->OnMessageSent().AddLambda([this](const FString& Message) -> void { OnMessageSent(Message); });
	Socket.GetSocket().Get()->OnClosed().AddLambda([this](int32 StatusCode, const FString& Reason, bool bWasClean) -> void { OnClose(StatusCode, Reason, bWasClean); });
	
	Socket.GetSocket().Get()->Connect();
}

void CodeStoryWebRTCBridge::CreatePeerConnection()
{
	Client.CreatePeerConnection();
}

void CodeStoryWebRTCBridge::OnSuccessCreatedOffer(std::string OfferSdp)
{
	// Local description 등록
	FString OfferSdpFS = UTF8_TO_TCHAR(OfferSdp.c_str());
	OfferSdpFS = OfferSdpFS.Replace(TEXT("\n"), TEXT("\\n"));
	
	TSharedRef<FJsonObject> OfferJson = MakeShareable(new FJsonObject);
	OfferJson->SetStringField("id", "viewer");
	OfferJson->SetStringField("sdpOffer", OfferSdpFS);

	Socket.Send(OfferJson);
}

void CodeStoryWebRTCBridge::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
	
}

void CodeStoryWebRTCBridge::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
	std::string CandidateName;
	candidate->ToString(&CandidateName);
	
	TSharedRef<FJsonObject> OnIceCandidateJson = MakeShareable(new FJsonObject);
	OnIceCandidateJson->SetStringField("id", "onIceCandidate");
	TSharedRef<FJsonObject> IceCandidateJson = MakeShareable(new FJsonObject);
	IceCandidateJson->SetStringField("candidate", CandidateName.c_str());
	IceCandidateJson->SetStringField("sdpMid", candidate->sdp_mid().c_str());
	IceCandidateJson->SetNumberField("sdpMLineIndex", candidate->sdp_mline_index());
	OnIceCandidateJson->SetObjectField("candidate", IceCandidateJson);
	
	Socket.Send(OnIceCandidateJson);
}

void CodeStoryWebRTCBridge::OnAddTrack(rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver, const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams)
{
	UE_LOG(LogTemp, Log, TEXT("OnAddTrack()"));
}


