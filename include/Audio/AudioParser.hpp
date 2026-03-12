#pragma once
#include <Audio/AudioShared.hpp>
#include <fstream>
#include <filesystem>



class AudioParser {  //IMPORTANT //TODO Add streaming to not load the whole file into memory.
	public:
	  WAVEFORMATEX wfx = {};
	  [[nodiscard]]
	  std::vector<byte> ParseAudioFile(std::string FileName) {
		  std::ifstream File(FileName, std::ios::binary);
			  if (!File.is_open()) {
				  throw std::runtime_error(std::format("Failed to open file: {}\n", FileName));
			  }
		  std::vector<byte> AudioData;
		  auto ext = std::filesystem::path(FileName).extension().string();
		  if (ext == ".wav") {
			  char header[512];
			  File.read(header, sizeof(header));
  
			  std::streamsize BytesRead = File.gcount();
			  std::string Data(header, BytesRead);
			  size_t DataPos = Data.find("data");
			  File.seekg(
				  DataPos +
				  8); // 8 is bytes for "data" and 32 bit integer for size of data
  
			  if (DataPos == std::string::npos) {
				  throw std::runtime_error("Could not find data chunk in WAV\n");
			  }
			  wfx = {};
			  wfx.wFormatTag = WAVE_FORMAT_PCM;
			  wfx.nChannels = *reinterpret_cast<uint16_t *>(&header[22]);
			  wfx.nSamplesPerSec = *reinterpret_cast<uint32_t *>(&header[24]);
			  wfx.wBitsPerSample = *reinterpret_cast<uint16_t *>(&header[34]);
			  wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
			  wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
			  wfx.cbSize = 0;
  
			  AudioData.assign((std::istreambuf_iterator<char>(File)),
							   std::istreambuf_iterator<char>());
			  File.close();
  
		  } else {
			  throw std::runtime_error(std::format("Unsupported format. File extension: {}\n", ext));
		  }
		  return AudioData;
	  }
  };