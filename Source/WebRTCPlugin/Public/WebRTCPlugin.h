// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "CodeStoryWebRTCFacade.h"
#include "CodeStoryWebRTCClient.h"

class WEBRTCPLUGIN_API FWebRTCPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual TSharedPtr<CodeStoryWebRTCClient> GetWebRTC();

	TSharedPtr<CodeStoryWebRTCClient> WebRTCClient;

};
