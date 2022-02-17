#include "CodeStoryPeerConnectionObserver.h"

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
	UE_LOG(LogTemp, Log, TEXT("Failure set local description %s"), UTF8_TO_TCHAR(error.message()));
}

