#pragma once
#include "WebRTCInitializer.h"
#include "media/engine/internal_encoder_factory.h"
#include "media/engine/internal_decoder_factory.h"
#include "media/engine/encoder_simulcast_proxy.h"

class WEBRTCPLUGIN_API CodeStoryVideoDecoderFactory : public webrtc::VideoDecoderFactory
{
public:
	CodeStoryVideoDecoderFactory() : InternalDecoderFactory_(new webrtc::InternalDecoderFactory) {}
	virtual ~CodeStoryVideoDecoderFactory() override;
	
	virtual std::vector<webrtc::SdpVideoFormat> GetSupportedFormats() const override;
	virtual std::unique_ptr<webrtc::VideoDecoder> CreateVideoDecoder(const webrtc::SdpVideoFormat& format) override;

private:
	std::unique_ptr<VideoDecoderFactory> InternalDecoderFactory_;
};

class WEBRTCPLUGIN_API CodeStoryVideoEncoderFactory : public webrtc::VideoEncoderFactory
{
public:
	CodeStoryVideoEncoderFactory() : InternalEncoderFactory_(new webrtc::InternalEncoderFactory) {}
	
	virtual ~CodeStoryVideoEncoderFactory() override;
	virtual std::vector<webrtc::SdpVideoFormat> GetSupportedFormats() const override;
	virtual CodecInfo QueryVideoEncoder(const webrtc::SdpVideoFormat& format) const override;
	virtual std::unique_ptr<webrtc::VideoEncoder> CreateVideoEncoder(const webrtc::SdpVideoFormat& format) override;

private:
	std::unique_ptr<VideoEncoderFactory> InternalEncoderFactory_;
};

std::unique_ptr<CodeStoryVideoEncoderFactory> CreateVideoEncoderFactory();
std::unique_ptr<CodeStoryVideoDecoderFactory> CreateVideoDecoderFactory();
