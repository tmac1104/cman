#include <string.h>
#include <stdio.h>
void MergeList (int a[], int b[],int c[],int la_len,int lb_len);
int main(int argc , char *argv[])
{
	int a[] = {3,5,8,11};
	int b[] = {2,6,8,9,11,15,20};
	int la_len = sizeof(a)/sizeof(int);
        int lb_len = sizeof(b)/sizeof(int);
	int c[20] = {0} ,i ;
	MergeList(a,b,c,la_len,lb_len) ;
	for( i = 0 ; i <20 ;i++)
		printf("[%d]",c[i]);
	return 0;
}
void MergeList (int a[], int b[],int c[],int la_len,int lb_len)
{
	int i = 0 , j = 0 ,k = 0;
	while ((i< la_len) && (j<lb_len))
	{
		if(a[i] <= b[j])
			c[k++]=a[i++];
		else
			c[k++]=b[j++];
	}
	
	while(i< la_len)
		c[k++]=a[i++];
	while(j< lb_len)
		c[k++]=b[j++];
}
