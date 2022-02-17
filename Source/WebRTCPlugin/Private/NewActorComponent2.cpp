// Fill out your copyright notice in the Description page of Project Settings.

#include "NewActorComponent2.h"
#include "opencv2/imgproc.hpp"
#include "Rendering/Texture2DResource.h"

// Sets default values for this component's properties
UNewActorComponent2::UNewActorComponent2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	RefreshRate = 15.0f;
	VideoSize = FVector2D(0, 0);
	ShouldResize = true;
	ResizeDeminsions = FVector2D(320, 240);
	RefreshTimer = 0.0f;
	Frame = cv::Mat();
	// ...
}


// Called when the game starts
void UNewActorComponent2::BeginPlay()
{
	Super::BeginPlay();

	const TSharedRef<CodeStoryVideoStreamReceiver> VideoCallbackImpl = MakeShared<CodeStoryVideoStreamReceiver>(MakeShareable(this));
	WebSocket = MakeShareable(
		CodeStoryWebRTCFacade::CreateWebSocket(TEXT("ws://45.32.249.81:7777/"), CodeStoryWebSocket::EnumToString(CodeStoryWebSocket::WS))
	);
	WebRtcClient = MakeShared<CodeStoryWebRTCClient, ESPMode::ThreadSafe>(CodeStoryWebRTCFacade::CreateClient(VideoCallbackImpl));
	WebRtcBridge = MakeShared<CodeStoryWebRTCBridge, ESPMode::ThreadSafe>(CodeStoryWebRTCFacade::CreateWebRTC(*WebRtcClient.Get(), *WebSocket.Get()));
	
	WebRtcClient->SetWebRTCBridge(WebRtcBridge);
	WebRtcBridge.Get()->CreatePeerConnection();
	WebRtcBridge.Get()->ConnectToSignalingProxyServer();
}


// Called every frame
void UNewActorComponent2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNewActorComponent2::DoProcessing()
{
}

void UNewActorComponent2::UpdateTexture()
{
	for(int y = 0; y < VideoSize.Y; y++)
	{
		for(int x = 0; x < VideoSize.X; x++)
		{
			int i = x + y * VideoSize.X;

			Data[i].B = Frame.data[i * 3];
			Data[i].G = Frame.data[i * 3 + 1];
			Data[i].R = Frame.data[i * 3 + 2];
		}
	}

	UpdateTextureRegions(VideoTexture, 0, 1, VideoUpdateTextureRegion, 4 * VideoSize.X, 4, (uint8*) Data.GetData(), false);	
}

void UNewActorComponent2::UpdateTextureRegions(UTexture2D* Texture, int32 MipIndex, uint32 NumRegions, FUpdateTextureRegion2D* Regions, uint32 SrcPitch, uint32 SrcBpp, uint8* SrcData, bool bFreeData)
{
	if(Texture->Resource)
	{
		struct FUpdateTextureRegionsData
		{
			FTexture2DResource* Texture2DResource;
			int32 MipIndex;
			uint32 NumRegions;
			FUpdateTextureRegion2D* Regions;
			uint32 SrcPitch;
			uint32 SrcBpp;
			uint8* SrcData;
		};

		FUpdateTextureRegionsData* RegionData = new FUpdateTextureRegionsData;

		RegionData->Texture2DResource = static_cast<FTexture2DResource*>(Texture->Resource);
		RegionData->MipIndex = MipIndex;
		RegionData->NumRegions = NumRegions;
		RegionData->Regions = Regions;
		RegionData->SrcPitch = SrcPitch;
		RegionData->SrcBpp = SrcBpp;
		RegionData->SrcData = SrcData;

		ENQUEUE_RENDER_COMMAND(SceneDrawCompletion)([=](FRHICommandListImmediate& RHICmdList)
		{
			for(uint32 RegionIndex = 0; RegionIndex < RegionData->NumRegions; ++RegionIndex)
			{
				int32 CurrentFirstMip = RegionData->Texture2DResource->GetCurrentFirstMip();

				if(RegionData->MipIndex >= CurrentFirstMip)
				{
					RHIUpdateTexture2D(
						RegionData->Texture2DResource->GetTexture2DRHI(),
						RegionData->MipIndex - CurrentFirstMip,
						RegionData->Regions[RegionIndex],
						RegionData->SrcPitch,
						RegionData->SrcData + RegionData->Regions[RegionIndex].SrcY + RegionData->SrcPitch + RegionData->Regions[RegionIndex].SrcX + RegionData->SrcBpp
					);
				}
			}

			if(bFreeData)
			{
				FMemory::Free(RegionData->Regions);
				FMemory::Free(RegionData->SrcData);
			}
			delete RegionData;
		});
	}
}

