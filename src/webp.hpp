#ifndef _WEBP_HPP
#define _WEBP_HPP

#include <cstdint>
#include <vector>

#include "image.hpp"


namespace WebP {
  std::vector<uint8_t> encode_static(Image::StaticImage);
}

#endif