#include <stdio.h>
#include <string.h>
int main(){
char dizi[20];
puts("pls enter your message");
gets(dizi);
int adet=strlen(dizi);
for(int i=0;i<adet;i++)
{
  dizi[i]=dizi[i]+1;
  
}
printf("Message encrypted by caesar:\n");
printf("%s",dizi);
}