#pragma once

#include "WebRTCInitializer.h"

namespace CodeStoryWebRTCThread
{
	static std::unique_ptr<rtc::Thread> SIGNALING_THREAD = rtc::Thread::CreateWithSocketServer();
	static std::unique_ptr<rtc::Thread> WORKER_THREAD = rtc::Thread::Create();
	static std::unique_ptr<rtc::Thread> NETWORK_THREAD = rtc::Thread::Create();
}
