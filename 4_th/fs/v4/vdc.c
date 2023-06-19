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
	//int btmt_bits;
	int no_of_flag_bytes;
}SB;

typedef struct FMD
{
	char file_name[240];
	long file_size;
	int block_no;
	unsigned char flag;
}FMD;

int write_fmds(int fd, long long disksize, int blocksize)
{
	int size_fmd = 0, no_of_fmd_blocks = 0, no_of_fmds = 0;
 
	size_fmd = ceil((double) disksize/100)*3;

	no_of_fmd_blocks = ceil((double) size_fmd/blocksize);

	no_of_fmds = (blocksize/sizeof(FMD)) * no_of_fmd_blocks;

	FMD *fmd = (FMD*)malloc(sizeof(FMD));
	strcpy(fmd -> file_name, "\0");
	fmd -> file_size = -1;
	fmd -> block_no = -1;
	fmd -> flag = 0;

	lseek(fd, blocksize, SEEK_SET);
	
	int i = 0;
	while(i < no_of_fmds)
	{
		write(fd, fmd, sizeof(FMD));
		i = i + 1;
	}	

	return no_of_fmd_blocks;
}

void read_fmds(int fd)
{
	SB *sb = (SB*)malloc(sizeof(SB));

	lseek(fd, 0, SEEK_SET);

	read(fd, sb, sizeof(SB));

	lseek(fd, sb -> block_size, SEEK_SET);

	FMD *fmd = (FMD*)malloc(sizeof(FMD));	

	int i = 0;
	while(i < 50)
	{
		read(fd, fmd, sizeof(FMD));
		printf("File name = %s\t", fmd -> file_name);
		printf("File size = %ld\t", fmd->file_size);
                printf("Block no = %d\t", fmd->block_no);
                printf("flag = %d\n", fmd -> flag);

                i = i + 1;

	}

	return;
}

long long write_flag_bits(int fd, long long disksize, int blocksize, int no_of_fmd_blocks)
{

	int no_of_bit_blocks = 0, no_of_long_long_ints = 0, fmd_blocks = 0,zero_bits = 0, zero_bytes = 0, zero_long_values = 0; 

	long long no_of_bits = 0, no_of_bytes = 0, no_of_blocks = 0;
	
	// no_of_total_blocks = no_of_bits
	no_of_bits = (long long )ceil((double)disksize/blocksize);
//	printf("No of bits = %lld\n",no_of_bits);

	// no_of_bytes of no_of_bits
	no_of_bytes = no_of_bits/8;
	
	// blocks for bits 
	no_of_bit_blocks = no_of_bytes/blocksize;
	
	// total 8 byte long values required to write.
	no_of_long_long_ints = no_of_bytes / 8;	

	// 1 block is of disk superblock 
	zero_bits = 1 + no_of_bit_blocks + no_of_fmd_blocks;

	//Data blocks remained after superblock, fmds blocks and bit blocks
	no_of_blocks = no_of_bits - zero_bits;

	//bytes we are going to make zero in bitmap to indicate occupied blocks.
	zero_bytes = ceil((double)zero_bits / 8);
//	printf("Zero bytes = %d\n",zero_bytes);
	
	int i = 0;

	unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * no_of_bytes);

	while(i < no_of_bytes)
	{
		buff[i] = 0xff;
		i = i + 1;
	}

	i = 0;
	int k = 0;

	while(i < zero_bytes)
	{
		for(int j = 7; j >= 0; j--)
		{
			buff[i] = buff[i] & (~(1 << j));
			k = k + 1;
			if(k == zero_bits)
			{	
//				printf("i = %d\tj = %d\tk = %d\tzero bits = %d\n",i,j,k,zero_bits);
				j = -1;
				i = zero_bytes;			
			}			
		}  

		i = i + 1;
	}

	lseek(fd, blocksize + (no_of_fmd_blocks * blocksize), SEEK_SET);
	write(fd, buff, no_of_bytes);

	return no_of_blocks;
}

void read_flag_bits(int fd, long long disksize, int blocksize)
{
	long long no_of_bits = ceil((double)disksize/blocksize);
	long long no_of_bytes = no_of_bits / 8;

	SB *sb = (SB*)malloc(sizeof(SB));

	lseek(fd, 0, SEEK_SET);
	read(fd, sb, sizeof(SB));

	//lseek(fd, sb -> btmt_bits, SEEK_SET);
	lseek(fd, blocksize + ((sb -> no_of_fmd_blocks) * blocksize), SEEK_SET);
	char buf[no_of_bytes + 10];
	long long cnt = read(fd, buf, no_of_bytes);

	for(int i = 100000; i < 100100; i++)
	{
		printf("%x\t",buf[i]);
	} 

	return;
}

void write_superblock(int fd, long long size_of_disk, int blocksize, int nofb, long long nob)
{
	SB *sb = (SB*)malloc(sizeof(SB));
	sb -> disk_size = size_of_disk;
	sb -> block_size = blocksize;
	sb -> no_of_fmd_blocks = nofb;
	sb -> no_of_fmds = nofb * (blocksize/sizeof(FMD));
	sb -> no_of_blocks = nob;

	//int bytes_to_move_to_bits = blocksize + (nofb * blocksize);

	//sb -> btmt_bits = bytes_to_move_to_bits;

	long long no_of_bytes = ((long long )ceil((double)size_of_disk/blocksize))/8;

	sb -> no_of_flag_bytes = no_of_bytes;

	lseek(fd, 0, SEEK_SET);

	write(fd, sb, sizeof(SB));

	return;
}

void read_superblock(int fd)
{
	lseek(fd, 0, SEEK_SET);

	SB *sb = (SB*)malloc(sizeof(SB));
	read(fd, sb, sizeof(SB));

	printf("Disk size = %lld\n", sb -> disk_size);
	printf("Block size = %d\n", sb -> block_size);
	printf("No. of fmd blocks = %d\n",sb -> no_of_fmd_blocks);
	printf("No. of fmds = %d\n", sb -> no_of_fmds);
	printf("No. of blocks = %lld\n", sb -> no_of_blocks);
	//printf("Btmt bits = %d\n", sb -> btmt_bits);
	printf("No of bit bytes = %d\n",sb -> no_of_flag_bytes);

	return;
}

void create_disk(char *diskname, int disksize, int blocksize)
{
	long long int size_of_disk = 0, nob = 0; 

	int nofb = 0, fd = 0;	

	size_of_disk = (long long int) disksize * 1024 * 1024 * 1024;
//	printf("Size of disk = %lld\n", size_of_disk);

	blocksize = blocksize * 1024;
//	printf("Block size = %d\n", blocksize);

	fd = open(diskname, O_CREAT | O_RDWR, 00600);	
	lseek(fd, (size_of_disk - 1), SEEK_SET);
	write(fd, '\0', 1);

	// returns no_of_fmds
	nofb = write_fmds(fd, size_of_disk, blocksize);
//	printf("No. of fmd blocks = %d\n", nofb);
	//read_fmds(fd);

	// returns no_of_blocks
	nob = write_flag_bits(fd, size_of_disk, blocksize, nofb);
	//printf("No of blocks = %lld\n", nob);

	//writing superblock
	write_superblock(fd, size_of_disk, blocksize, nofb, nob);
	//read_superblock(fd);
	
	//read_flag_bits(fd, size_of_disk, blocksize);
	
	return;
}

int main(int argc, char **argv)
{

	create_disk(argv[1], atoi(argv[2]), atoi(argv[3]));	

	return 0;
}

