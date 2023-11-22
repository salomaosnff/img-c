#include <emscripten.h>
#include <emscripten/bind.h>
#include <stdint.h>
#include <vector>
#include <webp/encode.h>
#include <webp/types.h>
#include "image.cpp"
#include "webp.cpp"

using namespace emscripten;

const size_t width = 600;
const size_t height = 600;
const size_t channels = 4;

std::vector<uint8_t> generate()
{
    return encode_static(random_image(width, height, PixelFormat::Grayscale));
}

EMSCRIPTEN_BINDINGS(my_struct)
{
    register_vector<unsigned char>("EncodedImage");

    function("generate", &generate);
}