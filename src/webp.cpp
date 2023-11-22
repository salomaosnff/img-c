#include <webp/encode.h>
#include <webp/types.h>

#include "image.hpp"

#include <vector>

const uint8_t QUALITY = 75;

std::vector<unsigned char> encode_static(Image::StaticImage image)
{
  unsigned char *data = nullptr;
  size_t size = 0;

  if (image.format == Image::PixelFormat::RGB)
  {
    size = WebPEncodeRGB(image.data, image.width, image.height, image.width * 3, QUALITY, &data);
  }

  else if (image.format == Image::PixelFormat::RGBA)
  {
    size = WebPEncodeRGBA(image.data, image.width, image.height, image.width * 4, QUALITY, &data);
  }
  else if (image.format == Image::PixelFormat::Grayscale)
  {
    size = WebPEncodeLosslessRGB(image.data, image.width, image.height, image.width * 1, &data);
  }

  return std::vector<unsigned char>(data, data + size);
}