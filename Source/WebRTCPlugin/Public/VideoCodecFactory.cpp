#include "VideoCodecFactory.h"
#include "modules/video_coding/codecs/h264/include/h264.h"
#include "modules/video_coding/codecs/vp8/include/vp8.h"
#include "modules/video_coding/codecs/vp9/include/vp9.h"

CodeStoryVideoEncoderFactory::~CodeStoryVideoEncoderFactory()
{
	
}

std::vector<webrtc::SdpVideoFormat> CodeStoryVideoEncoderFactory::GetSupportedFormats() const
{
	std::vector<webrtc::SdpVideoFormat> VideoFormats;

	VideoFormats.push_back(webrtc::SdpVideoFormat(cricket::kVp8CodecName));
	VideoFormats.push_back(webrtc::CreateH264Format(webrtc::H264::kProfileBaseline, webrtc::H264::kLevel3_1, "1"));
	VideoFormats.push_back(webrtc::CreateH264Format(webrtc::H264::kProfileBaseline, webrtc::H264::kLevel3_1, "0"));
	VideoFormats.push_back(webrtc::CreateH264Format(webrtc::H264::kProfileConstrainedBaseline, webrtc::H264::kLevel3_1, "1"));
	VideoFormats.push_back(webrtc::CreateH264Format(webrtc::H264::kProfileConstrainedBaseline, webrtc::H264::kLevel3_1, "0"));
	VideoFormats.push_back(webrtc::SdpVideoFormat(cricket::kAv1CodecName));
	VideoFormats.push_back(webrtc::SdpVideoFormat(cricket::kVp9CodecName));
	
	return VideoFormats;
}

webrtc::VideoEncoderFactory::CodecInfo CodeStoryVideoEncoderFactory::QueryVideoEncoder(const webrtc::SdpVideoFormat& format) const
{
	CodecInfo Info = { false, false };

	return Info;
}

std::unique_ptr<webrtc::VideoEncoder> CodeStoryVideoEncoderFactory::CreateVideoEncoder(const webrtc::SdpVideoFormat& format)
{
	webrtc::InternalEncoderFactory factory;
	
	return std::make_unique<webrtc::EncoderSimulcastProxy>(InternalEncoderFactory_.get(), format);
}

std::unique_ptr<CodeStoryVideoEncoderFactory> CreateVideoEncoderFactory()
{
	return std::make_unique<CodeStoryVideoEncoderFactory>();
}

CodeStoryVideoDecoderFactory::~CodeStoryVideoDecoderFactory()
{
	
}

std::vector<webrtc::SdpVideoFormat> CodeStoryVideoDecoderFactory::GetSupportedFormats() const
{
	std::vector<webrtc::SdpVideoFormat> VideoFormats;

	VideoFormats.push_back(webrtc::SdpVideoFormat(cricket::kVp8CodecName));
	VideoFormats.push_back(webrtc::CreateH264Format(webrtc::H264::kProfileBaseline, webrtc::H264::kLevel3_1, "1"));
	VideoFormats.push_back(webrtc::CreateH264Format(webrtc::H264::kProfileBaseline, webrtc::H264::kLevel3_1, "0"));
	VideoFormats.push_back(webrtc::CreateH264Format(webrtc::H264::kProfileConstrainedBaseline, webrtc::H264::kLevel3_1, "1"));
	VideoFormats.push_back(webrtc::CreateH264Format(webrtc::H264::kProfileConstrainedBaseline, webrtc::H264::kLevel3_1, "0"));
	VideoFormats.push_back(webrtc::SdpVideoFormat(cricket::kAv1CodecName));
	VideoFormats.push_back(webrtc::SdpVideoFormat(cricket::kVp9CodecName));
	
	return VideoFormats;
}

std::unique_ptr<webrtc::VideoDecoder> CodeStoryVideoDecoderFactory::CreateVideoDecoder(const webrtc::SdpVideoFormat& format)
{
	if(format.name == "H264")
	{
		return webrtc::H264Decoder::Create();
	}
	if(format.name == "VP8")
	{
		return webrtc::VP8Decoder::Create();
	}
	if(format.name == "VP9")
	{
		return webrtc::VP9Decoder::Create();
	}
	
	return nullptr;
}

std::unique_ptr<CodeStoryVideoDecoderFactory> CreateVideoDecoderFactory()
{
	return std::make_unique<CodeStoryVideoDecoderFactory>();
}
