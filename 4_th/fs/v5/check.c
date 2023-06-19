#include<stdio.h>
#include<stdlib.h>

int main()
{
/*
	unsigned char *ch = NULL;
	ch = (unsigned char*)malloc(sizeof(unsigned char) * 10);

	int i = 0;

	while(i < 10)
	{
		ch[i] = 0xff;
		i = i + 1;
	}

	i = 0;
	while(i < 10)
	{
		printf("ch[%d] = %x\t%d\n",i,ch[i],ch[i]);
		i = i + 1;
	}
*/

	char a [] = "\nData blocks are :\n";

	printf("%d\n", sizeof(a));
	
	return 0;
}
