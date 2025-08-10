#include "stb_image.h"

#include <cstdio>

int main() {
  /// This function, as the name implies, will load in the pixels of an image 
  /// given the file name. It also has a few out parameters. 
  ///
  /// Mainly, the width, height, and channels of the loaded image. 
  /// As for the last parameter, it will be a hint given to the function to 
  /// force the function to order the pixels to be `n` channels. 
  ///
  /// For example, say we want an image that will have RGBA format. So, that means, 
  /// we want the image to have 4 channels per pixel. STB will make sure that the 
  /// returned `pixels` array has 4 components/channels per pixels. And, because of that, 
  /// the returned `channels` parameter will be `4`. 
  ///
  /// However, if you do not care about that, you can set the final parameter to 0. 
  /// This will return the number of channels this image had when the image was exported.

  int width, height, channels;
  unsigned char* pixels = stbi_load("path/to/texture.png", &width, &height, &channels, 4);

  /// The `pixels` pointer STB returns will be a valid address only when the data was 
  /// loaded successfully. If otherwise, STB will return a `NULL` (or `nullptr`). In which 
  /// case, we need to print an error of some kind. STB does provide the `stbi_failure_reason` 
  /// function which returns a string (const char*) explaining the exact reason of the failure. 
  /// It's in the name. 

  if(!pixels) {
    printf("Failed to load image. REASON: %s\n", stbi_failure_reason());
    return -1;
  }

  /// If you want to just query the image for some relevant information but you 
  /// don't want to load the actual data, STB provides a function exactly for that. 
  ///
  /// It will return the width, height, and channels of an image at a given path. 
  /// The function will also return an error code. If the error code is `1`, then 
  /// the function was successful and the variables are set. Otherwise, there is 
  /// something wrong that happened and you should print an error.

  int err = stbi_info("path/to/texture.png", &width, &height, &channels);
  
  if(err != 1) {
    printf("Error with stbi_info\n");
    return -1;
  }

  /// Besides the regular LDR (low dynamic range) images, STB additionally supports 
  /// HDR (high dynamic range) images. However, unlike LDR images, the function for 
  /// HDR images returns an array of `float`s. 
  ///
  /// However, prior to loading an HDR image, we might need to configure a few constants STB sets.
  /// Mainly, those configurations are the GAMMA and the scale of the HDR image. 
  ///
  /// Initially, these constants are set as `2.2f` for the GAMMA and `1.0f` for the scale
  
  stbi_hdr_to_ldr_gamma(2.2f);
  stbi_hdr_to_ldr_scale(1.0f);

  /// Alternatively, we can load _any_ image--whether it is an HDR or not--as an HDR image and 
  /// then apply the _inverse_ of the constants to map back into LDR.
  
  stbi_ldr_to_hdr_scale(1.0f);
  stbi_ldr_to_hdr_gamma(2.2f);

  // Very similiar to `stbi_load` but returns an array of `float`s.
  int hdr_width, hdr_height, hdr_channels;
  float* hdr_pixels = stbi_loadf("path/to/hdr_texture.hdr", &hdr_width, &hdr_height, &hdr_channels, 0);

  // Always remember to check for errors...
  if(!hdr_pixels) {
    printf("Failed to load HDR image. REASON: %s\n", stbi_failure_reason());
    return -1;
  }

  /// We can also find out if an image at a given path is an HDR image or not.
  /// It will return `1` if the image is an HDR, and `0` otherwise.
  
  int is_hdr = stbi_is_hdr("path/to/hdr_texture.hdr");
  
  if(is_hdr == 1) {
    printf("HDR yay!\n");
  }

  /// Finally, when we are done with the data, we MUST de-allocate the pixels 
  /// array STB gave us. It's better to call this function than to directly 
  /// call `free(pixels)` ourselves, since STB does do some additional cleanup.
  ///
  /// However, if you do wish to insert your own allocation and de-allocation functionality, 
  /// you can do so by `#define` the STBI_MALLOC(size), STBI_REALLOC(ptr, new_size), and STBI_FREE(ptr) 
  /// before the `#include stb_image.h` line.

  stbi_image_free(pixels);
  stbi_image_free(hdr_pixels);
}
