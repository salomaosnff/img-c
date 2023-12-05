#ifndef _IMAGE_HPP
#define _IMAGE_HPP

#include <cstdint>

#include <vector>


namespace Image {
  typedef enum {
    RGB, RGBA, Grayscale, Undefined
  } PixelFormat;

  struct StaticImage {
    uint32_t width;
    uint32_t height;

    PixelFormat format;

    uint8_t*data;
  };

  struct AnimationFrame {
    int timestamp;
    unsigned char *data;
  };

  struct AnimatedImage {
    uint32_t width;
    uint32_t height;

    PixelFormat format;

    std::vector<AnimationFrame> frames;
  };

  StaticImage random_image(uint32_t, uint32_t, PixelFormat);
}

#endif