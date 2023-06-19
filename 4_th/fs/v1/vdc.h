#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<math.h>
#include<sys/stat.h>

typedef struct SB
{
	long disk_size;
	int block_size;
}SB;

typedef struct FMD
{
	char file_name[240];
	long file_size;
	int block_no;
	unsigned char flag;
}FMD;
