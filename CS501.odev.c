#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main()
{
   float paraustu;
    int yirmibessayisi=0,onluksayisi=0,besliksayisi=0,birliksayisi=0;
printf("\n\nPara ustu verilecek tutari giriniz:");
scanf("%f",&paraustu);
paraustu=round(paraustu*(100));

yirmibessayisi=paraustu/25;
paraustu=paraustu-(25*yirmibessayisi);

onluksayisi=paraustu/10;
paraustu=paraustu-(10*onluksayisi);
 
 besliksayisi=paraustu/5;
 paraustu=paraustu-(5*besliksayisi);

 birliksayisi=paraustu/1;
 paraustu=(1*birliksayisi);


printf("%d adaet 25 krs\n",yirmibessayisi);
printf("%d adet 10 krs \n",onluksayisi);
printf("%d adet 5 krs \n",besliksayisi);
printf("%d adet 1 krs\n",birliksayisi);



}