void UNewActorComponent2::UpdateFrame(rtc::scoped_refptr<webrtc::I420BufferInterface> FrameBuffer)
{
	/* i420 to RGB 변환 후 UE4 텍스처에 렌더링 해야함 */
	cv::Mat iy, iu, iv, yuv;
	cv::Mat iextu, iextv;
	const int PixelsInFrame = VideoSize.X * VideoSize.Y;
	int FrameWidth = static_cast<int>(VideoSize.X);
	int FrameHeight = static_cast<int>(VideoSize.Y);
	iy.create(VideoSize.Y, VideoSize.X, CV_8UC1);
	iu.create(FrameHeight >> 1, FrameWidth >> 1, CV_8UC1);
	iv.create(FrameHeight >> 1, FrameWidth >> 1, CV_8UC1);
	yuv.create(VideoSize.Y, VideoSize.X, CV_8UC3);

	std::vector<uint8> i420YVector(PixelsInFrame); // Y를 전체 Frame 크기만큼 읽는다.
	std::vector<uint8> i420UVector(PixelsInFrame >> 2); // U를 전체 프레임크기에서 1/4 만 읽는다.
	std::vector<uint8> i420VVector(PixelsInFrame >> 2); // V를 전체 프레임 크기에서 1/4만 읽는다.
	const uint8_t* Y = FrameBuffer.get()->DataY();
	const uint8_t* U = FrameBuffer.get()->DataU();
	const uint8_t* V = FrameBuffer.get()->DataV();
	
	// webrtc i420 buffer를 읽어서 i420YVector에 넣는다.
	for(int i = 0; i < PixelsInFrame; i++)
	{
		i420YVector.push_back(Y[i]);
	}

	// webrtc i420 buffer를 읽어서 i420UVector, i420VVector에 넣는다.
	for(int i = 0; i < PixelsInFrame >> 2; i++)
	{
		i420UVector.push_back(U[i]);
		i420UVector.push_back(V[i]);
	}

	for(int y = 0; y < VideoSize.Y; y++)
	{
		for(int x = 0; x < VideoSize.X; x++)
		{
			iy.at<uchar>(y, x) = i420YVector[y * VideoSize.X + x];
		}
	}

	for(int y = 0; y < FrameHeight >> 1; y++)
	{
		for(int x = 0; x < FrameWidth >> 1; x++)
		{
			iu.at<uchar>(y, x) = i420UVector[y * (FrameWidth >> 1) + x];
			iv.at<uchar>(y, x) = i420VVector[y * (FrameWidth >> 1) + x];
		}
	}

	resize(iu, iextu, iy.size(), 0, 0, 0);
	resize(iv, iextv, iy.size(), 0, 0, 0);

	for(int y = 0; y < FrameHeight; y++)
	{
		for(int x = 0; x < FrameWidth; x++)
		{
			yuv.at<cv::Vec3b>(y, x)[0] = iy.at<uchar>(y, x);
			yuv.at<cv::Vec3b>(y, x)[1] = iextu.at<uchar>(y, x);
			yuv.at<cv::Vec3b>(y, x)[2] = iextv.at<uchar>(y, x);
		}
	}

	cvtColor(yuv, yuv, CV_YCrCb2BGR);
	Frame = yuv;
}

void UNewActorComponent2::OnFrame(rtc::scoped_refptr<webrtc::I420BufferInterface> FrameBuffer, int VideoWidth, int VideoHeight)
{
	if(ShouldResize)
	{
		VideoSize.X = VideoWidth;
		VideoSize.Y = VideoHeight;
		Size = cv::Size(VideoWidth, VideoHeight);
		Data.Init(FColor(0, 0, 0, 255), VideoSize.X * VideoSize.Y);
		VideoTexture = UTexture2D::CreateTransient(VideoSize.X, VideoSize.Y);
		
		ShouldResize = false;
	}
	
	UpdateFrame(FrameBuffer);
	UpdateTexture();
}
