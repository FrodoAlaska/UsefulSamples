#define DR_MP3_IMPLEMENTATION
#include "dr_mp3.h"

#include <cstdio>
#include <cstddef>

/// You can find the dr_mp3 library at the link below: 
///
/// https://github.com/mackron/dr_libs

int main() {
  /// In order to load a MP3 file using dr_mp3, you can call the function 
  /// below with the specific path to the MP3 file. The function will 
  /// fill a useful `drmp3` struct that can be used later for reading 
  /// PCM frames.
  ///
  /// The `drmp3` will include useful information about the MP3 file, such as 
  /// the channels, the bits per sample, the sample rate, and so on. This information 
  /// will be relevant later on when reading the PCM frames of the file.
  ///
  /// The function can return an error, however, so make sure to check 
  /// that when calling it. 
  ///
  /// The last parameter of the function takes in an allocation callback that  
  /// you can provide. Once given, dr_mp3 will use your own allocater to allocate 
  /// any internal memory. For now, however, you can leave it as `nullptr`.

  drmp3 mp3; 
  if(!drmp3_init_file(&mp3, "path/to/audio.mp3", nullptr)) {
    printf("ERROR: Could not load MP3 file!\n");
    return -1;
  }

  /// In order to read the PCM frames of a certain MP3 file, you'll 
  /// need to use the function below, passing in a buffer of memory to 
  /// read the frames into as well as the total frames of the MP3 file and 
  /// how many frames you want to read.
  ///
  /// Firstly, we allocate a buffer of samples big enough for the function to read 
  /// samples into. We determine the size of the buffer, by querying the `drmp3` struct 
  /// we initialized above. 
  ///
  /// The `drmp3_read_pcm_frames_s32` will write the requested samples count into the samples 
  /// buffer you just allocated. It will return the number of frames read. 
  ///
  /// This function is supposed to be used inside an audio callback. The buffer will have to 
  /// be allocated beforehand, of course, but the actual samples being read and the 
  /// amount of samples that were _actually_ read by the buffer, is a _very_ useful information 
  /// for the audio callback. 

  int* s32_samples_buffer = malloc(mp3.totalPCMFrameCount * mp3.channels * sizeof(int));
  size_t frames_read      = drmp3_read_pcm_frames_s32(&mp3, mp3.totalPCMFrameCount, samples_buffer);

  /// Besides the `_s32` variant of the function, you can also decode the data in two other formats. 
  ///
  /// Mainly, `_f32` and `_s16`. The floating-point format is usually more "clean" and has a better quality. 
  /// However, as you would have guessed, it takes up more memory especially if the MP3 file contains lots of samples.

  // short* s16_samples_buffer = malloc(mp3.totalPCMFrameCount * mp3.channels * sizeof(short));
  // size_t frames_read        = drmp3_read_pcm_frames_s16(&mp3, mp3.totalPCMFrameCount, samples_buffer);

  // float* f32_samples_buffer = malloc(mp3.totalPCMFrameCount * mp3.channels * sizeof(float));
  // size_t frames_read        = drmp3_read_pcm_frames_f32(&mp3, mp3.totalPCMFrameCount, samples_buffer);

  /// When you're done with the your audio processing needs, make sure to de-initialize 
  /// the `drmp3` file, passing in a pointer to the struct.
  ///
  /// We also have to deallocate the samples buffer we allocated 

  free(s32_samples_buffer);
  drmp3_uninit(&mp3);

  /// The funtion below will open and read all the PCM frames of an MP3 file at a given path.
  /// The function will also write out to a `drmp3_config` struct that will include both the 
  /// channels and the sample rate of the buffer that was read. Along with that, the function 
  /// will also return the total amount of frames that were read.
  ///
  /// The function below will read the PCM frames as floating-point values. Even if the internal 
  /// MP3 file is not represented as floating-point, dr_mp3 will make sure to convert it to floating-point.
  ///
  /// However, dr_mp3 also supports reading the PCM frames of a MP3 file in a signed 2 byte format (or a `short`). 
  /// For that, you can use the `drmp3_open_file_and_read_pcm_frames_s16` function, which accepts the same parameters.
  /// The only difference here is in the return value. Instead of a `float*` return value, the `drmp3_open_file_and_read_pcm_frames_s16` 
  /// function will return, you guessed it, `short*`. 
  ///
  /// Sadly, as far as I know, dr_mp3 does not support doing the same operation with `int`s. I'm not sure why.
  ///
  /// This kind of operation is very useful for when you just want to read the whole entire PCM frames in one 
  /// go and save it off to your own format, for example. Or, perhaps, you know that the MP3 file is small 
  /// and you can read it in one go.
  ///
  /// And, much like the `drmp3_init_file` function, the last parameter here is for the allocater.
 
  drmp3_config mp3_config;
  unsigned long total_frames;

  float* samples = drmp3_open_file_and_read_pcm_frames_f32("path/to/audio.mp3", &mp3_config, &total_frames, nullptr);
  if(!samples) {
    printf("ERROR: Could not load MP3 file!\n");
    return -1;
  }

  /// Since the `samples` buffer is allocated by dr_mp3, we need to call 
  /// this function below to deallocate the samples buffer.

  drmp3_free(samples);
}
