// Copyright Epic Games, Inc. All Rights Reserved.

#include "WebRTCPlugin.h"

#define LOCTEXT_NAMESPACE "FWebRTCPluginModule"

void FWebRTCPluginModule::StartupModule()
{
	//InitializeWebRTC();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FWebRTCPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

TSharedPtr<CodeStoryWebRTCClient> FWebRTCPluginModule::GetWebRTC()
{
	return WebRTCClient;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FWebRTCPluginModule, WebRTCPlugin)