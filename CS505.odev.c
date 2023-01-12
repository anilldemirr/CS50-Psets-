//SPELLER.C

// Speller'in edevatlari

#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "dictionary.h"

// Herhangi bir tanımın tanımını kaldırın
#undef calculate
#undef getrusage

// Varsayilan sozluk
#define DICTIONARY "dictionaries/large"

// Prototip
double calculate(const struct rusage *b, const struct rusage *a);

int main(int argc, char *argv[])
{
    // Arg'ın dogru numarasi icin kontrol et
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./speller [DICTIONARY] text\n");
        return 1;
    }

    // Zamanlama verileri için yapılar
    struct rusage before, after;

    // Benchmarklar
    double time_load = 0.0, time_check = 0.0, time_size = 0.0, time_unload = 0.0;

    // Kullanmak icin sozluk tanimla
    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    // Sozluk yukle
    getrusage(RUSAGE_SELF, &before);
    bool loaded = load(dictionary);
    getrusage(RUSAGE_SELF, &after);

    // Sozluk yuklenmediyse cik
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }

    // Sozlugu yuklemek icin zamani hesapla
    time_load = calculate(&before, &after);

    // Text'i acmayi dene
    char *text = (argc == 3) ? argv[2] : argv[1];
    FILE *file = fopen(text, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    // Yanlis heceleri raporlamaya hazirlan
    printf("\nMISSPELLED WORDS\n\n");

    // Heceleri kontrol etmeye hazirlan
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH + 1];

    // Text'deki her hecenin kontrolu
    for (int c = fgetc(file); c != EOF; c = fgetc(file))
    {
        // Sadece kesme isareti ve alfebetik karakterlere izin ver
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Kelimeye karakter ilave et
            word[index] = c;
            index++;

            // Kelime olamayacak kadar uzun stringleri yok say
            if (index > LENGTH)
            {
                // Alfabetik dizinin geri kalanını tüket
                while ((c = fgetc(file)) != EOF && isalpha(c));

                // Yeni kelime icin hazirlan
                index = 0;
            }
        }

        // Sayi iceren kelimeleri yok say
        else if (isdigit(c))
        {
            // Alfasayısal dizenin geri kalanını tüket
            while ((c = fgetc(file)) != EOF && isalnum(c));

            // Yeni kelime icin hazirlan
            index = 0;
        }

        // Butun bir kelime bulmus olmalisin
        else if (index > 0)
        {
            // Gecerli kelimeyi sonlandir
            word[index] = '\0';

            // Sayaci güncelle
            words++;

            // Kelimelerin hecelenmesini kontrol et
            getrusage(RUSAGE_SELF, &before);
            bool misspelled = !check(word);
            getrusage(RUSAGE_SELF, &after);

            // Benchmark'i güncelle
            time_check += calculate(&before, &after);

            // Eger yanlis hecelenmisse kelimeyi ekrana yansit
            if (misspelled)
            {
                printf("%s\n", word);
                misspellings++;
            }

            // Yeni kelime icin hazirlan 
            index = 0;
        }
    }

    // Bir hata olup olmadığını kontrol edin
    if (ferror(file))
    {
        fclose(file);
        printf("Error reading %s.\n", text);
        unload();
        return 1;
    }

    // Texti kapat
    fclose(file);

    // Sozlugun buyuklugunu belirle
    getrusage(RUSAGE_SELF, &before);
    unsigned int n = size();
    getrusage(RUSAGE_SELF, &after);

    // Sözlüğün buyuklugunu belirlemek için zamanı hesaplayın
    time_size = calculate(&before, &after);

    // Sozlugun yuklemesini kaldir
    getrusage(RUSAGE_SELF, &before);
    bool unloaded = unload();
    getrusage(RUSAGE_SELF, &after);

    // Sözlük yüklenmemişse iptal et
    if (!unloaded)
    {
        printf("Could not unload %s.\n", dictionary);
        return 1;
    }

    // Sozlugun yuklemesini kaldirmak icin zamani hesapla
    time_unload = calculate(&before, &after);

    // Benchmarklari raporla
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", time_load);
    printf("TIME IN check:        %.2f\n", time_check);
    printf("TIME IN size:         %.2f\n", time_size);
    printf("TIME IN unload:       %.2f\n", time_unload);
    printf("TIME IN TOTAL:        %.2f\n\n",
           time_load + time_check + time_size + time_unload);

    // Basari
    return 0;
}

// b ve a arasındaki saniye sayısını döndürür
double calculate(const struct rusage *b, const struct rusage *a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}
