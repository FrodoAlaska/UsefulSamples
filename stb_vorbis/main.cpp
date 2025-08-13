#define STB_VORBIS_IMPLEMENTATION
#include "stb_vorbis.h"

#include <cstdio>
#include <cstdlib>

/// You can find the stb_vorbis library at the link below: 
///
/// https://github.com/nothings/stb

int main() {
  /// In order to use stb_vorbis, you'll need to call the function below to 
  /// load an `stb_vorbis` struct from a filename. The function expects a 
  /// a string specyfing the file name as well as a pointer to an `int`, 
  /// where it will write the error code if there's any. And, finally, an allocater 
  /// which we can leave as `nullptr` for now. 
  ///
  /// If stb_vorbis could not load the file specified, the returned `stb_vorbis` struct 
  /// will be returned as a `nullptr`. We can check the error code against the various 
  /// values in the `STBVorbisError` enum for the exact reason for the failure. 

  int error_code = 0;

  stb_vorbis* vorbis = stb_vorbis_open_filename("path/to/audio.ogg", &error_code, nullptr);
  if(!vorbis) {
    printf("Failed to load OGG file! %i\n", error_code); // We can just print the error code for now...
    return -1;
  }

  /// With the `stb_vorbis` struct now in our possession, we can use it for various tasks. 
  /// Firstly, we can use it to get useful information about the file. Mainly, we can 
  /// use it to get the `stb_vorbis_info` struct. This struct includes a lot of memebers, 
  /// but we only care about three: 
  ///
  ///   - stb_vorbis_info.channels
  ///   - stb_vorbis_info.sample_rate
  ///   - stb_vorbis_info.max_frame_size
  ///
  /// We can use that information to allocate a buffer of our own so that stb_vorbis can write 
  /// into it. 

  stb_vorbis_info vorbis_info = stb_vorbis_get_info(vorbis);

  /// Besides getting the information about the OGG file, we can also carry out 
  /// some useful operation on the file. 
  ///
  /// We can, for example, seek to the beginning of the file. 
  ///
  /// By the way, these `_seek` functions will return either a `1` if 
  /// the seeking operation was successful, and `0` otherwise. This 
  /// can be useful if don't want to seek past the end of the file, for example.
  ///
  /// I won't include the return value in each example, but just keep it in your mind.

  int ret = stb_vorbis_seek_start(vorbis);
  if(ret != 1) {
    printf("Cannot seek there for some reason...\n");
    return -1;
  }

  /// We can also seek to a specific frame and/or sample 
  ///
  /// @NOTE: The difference between a "frame" and a "sample" is that 
  /// a "sample" refers to a _single_ value in the audio file, while a "frame" is 
  /// a either a pair of samples if the audio file has two channels (stereo) or a 
  /// single sample if the file has only one channel (mono).
  
  stb_vorbis_seek_frame(vorbis, 420); // Seek to frame `420`
  stb_vorbis_seek(vorbis, 69);        // Seek to sample `69` 

  /// 
  /// @TODO (stb_vorbis): Write about the `stb_vorbis_get_frames_short` and `stb_vorbis_get_frame_short_interleaved` functions 
  ///

  /// We can open an OGG file, decode all of its data, and save it into a samples buffer. 
  /// This function is massively useful for when you just want all the data in the OGG file 
  /// to write it to your own format and then load it from elsewhere. 
  ///
  /// The function expects the name of the OGG file, a pointer to an `int` where it can 
  /// write the number of channels found in the OGG file, a pointer to an `int` where 
  /// it can write the samples rate found in the OGG file, and a pointer to a samples buffer. 
  ///
  /// The function will return the number of frames that were decoded. If there was an error 
  /// that occured during the decoding operation, the function will return with the value `-1`, 
  /// indicating failure. 

  int channels, sample_rate;
  short* samples;

  int frames = stb_vorbis_decode_filename("path/to/audio.ogg", &channels, &sample_rate, &samples);
  if(frames == -1) {
    printf("Failed to load OGG file!\n");
    return -1;
  } 

  /// Since stb_vorbis will allocate the samples buffer internally, we'll have to free it here. 

  free(samples);

  /// Always remember to de-initialize stb_vorbis when you're done with it. 
  
  stb_vorbis_close(vorbis);
}
