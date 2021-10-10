#ifndef BMP_H_
#define BMP_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#pragma pack(push, 2)
typedef struct BmpHeader
{
    unsigned char b1;
    unsigned char b2;
    int32_t size;
    int16_t reserve1;
    int16_t reserve2;
    int32_t position;
    int32_t move;
    int32_t width;
    int32_t height;
    int16_t colorPlanes;
    int16_t bitPixel;
    int32_t compressMethod;
    int32_t massLength;
    int32_t massWidth;
    int32_t massHeight;
    int32_t colorNumber;
    int32_t generalColorNumber;
} BmpHeader;
#pragma pack(pop)

typedef struct image
{
    BmpHeader header;
    int** pixels;
} image;

int load_bmp( const char* filename, image* img );
int save_bmp( const char* filename, const image* img );
void rotate( image* img );
void crop( image* img, const int32_t newX, const int32_t newY, const int32_t newW, const int32_t newH );

#endif