#include "bmp.h"

static int32_t char_to_int32_t( const char* src )
{
    int32_t number = 0, start = 0;

    if (src[0] == '-')
    {
        printf("Size can not be less then 0!\n");
        exit(1);
    }
    while (src[start] != 0)
        number = number * 10 + (src[start++] - '0');
    return number;
}

int main( int argc, char* argv[] )
{
    if (argc != 8 || strcmp(argv[1], "crop-rotate") != 0)
    {
        printf("Incorrect program call\n");
        exit(1);
    }
    image* img = malloc(sizeof(image));

    load_bmp(argv[2], img);
    const int32_t x = char_to_int32_t(argv[4]);
    const int32_t y = char_to_int32_t(argv[5]);
    const int32_t w = char_to_int32_t(argv[6]);
    const int32_t h = char_to_int32_t(argv[7]);

    assert(!(x + w > img->header.width
          || y + h > img->header.height));

    crop(img, x, y, w, h);
    rotate(img);
    save_bmp(argv[3], img);
    free(img->pixels);
    free(img);
    return 0;
}