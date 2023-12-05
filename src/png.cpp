#include "png.hpp"

#include <libpng/png.h>

#include <cstdint>
#include <cstdlib>

#include <cstdio>

#include <string>


Image::StaticImage PNG::decode(const char *filename) {
  Image::StaticImage image;

  png_structp png_ptr = nullptr;
  png_infop png_info = nullptr;

  FILE *png_file = nullptr;

  int png_color_type;
  png_byte channels;

  uint8_t *buffer = nullptr;

  if (!(png_file = fopen(filename, "rb"))) {
  // throw PNG::Exceptipn("Erro ao abrir a imagem");
    goto _Return;
  }

  if (!(png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL))) {
  // throw PNG::Exceptipn("Erro ao inicializar a biblioteca PNG");
    goto _Return;
  }

  if (!(png_info = png_create_info_struct(png_ptr))) {
  // throw PNG::Exceptipn("Erro ao inicializar o info da imagem PNG");
    goto _Return;
  }

  png_init_io(png_ptr, png_file);
  png_read_info(png_ptr, png_info);

  png_get_IHDR(png_ptr, png_info, &image.width, &image.height, NULL, &png_color_type, NULL, NULL, NULL);

  image.format = [&](void) -> Image::PixelFormat {
    switch (png_color_type) {
      case PNG_COLOR_TYPE_GRAY:
        return Image::PixelFormat::Grayscale;

      case PNG_COLOR_TYPE_RGB:
        return Image::PixelFormat::RGB;

      case PNG_COLOR_TYPE_RGBA:
        return Image::PixelFormat::RGBA;
    }

    return Image::PixelFormat::Undefined;
  }();

  channels = png_get_channels(png_ptr, png_info);

  if (!(buffer = (uint8_t *)malloc(image.width * image.height * channels))) {
    goto _Return;
  }

  for (png_uint_32 byte = 0; byte < image.height; byte++) {
    png_read_row(png_ptr, buffer + byte * image.width * channels, NULL);
  }

  image.data = buffer;

_Return:
  png_destroy_read_struct(&png_ptr, &png_info, NULL);
//png_destroy_info_struct(png_ptr, &png_info);

  fclose(png_file);

  return image;
}
