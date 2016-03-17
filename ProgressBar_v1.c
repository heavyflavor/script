#include <stdio.h>  
int main()
{
  int i;
  char x[300];
  memset(x,0,300);
  for(i=0;i<100;i++)
    {
      sprintf(&x[0],"%2d",i);
      x[2]='%';
      x[3+i]='=';
      printf("\r%s>",x);
      fflush(stdout);
      sleep(1);
    }
printf("\n");

} 
