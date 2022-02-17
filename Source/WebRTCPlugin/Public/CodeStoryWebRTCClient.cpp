#include "CodeStoryWebRTCClient.h"
#include "VideoCodecFactory.h"

DECLARE_LOG_CATEGORY_EXTERN(CodeStoryWebRTCClientLog, Log, All);
DEFINE_LOG_CATEGORY(CodeStoryWebRTCClientLog);

// ============================================================================================ //
//
//							WebSocket Observer Implementations!
//
// ============================================================================================ //

void CodeStoryWebRTCClient::OnConnect()
{
	UE_LOG(LogTemp, Warning, TEXT("Connected WebRTC Proxy Server"));
	CreateOfferSdp();
}

void CodeStoryWebRTCClient::OnMessage(const FString& Message)
{
	UE_LOG(LogTemp, Log, TEXT("Response Message %s"), *Message);
	
	TSharedPtr<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Message);
	TSharedPtr<FJsonObject> Response = MakeShareable(new FJsonObject());
	FJsonSerializer::Deserialize(JsonReader.ToSharedRef(), Response);

	FString ResponseId = Response.Get()->GetStringField("id"); // viewerResponse, presenterResponse, iceCandidate
	FString ResponseType = Response.Get()->GetStringField("response"); // rejected, accepted

	if(ResponseId.Equals("iceCandidate"))
	{
		FString	SdpMid = Response.Get()->GetStringField("sdpMid");
		FString CandidateString = Response.Get()->GetStringField("candidate");
		FString SdpMLineIndex = Response.Get()->GetStringField("sdpMLineIndex");
		webrtc::SdpParseError _SdpParseError;

		std::string ParsedSdpMid = TCHAR_TO_UTF8(*SdpMid);
		std::string ParsedCandidate = TCHAR_TO_UTF8(*CandidateString);
		int32 ParsedMLineIndex = FCString::Atoi(*SdpMLineIndex);
		UE_LOG(LogTemp, Log, TEXT("sdpMLineIndex %d"), ParsedMLineIndex);
		
		webrtc::IceCandidateInterface *Candidate = webrtc::CreateIceCandidate(
		   ParsedSdpMid,
	       ParsedMLineIndex,
	       ParsedCandidate,
		 &_SdpParseError
		 );

		const cricket::Candidate& candidate = Candidate->candidate();
		std::string id = candidate.id();
		std::string dd = candidate.foundation();
		std::string cc = candidate.network_name();
		std::string ne = candidate.network_name();
		std::string tt = candidate.tcptype();
		std::string ttt = candidate.transport_name();
		std::string af = candidate.ToString();
		const std::string spdmid = Candidate->sdp_mid();
		const int ind = Candidate->sdp_mline_index();
		const std::string url = Candidate->server_url();
		PeerConnection.get()->AddIceCandidate(Candidate);
		return;
	}
	
	// 만약 Remote Stream과 SDP 교환이 완료된다면 Remote Stream의 SDP Answer가 반환된다.
	if(ResponseType.Equals("accepted"))
	{
		FString RemoteSdpAnswer = Response.Get()->GetStringField("sdpAnswer");
		SetAnswerSdp(RemoteSdpAnswer);
	}
	else
	{
		FString ErrorMessage = Response.Get()->GetStringField("message");
		UE_LOG(LogTemp, Log, TEXT("Signaling Message %s"), *ErrorMessage);
	}
}

