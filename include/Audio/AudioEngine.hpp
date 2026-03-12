#pragma once
#include <Audio/AudioShared.hpp>
#include <stdexcept>

class AudioEngine {
	private:
	  IXAudio2 *XAudio2Pointer = nullptr;
	  IXAudio2MasteringVoice *MasterVoicePointer = nullptr;
	  std::vector<IXAudio2SourceVoice *> SourceVoicePointers;
  
	public:
	  AudioEngine() {
		  if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) {
			  throw std::runtime_error("COM initialization failed.\n");
		  }
  
		  if (FAILED(
				  XAudio2Create(&XAudio2Pointer, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
			  throw std::runtime_error("Failed to create XAudio2 engine.\n");
		  }
  
		  if (FAILED(XAudio2Pointer->CreateMasteringVoice(&MasterVoicePointer))) {
			  throw std::runtime_error("Failed to create mastering voice.\n");
		  }
	  }
	  [[nodiscard]]
	   Sound CreateSoundObject(std::vector<byte> AudioData,
											 WAVEFORMATEX &wfx) {
		  Sound Sound;
		  Sound.SourceVoicePointer = nullptr;
		  Sound.AudioData = AudioData;
		  if (FAILED(
				  XAudio2Pointer->CreateSourceVoice(&Sound.SourceVoicePointer, &wfx))) {
			  throw std::runtime_error("Failed to create source voice.\n");
		  }
		  Sound.XAudioBuffer = {};
		  Sound.XAudioBuffer.AudioBytes = static_cast<UINT32>(Sound.AudioData.size());
		  Sound.XAudioBuffer.pAudioData = Sound.AudioData.data();
		  Sound.XAudioBuffer.Flags = XAUDIO2_END_OF_STREAM;
  
		  SourceVoicePointers.push_back(Sound.SourceVoicePointer);
		  return Sound;
	  }
	  ~AudioEngine() {
		  for (auto v : SourceVoicePointers) {
			  v->DestroyVoice();
		  }
		  if (MasterVoicePointer) {
			  MasterVoicePointer->DestroyVoice();
		  }
		  if (XAudio2Pointer)
			  XAudio2Pointer->Release();
		  CoUninitialize();
	  }
  };