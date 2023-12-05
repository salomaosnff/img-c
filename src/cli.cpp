#include "image.hpp"
#include "png.hpp"
#include "webp.hpp"


int main() {
  Image::StaticImage image = PNG::decode("input.png");
  WebP::encode_static(image);

  return 0;
}