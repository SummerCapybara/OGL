#pragma once
#include <xaudio2.h>
#include <vector>

struct Sound {
	std::vector<byte> AudioData;
	XAUDIO2_BUFFER XAudioBuffer;
	IXAudio2SourceVoice* SourceVoicePointer;
};