void CodeStoryWebRTCClient::OnConnectionError(const FString& Error)
{
	UE_LOG(LogTemp, Error, TEXT("Connection Error! %s"), *Error);
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

void CodeStoryWebRTCClient::AddIceCandidateFromProxyServer(FString iceCandidate)
{
	
}


void CodeStoryWebRTCClient::CreatePeerConnection()
{
	webrtc::PeerConnectionInterface::RTCConfiguration Config;
	Config.sdp_semantics = webrtc::SdpSemantics::kUnifiedPlan;
	Config.enable_dtls_srtp = true;

	webrtc::PeerConnectionInterface::IceServer GoogleStun;
	GoogleStun.uri = "stun:stun.l.google.com:19302";
	webrtc::PeerConnectionInterface::IceServer StunProtocol;
	StunProtocol.uri = "stun:stun.stunprotocol.org:3478";
	Config.servers.push_back(GoogleStun);
	Config.servers.push_back(StunProtocol);
	
	PeerConnection = PeerConnectionFactory -> CreatePeerConnection(
		Config, nullptr, nullptr, this
	);
}

rtc::scoped_refptr<webrtc::PeerConnectionInterface> CodeStoryWebRTCClient::GetPeerConnection()
{
	return PeerConnection;
}

void CodeStoryWebRTCClient::CreateOfferSdp()
{
	CodeStoryWebRTCThread::WORKER_THREAD->Invoke<void>(RTC_FROM_HERE, [this]()
	{
		int offer_to_receive_video = webrtc::PeerConnectionInterface::RTCOfferAnswerOptions::kOfferToReceiveMediaTrue; // 1로 설정시 receive
		int offer_to_receive_audio = webrtc::PeerConnectionInterface::RTCOfferAnswerOptions::kOfferToReceiveMediaTrue; // 0으로 설정시 send only
		bool voice_activity_detection = false;
		bool ice_restart = true;
		bool use_rtp_mux = true;

		webrtc::RtpTransceiverInit TInit;
		TInit.direction = webrtc::RtpTransceiverDirection::kRecvOnly;
		PeerConnection.get()->AddTransceiver(cricket::MEDIA_TYPE_VIDEO,TInit);
		PeerConnection.get()->GetTransceivers()[0].get()->receiver().get()->SetObserver(new CodeStoryRtcReceiverObserver);
		PeerConnection.get()->AddTransceiver(cricket::MEDIA_TYPE_AUDIO, TInit);
		
		PeerConnection.get()->CreateOffer(this, webrtc::PeerConnectionInterface::RTCOfferAnswerOptions(
			offer_to_receive_video,
			offer_to_receive_audio,
			voice_activity_detection,
			ice_restart,
			use_rtp_mux
		));
	});
}

void CodeStoryWebRTCClient::SetAnswerSdp(const FString& AnswerSdp)
{
	std::string SdpStdString = TCHAR_TO_ANSI(*AnswerSdp);
	// std::unique_ptr<webrtc::SessionDescriptionInterface> RemoteSDPInstance(std::move(CreateSessionDescription(webrtc::SdpType::kAnswer, SdpStdString, nullptr)));

	//this->AnswerSdp = std::move(CreateSessionDescription(webrtc::SdpType::kAnswer, SdpStdString, nullptr));

	webrtc::SessionDescriptionInterface* AnswerSdpInstance = webrtc::CreateSessionDescription(webrtc::SdpTypeToString(webrtc::SdpType::kAnswer), SdpStdString, nullptr);
	CodeStoryPeerSetSessionDescriptionObserver* PeerSetObserver = CodeStoryPeerSetSessionDescriptionObserver::CreateObserver();
	PeerConnection->SetRemoteDescription(PeerSetObserver, AnswerSdpInstance);
}

void CodeStoryWebRTCClient::SetLocalDescription(webrtc::SessionDescriptionInterface* desc)
{
	CodeStoryPeerSetSessionDescriptionObserver* PeerSetObserver = CodeStoryPeerSetSessionDescriptionObserver::CreateObserver();
	PeerConnection->SetLocalDescription(PeerSetObserver, desc);
}

void CodeStoryWebRTCClient::SetWebRTCBridge(TSharedPtr<CodeStoryWebRTCBridge, ESPMode::ThreadSafe> _Bridge)
{
	this->Bridge = _Bridge;
}

// ============================================================================================ //
//
//							Peer Connection Observer Implementations!
//
// ============================================================================================ //

void CodeStoryWebRTCClient::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
	UE_LOG(LogTemp, Log, TEXT("On Add Stream id : %d"))
	Bridge->OnAddStream(stream);
}

void CodeStoryWebRTCClient::OnAddTrack(
	rtc::scoped_refptr<webrtc::RtpReceiverInterface> receiver,
	const std::vector<rtc::scoped_refptr<webrtc::MediaStreamInterface>>& streams
)
{
	const auto Track = receiver->track().get();
	
	if(Track->kind() == "video")
	{
		VideoTrack = static_cast<webrtc::VideoTrackInterface*>(Track);
//		VideoTrack->GetSource()->AddOrUpdateSink(new CodeStoryVideoStreamReceiver, rtc::VideoSinkWants());
		VideoTrack -> AddOrUpdateSink(&VideoReceiver.Get(), rtc::VideoSinkWants());
	}
	
	Bridge->OnAddTrack(receiver, streams);
}

void CodeStoryWebRTCClient::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
	UE_LOG(LogTemp, Log, TEXT("Called OnIceCandidate()"));
	PeerConnection->AddIceCandidate(candidate);
	std::string CandidateName;

	candidate->ToString(&CandidateName);
	UE_LOG(LogTemp, Log, TEXT("%s"), UTF8_TO_TCHAR(CandidateName.c_str()));

	Bridge->OnIceCandidate(candidate);
}

void CodeStoryWebRTCClient::OnSuccess(webrtc::SessionDescriptionInterface* desc)
{
	std::string OfferSdp;
	
	desc->ToString(&OfferSdp);
	SetLocalDescription(desc);
	Bridge.Get()->OnSuccessCreatedOffer(OfferSdp);
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
	UE_LOG(LogTemp, Log, TEXT("OnSignalingChange()"));
}

void CodeStoryWebRTCClient::OnRenegotiationNeeded()
{
	UE_LOG(LogTemp, Log, TEXT("OnRenegotitationNeeded()"));
}

void CodeStoryWebRTCClient::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel)
{
}

void CodeStoryWebRTCClient::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
	UE_LOG(LogTemp, Log, TEXT("OnIceGatheringChange()"));

	if(webrtc::PeerConnectionInterface::IceGatheringState::kIceGatheringComplete == new_state)
	{
	}
}

void CodeStoryWebRTCClient::AddRef() const
{
	
}

rtc::RefCountReleaseStatus CodeStoryWebRTCClient::Release() const
{
	return rtc::RefCountReleaseStatus::kDroppedLastRef;
}

