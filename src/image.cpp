#include "image.hpp"

#include <cstdlib>
#include <ctime>

using namespace Image;

StaticImage random_image(uint32_t width, uint32_t height, PixelFormat format)
{
  StaticImage image;

  image.width = width;
  image.height = height;
  image.format = format;

  size_t size = width * height * (format == PixelFormat::RGB ? 3 : 4);

  image.data = new unsigned char[size];

  for (int i = 0; i < size; i++)
  {
    srand(time(NULL) + i);
    image.data[i] = rand() % 256;
  }

  return image;
}