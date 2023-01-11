#include <stdlib.h>
#include <stdio.h>


int main()
{
float paraustu;
float yirmibes=0.25,onluk=0.10,beslik=0.05,birlik=0.01;
int yirmibessayisi=0,onluksayisi=0,besliksayisi=0,birliksayisi=0;
printf("\n\nPara üstü verilecek tutar:");
scanf("%f",&paraustu);
while (paraustu>0.01)
{
if(paraustu>yirmibes)
{
paraustu=paraustu-yirmibes;
yirmibessayisi++;
continue;
}
if (paraustu>onluk)
{
    paraustu=paraustu-onluk;
    onluksayisi++;
    continue;
}

if (paraustu>beslik)
{
    paraustu=paraustu-beslik;
    besliksayisi++;
    continue;
}

if (paraustu>birlik)
{
    paraustu=paraustu-birlik;
    birliksayisi++;
    continue;
}

}
printf("%d adet 25 krs\n",yirmibessayisi);
printf("%d adet 10 krs \n",onluksayisi);
printf("%d adet 5 krs \n",besliksayisi);
printf("%d adet 1 krs\n",birliksayisi);


}
