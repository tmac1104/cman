#include <string.h>
#include <stdio.h>
void replace(int *a,int *b);
int main(int argc , char *argv[])
{
	int a = 5,b=2;
	replace(&a,&b);	
	printf("a=[%d],b=[%d]\n",a,b);	
	return 0;
}
void replace(int *a,int *b)
{
	*a=*a^*b;
        *b=*b^*a;
        *a=*a^*b;
}
