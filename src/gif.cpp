#include "gif.hpp"

#include <gif_lib.h>

#include <cstdint>
#include <cstdlib>

#include <cstdio>


Image::AnimatedImage GIF::decode(const char *filename) {
  Image::AnimatedImage image;

  GifFileType *gif = nullptr;
  int channels, error;

  uint8_t *buffer = nullptr;

  if (!(gif = DGifOpenFileName(filename, &error))) {
    goto _Return;
  }

  image.width = gif->SWidth;
  image.height = gif->SHeight;

  channels = gif->Image.ColorMap ? gif->Image.ColorMap->ColorCount : 256;

  
  if (!(buffer = (uint8_t *)malloc(image.width*image.height*channels))) {
    goto _Return;
  }

  for (int i = 0; i < image.height; i++) {
    DGifGetLine(gif, buffer + i * image.width * channels, image.width * channels);
  }

_Return:
  DGifCloseFile(gif, &error);

  return image;
}
