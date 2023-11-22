#include <libpng/png.h>
#include <fstream>

#include <cstdint>
#include <cstdio>

#include "image.hpp"

#include <stdio.h>
#include <png.h>

int ReadPNG(Image::StaticImage &image, const char *filename)
{
  // Abre a imagem PNG
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    fprintf(stderr, "Erro ao abrir a imagem\n");
    return 1;
  }

  // Inicializa a biblioteca PNG
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL)
  {
    fprintf(stderr, "Erro ao inicializar a biblioteca PNG\n");
    return 1;
  }

  // Inicializa o info da imagem PNG
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (info_ptr == NULL)
  {
    png_destroy_read_struct(&png_ptr, NULL, NULL);
    fprintf(stderr, "Erro ao inicializar o info da imagem PNG\n");
    return 1;
  }

  // Associa o info da imagem PNG à biblioteca PNG
  png_init_io(png_ptr, fp);

  // Lê o cabeçalho da imagem PNG
  png_read_info(png_ptr, info_ptr);

  image.format = [&](void) -> Image::PixelFormat
  {
    switch (png_color_type)
    {
    case PNG_COLOR_TYPE_GRAY:
      puts("gray");
      return Image::PixelFormat::Grayscale;

    case PNG_COLOR_TYPE_RGB:
      puts("rgb");
      return Image::PixelFormat::RGB;

    case PNG_COLOR_TYPE_RGBA:
      puts("rgba");
      return Image::PixelFormat::RGBA;
    }

    return Image::PixelFormat::Undefined;
  }();

  // Obtém as dimensões da imagem
  int width = png_get_image_width(png_ptr, info_ptr);
  int height = png_get_image_height(png_ptr, info_ptr);

  // Obtém o número de canais da imagem
  int channels = png_get_channels(png_ptr, info_ptr);

  // Aloca um buffer para armazenar os pixels da imagem
  unsigned char *buffer = (unsigned char *)malloc(width * height * channels);

  // Lê os pixels da imagem
  for (int i = 0; i < height; i++)
  {
    png_read_row(png_ptr, buffer + i * width * channels, NULL);
  }

  // Fecha a imagem PNG
  png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

  // Imprime os pixels da imagem
  for (int i = 0; i < height; i++)
  {
    for (int j = 0; j < width; j++)
    {
      for (int k = 0; k < channels; k++)
      {
        printf("%02X ", buffer[i * width * channels + j * channels + k]);
      }
    }
    printf("\n");
  }

  // Libera o buffer
  free(buffer);

  // Fecha o arquivo
  fclose(fp);

  return 0;
}

int main()
{

  ReadPNG("input.png");
  /*
    Image::StaticImage image;
    FILE *png_file = NULL;

    int bits, png_color_type;

    if (!(png_file = fopen("input.png", "rb")))
    {
      printf("Error: could not open input file\n");
      return 1;
    }

    png_byte pbSig[8];
    fread(pbSig, 1, 8, png_file);
    if (!png_check_sig(pbSig, 8))
    {
      fclose(png_file);
      printf("ReadFilePNG: invalid PNG signature\r\n");
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop png_info = png_create_info_struct(png_ptr);

    png_init_io(png_ptr, png_file);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, png_info);

    png_get_IHDR(png_ptr, png_info, &image.width, &image.height, &bits, &png_color_type, NULL, NULL, NULL);

    printf(" height: %u\n"
           " width: %u\n",
           image.height, image.width);

    auto nbands = (int)png_get_channels(png_ptr, png_info);

    if (bits == 16)
      png_set_strip_16(png_ptr);

    // change palette color into RGB
    if (png_color_type == PNG_COLOR_TYPE_PALETTE)
      png_set_expand(png_ptr);

    // want at least 8 bits
    if (bits < 8)
      png_set_expand(png_ptr);

    // if there is a transparent palette entry, create alpha channel
    if (png_get_valid(png_ptr, png_info, PNG_INFO_tRNS))
      png_set_expand(png_ptr);

    // make gray images with alpha channel into RGBA -- TODO: or just ignore alpha?
    if (png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      // colorType == PNG_COLOR_TYPE_GRAY       // but leave gray images alone
      png_set_gray_to_rgb(png_ptr);

    // set the background color to draw transparent and alpha images over.
    // only needed for gray images with alpha
    if (png_color_type == PNG_COLOR_TYPE_GRAY_ALPHA ||
        png_color_type == PNG_COLOR_TYPE_GRAY)
    {
      png_color_16 *pBackground;
      if (png_get_bKGD(png_ptr, png_info, &pBackground))
        png_set_background(png_ptr, pBackground, PNG_BACKGROUND_GAMMA_FILE, 1, 1.0);
    }

    png_set_bgr(png_ptr);

    // always convert 3-band to 4-band images (add alpha):
    if (png_color_type == PNG_COLOR_TYPE_RGB)
      png_set_add_alpha(png_ptr, 255, PNG_FILLER_AFTER);

    png_read_update_info(png_ptr, png_info);

    png_get_IHDR(png_ptr, png_info, &image.width, &image.height, &bits, &png_color_type, NULL, NULL, NULL);

    nbands = (int)png_get_channels(png_ptr, png_info);

    image.format = [&](void) -> Image::PixelFormat
    {
      switch (png_color_type)
      {
      case PNG_COLOR_TYPE_GRAY:
        puts("gray");
        return Image::PixelFormat::Grayscale;

      case PNG_COLOR_TYPE_RGB:
        puts("rgb");
        return Image::PixelFormat::RGB;

      case PNG_COLOR_TYPE_RGBA:
        puts("rgba");
        return Image::PixelFormat::RGBA;
      }

      return Image::PixelFormat::Undefined;
    }();

    image.data = new uint8_t[image.width * image.height * 3];
    std::vector<uint8_t *> v;
    v.resize(image.height);

    png_read_image(png_ptr, &v[0]);

    for (uint32_t i = 0; i < 10 * 3; i++)
    {
      printf(" %02X", v[0]), fflush(stdout);
    }

    png_destroy_read_struct(&png_ptr, &png_info, NULL);
    fclose(png_file);
  */
  return 0;
}