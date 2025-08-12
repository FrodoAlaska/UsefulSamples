#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"

#include <cstdio>
#include <cstddef>

/// You can find the stb_image library at the link below: 
///
/// https://github.com/mackron/dr_libs

int main() {
  /// In order to load a WAV file using drwav, you can call the function 
  /// below with the specific path to the WAV file. The function will 
  /// fill a useful `drwav` struct that can be used later for reading 
  /// PCM frames.
  ///
  /// The `drwav` will include useful information about the WAV file, such as 
  /// the channels, the bits per sample, the sample rate, and so on. This information 
  /// will be relevant later on when reading the PCM frames of the file.
  ///
  /// The function can return an error, however, so make sure to check 
  /// that when calling it. 
  ///
  /// The last parameter of the function takes in an allocation callback that  
  /// you can provide. Once given, dr_wav will use your own allocater to allocate 
  /// any internal memory. For now, however, you can leave it as `nullptr`.

  drwav wav; 
  if(!drwav_init_file(&wav, "path/to/audio.wav", nullptr)) {
    printf("ERROR: Could not load WAV file!\n");
    return -1;
  }

  /// In order to read the PCM frames of a certain WAV file, you'll 
  /// need to use the function below, passing in a buffer of memory to 
  /// read the frames into as well as the total frames of the WAV file and 
  /// how many frames you want to read.
  ///
  /// Firstly, we allocate a buffer of samples big enough for the function to read 
  /// samples into. We determine the size of the buffer, by querying the `drwav` struct 
  /// we initialized above. 
  ///
  /// The `drwav_read_pcm_frames_s32` will write the requested samples count into the samples 
  /// buffer you just allocated. It will return the number of frames read. 
  ///
  /// This function is supposed to be used inside an audio callback. The buffer will have to 
  /// be allocated beforehand, of course, but the actual samples being read and the 
  /// amount of samples that were _actually_ read by the buffer, is a _very_ useful information 
  /// for the audio callback. 

  int* s32_samples_buffer = malloc(wav.totalPCMFrameCount * wav.channels * sizeof(int));
  size_t frames_read      = drwav_read_pcm_frames_s32(&wav, wav.totalPCMFrameCount, samples_buffer);

  /// Besides the `_s32` variant of the function, you can also decode the data in two other formats. 
  ///
  /// Mainly, `_f32` and `_s16`. The floating-point format is usually more "clean" and has a better quality. 
  /// However, as you would have guessed, it takes up more memory especially if the WAV file contains lots of samples.

  // short* s16_samples_buffer = malloc(wav.totalPCMFrameCount * wav.channels * sizeof(short));
  // size_t frames_read        = drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, samples_buffer);

  // float* f32_samples_buffer = malloc(wav.totalPCMFrameCount * wav.channels * sizeof(float));
  // size_t frames_read        = drwav_read_pcm_frames_f32(&wav, wav.totalPCMFrameCount, samples_buffer);

  /// When you're done with the your audio processing needs, make sure to de-initialize 
  /// the `drwav` file, passing in a pointer to the struct.
  ///
  /// We also have to deallocate the samples buffer we allocated 

  free(s32_samples_buffer);
  drwav_uninit(&wav);

  /// Opening and reading all the PCM frames of a WAV file at a given path.
  /// The function will also return the channels (mono or stereo), the sample rate, and the 
  /// total PCM frames that have been read.
  ///
  /// The function below will read the PCM frames as floating-point values. Even if the internal 
  /// WAV file is not represented as floating-point, dr_wav will make sure to convert it to floating-point.
  ///
  /// However, dr_wav also supports reading the PCM frames of a WAV file in a signed 2 byte format (or a `short`). 
  /// For that, you can use the `drwav_open_file_and_read_pcm_frames_s16` function, which accepts the same parameters.
  /// The only difference here is in the return value. Instead of a `float*` return value, the `drwav_open_file_and_read_pcm_frames_s16` 
  /// function will return, you guessed it, `short*`. 
  ///
  /// There is also the `drwav_open_file_and_read_pcm_frames_s32` variant which will read the PCM frames of a WAV 
  /// file and return an array of `int`s (`int*`).
  ///
  /// This kind of operation is very useful for when you just want to read the whole entire PCM frames in one 
  /// go and save it off to your own format, for example. Or, perhaps, you know that the WAV file is small 
  /// and you can read it in one go.
  ///
  /// And, much like the `drwav_init_file` function, the last parameter here is for the allocater.
  
  int channels, sample_rate;
  unsigned long total_frames;

  float* samples = drwav_open_file_and_read_pcm_frames_f32("path/to/audio.wav", &channels, &sample_rate, &total_frames, nullptr);
  if(!samples) {
    printf("ERROR: Could not load WAV file!\n");
    return -1;
  }

  /// Since the `samples` buffer is allocated by drwav, we need to call 
  /// this function below to deallocate the samples buffer.

  drwav_free(samples);
}
