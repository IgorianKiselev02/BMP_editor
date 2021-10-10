#include "bmp.h"

#define charmove 256

int load_bmp(const char* filename, image* img)
{
    int32_t i, j, k;
    FILE* F;

    if ((F = fopen(filename, "rb")) == NULL)
    {
        printf("Failed to open file for reading\n");
        exit(1);
    }

    fread(&img->header, sizeof(img->header), 1, F);
    if (img->header.b1 != 'B' || img->header.b2 != 'M' || img->header.bitPixel != 24)
    {
        printf("Wnong file type or corrupted file...\n");
        exit(1);
    }

    const int32_t size = img->header.height * sizeof(int*) + img->header.width * sizeof(int) * img->header.height;
    int* arr;

    img->pixels = malloc(size);
    arr = (int*)(img->pixels + img->header.height);
    for (i = 0; i < img->header.height; i++)
    {
        img->pixels[i] = (arr + img->header.width * i);
        for (j = 0; j < img->header.width; j++)
        {
            char R, G, B;
            fread(&R, sizeof(char), 1, F);
            fread(&G, sizeof(char), 1, F);
            fread(&B, sizeof(char), 1, F);
            img->pixels[i][j] = ((R * charmove) + G) * charmove + B;
        }
        if (img->header.width % 4 != 0)
            for (k = 0; k < img->header.width % 4; k++)
            {
                char Extra;
                fread(&Extra, sizeof(char), 1, F);
            }
    }

    fclose(F);
    return 0;
}

int save_bmp(const char* filename, const image* img)
{
    int32_t i, j, k;
    const int32_t emptyBytes = img->header.width % 4;
    const char stringEndDigit = '\0';
    FILE* F;

    if ((F = fopen(filename, "wb")) == NULL)
    {
        printf("Failed to open file for saving\n");
        return 1;
    }

    fwrite(&img->header, sizeof(img->header), 1, F);
    for (i = 0; i < img->header.height; i++)
    {
        for (j = 0; j < img->header.width; j++)
        {
            const char B = img->pixels[i][j] % charmove;
            const char G = (img->pixels[i][j] - B) / charmove % charmove;
            const char R = ((img->pixels[i][j] - B) / charmove - G) / charmove;
            fwrite(&R, sizeof(char), 1, F);
            fwrite(&G, sizeof(char), 1, F);
            fwrite(&B, sizeof(char), 1, F);
        }
        for (k = 0; k < emptyBytes; k++)
            fwrite(&stringEndDigit, sizeof(char), 1, F);
    }

    fclose(F);
    return 0;
}

void rotate(image* img)
{
    const int32_t size = img->header.width * sizeof(int*) + img->header.height * sizeof(int) * img->header.width;
    int** tmp = malloc(size);
    int32_t i, j;
    int* arr;

    arr = (int*)(tmp + img->header.width);
    for (i = 0; i < img->header.width; i++)
        tmp[i] = (arr + img->header.height * i);

    for (i = 0; i < img->header.height; i++)
        for (j = 0; j < img->header.width; j++)
            tmp[img->header.width - j - 1][img->header.height - i - 1] = img->pixels[img->header.height - i - 1][j];

    free(img->pixels);
    img->pixels = tmp;
    i = img->header.width;
    img->header.width = img->header.height;
    img->header.height = i;
}

void crop(image* img, const int32_t newX, const int32_t newY, const int32_t newW, const int32_t newH)
{
    const int32_t size = newH * sizeof(int*) + newW * sizeof(int) * newH;
    int** tmp = malloc(size);
    int32_t i, j;
    int* arr;

    arr = (int*)(tmp + newH);
    for (i = 0; i < newH; i++)
        tmp[i] = (arr + newW * i);

    for (i = 0; i < newH; i++)
        for (j = 0; j < newW; j++)
            tmp[i][j] = img->pixels[img->header.height - newY - newH + i][newX + j];

    free(img->pixels);
    img->pixels = tmp;
    img->header.size = img->header.size - 3 * img->header.width * img->header.height + 3 * newH * newW;
    img->header.width = newW;
    img->header.height = newH;
}