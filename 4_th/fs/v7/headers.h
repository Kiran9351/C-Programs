
/*

Including header files
Structure of super block and function metadata

*/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<math.h>
#include<sys/stat.h>
#include<limits.h>
#include<string.h>


typedef struct SB
{
	long long disk_size;
	int block_size;
	long long no_of_blocks;
	int no_of_fmd_blocks;
	int no_of_fmds;
	int no_of_flag_bytes;
	int no_of_files;
}SB;

typedef struct FMD
{
	char file_name[240];
	long file_size;
	int block_no;
	unsigned char flag;
}FMD;

