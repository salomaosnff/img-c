#include "webp.hpp"

#include <webp/encode.h>
#include <webp/types.h>

#include <vector>

#include "image.hpp"


#define WEBP_IMAGE_QUALITY 25


std::vector<uint8_t> WebP::encode_static(Image::StaticImage image) {
  uint8_t*data = nullptr;
  size_t size = 0;

  if (image.format == Image::PixelFormat::RGB) {
    size = WebPEncodeRGB(image.data, image.width, image.height, image.width*3, WEBP_IMAGE_QUALITY, &data);
  } else if (image.format == Image::PixelFormat::RGBA) {
    size = WebPEncodeRGBA(image.data, image.width, image.height, image.width*4, WEBP_IMAGE_QUALITY, &data);
  } else if (image.format == Image::PixelFormat::Grayscale) {
    size = WebPEncodeLosslessRGB(image.data, image.width, image.height, image.width*1, &data);
  }

  return std::vector<uint8_t>(data, data+size);
}