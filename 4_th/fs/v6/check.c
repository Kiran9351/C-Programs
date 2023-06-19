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
/*
	char a [] = "\nData blocks are :\n";

	printf("%d\n", sizeof(a));
*/
/*
	int val = 8;

	val = val | (1 << 2);

	printf("Val = %d\n",val);
*/

	unsigned char *map = NULL;

	map = (unsigned char*)malloc(sizeof(unsigned char) * 10);

	for(int i = 0; i < 10; i++)
	{
		map[i] = 0;
	}

	int block_no = 50;
	int map_index = block_no / 8;
	int bit_no = block_no % 8;

	for(int i = 0; i < 10; i++)
	{
		printf("b %d %x\n",i,map[i]);
	}

	map[map_index] = map[map_index] | (1 << (7 - bit_no));

	for(int i = 0; i < 10; i++)	
	{
		printf("a %d %x\n", i, map[i]);
	}
		
	return 0;
}
