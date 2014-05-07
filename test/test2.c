#include <string.h>
#include <stdio.h>
int main(int argc , char *argv[])
{
	int a = 5,b=2;
	a=a^b;
        b=b^a;
        a=a^b;	
	printf("a=[%d],b=[%d]\n",a,b);	
	return 0;
}
