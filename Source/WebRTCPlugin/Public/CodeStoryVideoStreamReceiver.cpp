#include "CodeStoryVideoStreamReceiver.h"

void CodeStoryVideoStreamReceiver::OnFrame(const webrtc::VideoFrame& frame)
{
	UE_LOG(LogTemp, Log, TEXT("Received Video Frame id : [%d]"), frame.id());

	const int VideoWidth = frame.width();
	const int VideoHeight = frame.height();

	this->CodeStoryReceiver.Get()->OnFrame(frame.video_frame_buffer()->ToI420(), VideoWidth, VideoHeight);
}

void CodeStoryVideoStreamReceiver::OnDiscardedFrame()
{
	UE_LOG(LogTemp, Log, TEXT("Discarded Video FramE"));
}

void CodeStoryVideoStreamReceiver::OnChanged()
{
	UE_LOG(LogTemp, Log, TEXT("[VideoStreamReceiver] OnChanged"));
}

void CodeStoryRtcReceiverObserver::OnFirstPacketReceived(cricket::MediaType media_type)
{
	UE_LOG(LogTemp, Log, TEXT("[RtcReceiverObserver] FirstPacketReceived"));
}


