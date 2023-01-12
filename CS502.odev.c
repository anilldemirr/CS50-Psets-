//CEASER.C

#include <stdio.h>
#include <string.h>
int main()
{
int i,adet; 
char message[20];
puts("Please Enter Your Message");
gets(message);
adet=strlen(message);
for(i=0;i<adet;i++)
{
  message[i]=message[i]-1;
}
printf("Message encrypted by caesar:\n");
printf("%s",message);
}
