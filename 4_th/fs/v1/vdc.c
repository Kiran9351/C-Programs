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
}SB;

typedef struct FMD 
{
        char file_name[240];
        long file_size;
        int block_no;
        unsigned char flag;
}FMD;

//Reading superblock
void read_superblock(int fd)
{
	SB *sb = (SB*) malloc(sizeof(SB));
	lseek(fd, 0, SEEK_SET);
	read(fd, sb, sizeof(SB));

//	printf("Disk size = %lld\n", sb -> disk_size);
//	printf("Block size = %d\n", sb -> block_size);

	return; 
}

// Writing superblock to disk.
void write_superblock(int fd, long long disksize, int blocksize)
{
	SB *sb = (SB*) malloc(sizeof(SB));

	sb -> disk_size = disksize;
	sb -> block_size = blocksize; 

	lseek(fd, 0, SEEK_SET);
	write(fd, sb, sizeof(SB));

	return;
}

// To write the flag bits.
void write_flag_bits(int fd, long long disksize, int blocksize)
{
	int no_of_blocks = ceil((double) disksize / blocksize);
//	printf("No of blocks = %d\n", no_of_blocks);

	int no_of_bits = no_of_blocks;
	int no_of_bytes = no_of_bits/8;
//	printf("No. of bytes = %d\n", no_of_bytes);

	int no_of_bit_blocks = no_of_bytes / blocksize;
//	printf("No of bit blocks = %d\n", no_of_bit_blocks);

	int no_of_long_long_ints = no_of_bytes / 8;
//	printf("No of long long ints = %d\n", no_of_long_long_ints);	

	unsigned long long val = ULLONG_MAX;
//	printf("%llx\n", val);
	lseek(fd, blocksize, SEEK_SET);

	int i = 0;
//	printf("Size of unsigned long long = %ld\n", sizeof(unsigned long long));

	while(i < no_of_long_long_ints)
	{
		write(fd, &val, sizeof(unsigned long long int));
		i = i + 1;
	}
		
	return;
}

//To read the flag bits.
void read_flag_bits(int fd, long long disksize, int blocksize)
{
	int no_of_bits = ceil((double) disksize/blocksize);
	int no_of_bytes = no_of_bits / 8;

	lseek(fd, blocksize, SEEK_SET);

	char buf[no_of_bytes+10];
	int cnt = read(fd, buf, no_of_bytes);	

	for(int i = 100000; i < 100100; i++)
	{
		printf("%x\n", buf[i]);
	}

	return;
}

// Write FMD to disk.
void write_fmd(int fd, long long disksize, int blocksize)
{
	int size_fmd = ceil((double) disksize/100)*3;
//	printf("Size fmd = %d\n", size_fmd);

	int no_of_fmd_blocks = ceil((double) size_fmd/blocksize);
//	printf("No of fmd blocks = %d\n", no_of_fmd_blocks);

	int no_of_fmds = no_of_fmd_blocks * 4;
//	printf("no of fmds = %d\n", no_of_fmds);

	FMD *fmd = (FMD*)malloc(sizeof(FMD));
	strcpy(fmd -> file_name, "\0");
	fmd -> file_size = -1;
	fmd -> block_no = -1;
	fmd -> flag = 0;

	int no_of_bits = ceil((double) disksize/blocksize);
	int no_of_flag_bytes = ceil((double)no_of_bits/8);
	int bytes_to_move = blocksize + no_of_flag_bytes;

	lseek(fd, bytes_to_move, SEEK_SET);
		
	int i = 0;

	while(i < no_of_fmds)
	{
		write(fd, fmd, sizeof(FMD));
		i = i + 1;
	}	
	
	return;
}

// Read FDM from disk.
void read_fmd(int fd, long long disksize, int blocksize)
{
	int no_of_bits = ceil((double)disksize/blocksize);
	int no_of_flag_bytes = no_of_bits/8;
	int bytes_to_move = blocksize + no_of_flag_bytes;

	lseek(fd, bytes_to_move, SEEK_SET);

	int i = 0;

	FMD *fmd = (FMD*)malloc(sizeof(FMD));

	while(i < 50)
	{
		read(fd, fmd, sizeof(FMD));
		printf("File name = %s\t", fmd->file_name);
		printf("File size = %ld\t", fmd->file_size);
		printf("Block no = %d\t", fmd->block_no);
		printf("flag = %d\n", fmd -> flag);

		i = i + 1;
	}

	return;
}

void make_bits_zero(int fd, long long disksize, int blocksize)
{
	int total_blocks_required = 0;

	int blocks_req_for_disk_metadata = 1;
	int blocks_req_for_bits = (ceil((double)disksize/blocksize)/8)/blocksize;
	int blocks_req_for_fmd = ceil((double)(ceil((double)disksize/100)*3)/blocksize);
	total_blocks_required = blocks_req_for_disk_metadata + blocks_req_for_bits + blocks_req_for_fmd;

	int no_of_bits = total_blocks_required;
	int no_of_bytes = no_of_bits/8;

	lseek(fd, blocksize, SEEK_SET);
	
	int total_long_long_values = no_of_bytes/8;

	unsigned long long value = 0;
	int i = 0;
	while(i < total_long_long_values)
	{
		write(fd, &value, sizeof(unsigned long long int));
		i = i + 1;
	}

	return ;
	
}

void create_disk(char *disk_name, long long disk_size, int block_size)
{	
//	char buf[10];
	int fd = open(disk_name, O_CREAT | O_RDWR, 00700);
	lseek(fd, disk_size - 1,SEEK_SET);
	write(fd, "\0", 1);
//	lseek(fd, disk_size - 1, SEEK_SET);
//	int d = read(fd, &buf, 1);
//	printf("Ch = %s\n",buf);

// Writing superblock to disk.
	write_superblock(fd, disk_size, block_size);
	read_superblock(fd);

// Writing bits to disk.
	write_flag_bits(fd, disk_size, block_size);

// Reading bits from disk.
//	read_flag_bits(fd, disk_size, block_size);
	
//Write FMD to disk.
	write_fmd(fd, disk_size, block_size);

// Reading FMD from disk.
//	read_fmd(fd, disk_size, block_size);

	make_bits_zero(fd, disk_size, block_size);
	read_flag_bits(fd, disk_size, block_size);

	return;
} 

int main(int argc, char **argv)
{
	long long int size_of_disk = 0;
	int size_of_block = 0;
	
//This size is in GB
	size_of_disk = atoi(argv[2]);
//	printf("Size of disk = %d", size_of_disk);

// This is size in bytes
	size_of_disk = size_of_disk * 1024 * 1024 * 1024;
//	printf("Size of disk in byters = %lld\n",size_of_disk);

//This size is in kb
	size_of_block = atoi(argv[3]);
	//printf("Size of block = %d", size_of_block);
	
//This size is in bytes
	size_of_block = size_of_block * 1024;
//	printf("Size of block in bytes = %lld\n", size_of_block);

//This is disk name.
//	printf("Disk name = %s\n",argv[1]);
	
	create_disk(argv[1], size_of_disk, size_of_block);

	return 0;
}
