// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CodeStoryWebRTCFacade.h"
#include "CodeStoryWebRTCClient.h"
#include "CodeStoryWebRTCBridge.h"
#include "CodeStoryVideoStreamReceiver.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "GameFramework/Actor.h"
#include "NewActorComponent2.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WEBRTCPROJ_API UNewActorComponent2 :
	public UActorComponent,
	public FCodeStoryVideoStreamReceiverInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNewActorComponent2();

	virtual void OnFrame(rtc::scoped_refptr<webrtc::I420BufferInterface> FrameBuffer, int VideoWidth, int VideoHeight) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	TSharedPtr<WebSocketWrapper> WebSocket;
	TSharedPtr<CodeStoryWebRTCClient, ESPMode::ThreadSafe> WebRtcClient;
	TSharedPtr<CodeStoryWebRTCBridge, ESPMode::ThreadSafe> WebRtcBridge;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
	bool ShouldResize;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
	FVector2D ResizeDeminsions;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
	float RefreshRate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Webcam)
	float RefreshTimer;
	
	cv::Mat Frame;
	cv::VideoCapture Stream;
	cv::Size Size;

	void UpdateFrame(rtc::scoped_refptr<webrtc::I420BufferInterface> FrameBuffer);
	
	UFUNCTION(BlueprintCallable, Category = Webcam)
	void DoProcessing();
	
	UFUNCTION(BlueprintCallable, Category = Webcam)
	void UpdateTexture();

	FVector2D VideoSize;

	UPROPERTY(BlueprintReadOnly, Category = Webcam)
	UTexture2D* VideoTexture;

	TArray<FColor> Data;

	void UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData);
	FUpdateTextureRegion2D* VideoUpdateTextureRegion;
	
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
