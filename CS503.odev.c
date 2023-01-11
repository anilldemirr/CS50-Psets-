#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Maksimum oy kullanan ve aday sayisi
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] i'inci oyun j'inci tercihi
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Adaylar isim, oy sayisi, eleminasyon durumuna sahip
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Adaylarin dizisi
candidate candidates[MAX_CANDIDATES];

// Adaylarin ve oy kullananların sayisi
int voter_count;
int candidate_count;

// Fonksiyon prototipleri
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Gecersiz kullanim kontrolu
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Adaylarin doluluk dizisi
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Oylar icin sorgulamaya devam et
    for (int i = 0; i < voter_count; i++)
    {

        // Her rank icin sorgulama
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Kazanan belli olana kadar son etapi devam ettir
    while (true)
    {
        // Geriye kalan adaylari oyunu hesapla
        tabulate();

        // Secimin kazananı var mı kontrol et
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Sonuncu adayi ele
        int min = find_min();
        bool tie = is_tie(min);

        // Eger tie durumu varsa, herkes kazanir
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Minumum oylu birini ele
        eliminate(min);

        // Oy sayaclarini sıfırla
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Oy gecerli ise tercihi kayit et
bool vote(int voter, int rank, string name)
{
    // Adaylari tamamiyle kontrol et
    for (int i = 0 ; i < candidate_count ; i++)
    {
        // İsim gecerli mi kontrol et
        if (strcmp(candidates[i].name, name) == 0)
        {
            // Preferneces dizisine oy ekle
            preferences[voter][rank] = i;
            return true;
        }
    }

    return false;
}

// Elenmemis adaylar icin oylari tasnif et 
void tabulate(void)
{
    // Her oy kullanan icin
    for (int i = 0 ; i < voter_count ; i++)
    {
        // Elenmemis adaylar icin oy kullanicilarinin oylarini kontrol et ve oy ekle
        // Oy eklendi mi, breakle ve bir sonraki oy kullanicisina gec
        for (int j = 0 ; j < candidate_count ; j++)
        {
            if (!candidates[preferences[i][j]].eliminated)
            {
                candidates[preferences[i][j]].votes++;
                break;
            }
        }
    }

    return;
}

// Eger kazanan biri varsa ekrana yansit
bool print_winner(void)
{
    // Tum adaylarin oylarini kontrol et
    for (int i = 0 ; i < candidate_count ; i++)
    {
        // Eger adayin yarıdan fazla oyu var ise kazanan olarak ekrana yansit
        if (candidates[i].votes > (voter_count / 2))
        {
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Kalan adaylardan en dusuk oy alanin oy sayisini dondur
int find_min(void)
{
    // Minimum oy kullanici sayisini oy sayisi olarak varsay
    int min = voter_count;

    // Eger aday elenmediyse ve has mevcut minimumdan daha az oya sahip ise, yeni minimum yarat
    for (int i = 0 ; i < candidate_count ; i++)
    {
        if (!candidates[i].eliminated && candidates[i].votes < min)
        {
            min = candidates[i].votes;
        }
    }

    return min;
}

// Eger secim tum adaylar arasinda berabere bitti ise true dondur, diger durumlarda false dondur
bool is_tie(int min)
{

    for (int i = 0 ; i < candidate_count ; i++)
    {
        // Elenmemiş herhangi bir adayın oy sayısı min'den yüksekse, false değerini döndürün
        if (!candidates[i].eliminated && candidates[i].votes != min)
        {
            return false;
        }
    }

    return true;
}

// Sondaki aday veya adaylari ele
void eliminate(int min)
{
    for (int i = 0 ; i < candidate_count ; i++)
    {
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }

    return;
}
