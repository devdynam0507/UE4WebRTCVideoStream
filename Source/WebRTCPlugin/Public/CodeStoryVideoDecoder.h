#pragma once

#include "WebRTCInitializer.h"
#include "VideoDecoder.h"

class WEBRTCPLUGIN_API CodeStoryVideoDecoderCallback : public webrtc::DecodedImageCallback
{
public:
	virtual int32_t Decoded(webrtc::VideoFrame& decodedImage);
};

class WEBRTCPLUGIN_API CodeStoryVideoDecoder : public webrtc::VideoDecoder
{
public:
	virtual int32_t InitDecode(const webrtc::VideoCodec* codec_settings,
							   int32_t number_of_cores) = 0;

	virtual int32_t Decode(const webrtc::EncodedImage& input_image,
						   bool missing_frames,
						   int64_t render_time_ms) = 0;

	virtual int32_t RegisterDecodeCompleteCallback(
		webrtc::DecodedImageCallback* callback) = 0;

	virtual int32_t Release() = 0;

	// Returns true if the decoder prefer to decode frames late.
	// That is, it can not decode infinite number of frames before the decoded
	// frame is consumed.
	virtual bool PrefersLateDecoding() const;

	virtual const char* ImplementationName() const;
};
