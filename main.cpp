#include <iostream>
#include <sndfile.h>
#include <array>
#include <sndfile.hh>
#include <vector>

const float SILENCE_THRESHOLD = 0.02f;
const float TARGET_AMPLITUDE = 0.99f;

size_t find_start_of_sound(const SF_INFO &sfInfo, const float *buffer, sf_count_t numFrames) noexcept {
  for (size_t i = 0; i < numFrames * sfInfo.channels; i += sfInfo.channels) {
    bool isSilent = true;
    for (int ch = 0; ch < sfInfo.channels; ++ch) {
      if (std::abs(buffer[i + ch]) > SILENCE_THRESHOLD) {
        isSilent = false;
        break;
      }
    }

    if (!isSilent) {
      return i / sfInfo.channels;
    }
  }
  return 0;
}

size_t find_end_of_sound(const SF_INFO &sfInfo, const float *buffer, sf_count_t numFrames, size_t start = 0) noexcept {
  for (size_t i = numFrames * sfInfo.channels - sfInfo.channels; i > start * sfInfo.channels; i -= sfInfo.channels) {
    bool isSilent = true;
    for (int ch = 0; ch < sfInfo.channels; ++ch) {
      if (std::abs(buffer[i + ch]) > SILENCE_THRESHOLD) {
        isSilent = false;
        break;
      }
    }

    if (!isSilent) {
      return (i / sfInfo.channels) + 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {

  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <input file> <output file> [duration in seconds]" << std::endl;
    return 1;
  }

  const char *inputFile = argv[1];
  const char *outputFile = argv[2];
  float MAX_DURATION = 1.0f; // 1 Секунда за замовчуванням

  // Перевіряємо, чи введена тривалість
  if (argc >= 4) {
    try {
      MAX_DURATION = std::stof(argv[3]);
      if (MAX_DURATION <= 0.0f) {
        std::cerr << "Duration must be greater than 0" << std::endl;
        return 1;
      }
    } catch (const std::exception &e) {
      std::cerr << "Invalid duration: " << argv[3] << std::endl;
      return 1;
    }
  }

  SF_INFO sfInfo;
  SNDFILE *input = sf_open(inputFile, SFM_READ, &sfInfo);

  if (!input) {
    std::cerr << "Error opening input file: " << inputFile << std::endl;
    return 1;
  }

  std::vector<float> buffer(sfInfo.frames * sfInfo.channels);
  sf_count_t numFrames = sf_readf_float(input, buffer.data(), sfInfo.frames);

  sf_close(input);

  // Межі звуку
  size_t start = find_start_of_sound(sfInfo, buffer.data(), numFrames);
  size_t end = find_end_of_sound(sfInfo, buffer.data(), numFrames, start);

  if (start >= end) {
    std::cerr << "No sound found in input file" << std::endl;
    return 1;
  }

  // Обрізаємо звук до MAX_DURATION секунди
  //size_t maxSamples = sfInfo.samplerate * sfInfo.channels * MAX_DURATION;
  if ((end - start) > sfInfo.samplerate * MAX_DURATION) {
    end = start + sfInfo.samplerate  * MAX_DURATION;
  }

  // Знаходимо максимальну амплітуду звука
  float maxAmplitude = 0.0f;
  for(size_t i = start * sfInfo.channels; i < end * sfInfo.channels; ++i) {
    maxAmplitude = std::max(maxAmplitude, buffer[i]);
  }

  // Якщо амплітуда більше ніж ми хочемо, нормалізуємо її.
  if(maxAmplitude > TARGET_AMPLITUDE) {
    float scale = TARGET_AMPLITUDE / maxAmplitude;
    for(size_t i = start * sfInfo.channels; i < end * sfInfo.channels; ++i) {
      buffer[i] *= scale;
    }
  }

  SndfileHandle output(outputFile, SFM_WRITE, sfInfo.format, sfInfo.channels, sfInfo.samplerate);

  output.write(&buffer[start * sfInfo.channels], (end - start) * sfInfo.channels);

  std::cout << "Sound extracted from " << inputFile << " and saved to " << outputFile << std::endl;

  return 0;
}
