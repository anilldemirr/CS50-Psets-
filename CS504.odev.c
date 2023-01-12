#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"

int main(int argc, char *argv[])
{

    // İzin verilen filtreleri tanımlayın
    char *filters = "begr";

    // Filtre flagini al ve gecerliligini kontrol et
    char filter = getopt(argc, argv, filters);
    if (filter == '?')
    {
        fprintf(stderr, "Invalid filter.\n");
        return 1;
    }

    // Bir filtre oldugundan emin ol
    if (getopt(argc, argv, filters) != -1)
    {
        fprintf(stderr, "Only one filter allowed.\n");
        return 2;
    }

    // Dogru kullanım oldugundan emin ol
    if (argc != optind + 2)
    {
        fprintf(stderr, "Usage: filter [flag] infile outfile\n");
        return 3;
    }

    // Dosya isimlerini hatırla
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    // Girdi dosyasini ac
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 4;
    }

    // Cikti dosyasini ac
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 5;
    }

    // Dosyanın BITMAPFILEHEADER'ını oku
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Dosyanın BITMAPINFOHEADER'ını oku
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Dosyanın (muhtemelen) 24 bit sıkıştırılmamış bir BMP 4.0 olduğundan emin olun
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 6;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // Görsel icin hafizayi tahsis et
    RGBTRIPLE(*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }

    // Tarama cizgileri icin padding belirle
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Dosyanın tarama çizgileri üzerinde yineleme yapın
    for (int i = 0; i < height; i++)
    {
        // Satırı piksel dizisine oku
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // Paddingi atla
        fseek(inptr, padding, SEEK_CUR);
    }

    // Resmi filtrele
    switch (filter)
    {
        // Bulanıklık
        case 'b':
            blur(height, width, image);
            break;

        // Kenarlar
        case 'e':
            edges(height, width, image);
            break;

        // Gri Tonlama
        case 'g':
            grayscale(height, width, image);
            break;

        // Yansıt
        case 'r':
            reflect(height, width, image);
            break;
    }

    // Dosyanın BITMAPFILEHEADER'ını yaz
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Dosyanın BITMAPINFOHEADER'ını yaz
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Outfile'a bir piksel yaz
    for (int i = 0; i < height; i++)
    {
        // Outfile'a satir yaz
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // Satir sonuna padding yaz
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // Resim icin bos hafiza
    free(image);

    // İnfile'i kapa
    fclose(inptr);

    // Ourfile'i kapa
    fclose(outptr);

    return 0;
}
