#include "CodeStoryVideoDecoder.h"

int32_t CodeStoryVideoDecoderCallback::Decoded(webrtc::VideoFrame& decodedImage)
{
	return -1;
}

// ================================================================================= //

int32_t CodeStoryVideoDecoder::RegisterDecodeCompleteCallback(webrtc::DecodedImageCallback* callback)
{
	// TODO: Setup the Decoded Image Callback

	return WEBRTC_VIDEO_CODEC_OK;
}

int32_t CodeStoryVideoDecoder::InitDecode(const webrtc::VideoCodec* codec_settings, int32_t number_of_cores)
{
	return 0;
}

bool CodeStoryVideoDecoder::PrefersLateDecoding() const
{
	return true;
}

int32_t CodeStoryVideoDecoder::Decode(const webrtc::EncodedImage& input_image, bool missing_frames, int64_t render_time_ms)
{
	return 0;
}

const char* CodeStoryVideoDecoder::ImplementationName() const
{
	return "CodeStoryVDecoder";
}

int32_t CodeStoryVideoDecoder::Release()
{
	return 0;
}
