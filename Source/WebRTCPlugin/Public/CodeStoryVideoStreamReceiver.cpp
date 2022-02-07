#include "CodeStoryVideoStreamReceiver.h"

void CodeStoryVideoStreamReceiver::OnFrame(const webrtc::VideoFrame& frame)
{
	UE_LOG(LogTemp, Log, TEXT("Received Video Frame id : [%d]"), frame.id());
}
