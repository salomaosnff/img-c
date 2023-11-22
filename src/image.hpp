#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include <cstddef>
#include <vector>

namespace Image
{
  enum PixelFormat
  {
    RGB,
    RGBA,
    Grayscale,
    Undefined
  };

  struct StaticImage
  {
    uint32_t width;
    uint32_t height;

    PixelFormat format;

    unsigned char *data;
  };

  struct AnimationFrame
  {
    int timestamp;
    unsigned char *data;
  };

  struct AnimatedImage
  {
    size_t width;
    size_t height;

    PixelFormat format;

    std::vector<AnimationFrame> frames;
  };

  StaticImage random_image(uint32_t, uint32_t, PixelFormat);
}

#endif