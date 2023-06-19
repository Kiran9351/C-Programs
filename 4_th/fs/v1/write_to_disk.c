#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<math.h>
#include<string.h>

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

long disk_size;
int block_size;

int check_bits_availability(int fd, int filesize, unsigned char *bmap)
{
	int no_of_blocks = filesize;
	int bits_need = filesize + 1;
	
	int f = 0;
	
//	printf("Disk_size = %ld, Block size = %d\n",disk_size, block_size);

	lseek(fd, block_size, SEEK_SET);

	int flag_bytes = ceil((double)disk_size/block_size) / 8;
	int cnt = 0;

	for(int i = 0; i < flag_bytes; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(bmap[i] & (1 << j) == (1 << j))
			{
				cnt = cnt + 1;
				if(f == 0)
				{
					printf("%x\n", bmap[i]);
					f = (i * 8) + (7-j);
					bmap[i] = bmap[i] & (~(1 << j));
					printf("%x\n", bmap[i]); 
				}
			}
		}
	}

//	for(int i = 39000; i <50000 ; i++)
//        {
//                printf("%x",bmap[i]);
    
//        }

//        printf("\n");


//	printf("Cnt = %d\n", cnt);

	if(cnt > bits_need)
	{
		printf("Yes\n");
	}
	else
	{
		printf("No\n");
		f = -1;
	}

	return f;
}

unsigned char *get_bmap(int fd)
{
	lseek(fd, block_size, SEEK_SET);

	int flag_bytes = ceil((double)disk_size/block_size)/8;
	
	unsigned char* buf = (unsigned char*)malloc(sizeof(unsigned char) * flag_bytes);
		
	read(fd, buf, flag_bytes);

	return buf;
}

void find_and_write_fmd(int fd, char *filename, int filesize, int firstblock)
{
	FMD *fmd = (FMD*)malloc(sizeof(FMD));

	int flag_bytes = ceil((double)disk_size/block_size)/8;
	int bytes_to_move = block_size + flag_bytes;

	lseek(fd, bytes_to_move, SEEK_SET);

	int total_fmds = ceil((double)(ceil((double)disk_size/100)*3)/block_size)*4;

	int i = 0;

	while(i < total_fmds)
	{
		
		read(fd, fmd, sizeof(FMD));
		if(fmd -> flag == 0)
		{
			strcpy(fmd -> file_name, filename);
			fmd -> file_size = filesize * block_size;
			fmd -> block_no = firstblock;
			fmd -> flag = 1;

			lseek(fd, -(sizeof(FMD)), SEEK_CUR);
			
			write(fd, fmd, sizeof(FMD));
			i = total_fmds;
		}
	
		i = i + 1;
	}

//	lseek(fd, bytes_to_move, SEEK_SET);

//	read(fd, fmd, sizeof(FMD));
		
//	printf("File name = %s\n", fmd -> file_name);
//	printf("File size = %ld\n", fmd -> file_size);
//	printf("First block = %d\n", fmd -> block_no);
//	printf("Flag = %d\n", fmd -> flag);

	return ;	
}

void write_data_blocks_to_first_block(int fd, unsigned char *bmap, int first_block)
{
	int* block = (int*)malloc(sizeof(int) * (block_size/4));
	int k = 0;
	long int flag_bytes = ceil((double) disk_size/ block_size)/8;
	int j = 0;
	long int i = 0;
	for(i = 0; i < flag_bytes; i++)
	{
	
		//printf("I am here%ld\n",i);		

		for(j = 0; j < 8; j++)
		{
		if(bmap[i] & (1 << j) == (1 << j))
			{
				//printf("%x\n",bmap[i]);
				block[k] = (i * 8) + (7 - j);
				bmap[i] = bmap[i] & (~(1 << j));
				//printf("%x\n",bmap[i]);
				k = k + 1;
				//printf("K = %d\n",k);
				if(k == (block_size / 4))
				{
					j = 8;
					i = flag_bytes;
				}
			}
		}
	}

	lseek(fd, first_block * block_size, SEEK_SET);

	write(fd, block, block_size);
		
//	printf("\n");
	return ;
}

void read_first_block(int fd, char* filename)
{	
	int* block = (int*)malloc(sizeof(int) * (block_size / 4));

	long int no_of_bits = ceil((double) disk_size/ block_size);
	int flag_bytes = no_of_bits/8;
	
	int bytes_to_move = block_size + flag_bytes;

	lseek(fd, bytes_to_move, SEEK_SET);

	FMD *fmd = (FMD*)malloc(sizeof(FMD));

	int no_of_fmds = ceil((double)(ceil((double)disk_size/100)*3)/block_size)*4;
	int first_block = 0;

	for(int i = 0; i < no_of_fmds; i++)
	{
		read(fd, fmd, sizeof(FMD));

		if(strcmp(fmd -> file_name, filename) == 0)
		{
			first_block = fmd -> block_no;
			i = no_of_fmds;
		}
	}

	lseek(fd, first_block * block_size, SEEK_SET);

	read(fd, block, block_size);

	for(int i = 0; i < (block_size/4); i++)
	{
		printf("%d\t", block[i]);
	}

	printf("\n");

	return ;


void write_to_block(int fd, int block_no, unsigned char *buff)
{
	lseek(fd, block_no * block_size, SEEK_SET);

	write(fd, buff, block_size);

	return;
}

void write_data_to_blocks(int fd, char *filename)
{
	int flag_bytes= ceil((double)disk_size/block_size)/8, first_block = 0;

	int bytes_to_move = block_size + flag_bytes;

	lseek(fd, bytes_to_move, SEEK_SET);
	FMD *fmd = (FMD*)malloc(sizeof(FMD));

	int no_of_fmds = ceil((double)(ceil((double)disk_size/100)*3)/block_size)*4;
	
	for(int i = 0; i < no_of_fmds; i++)
	{
		read(fd, fmd, sizeof(FMD));

		if(strcmp(fmd -> file_name, filename) == 0)
		{
			first_block = fmd -> block_no;
			i = no_of_fmds;
		}
	}

	//printf("I am here\n");
	lseek(fd, block_size * first_block, SEEK_SET);

	int* pointer_block = (int*)malloc(sizeof(int) * (block_size / 4));

	read(fd, pointer_block, block_size);

	int fd1 = open("source.pdf", O_RDONLY);

	unsigned char *wbuff = (unsigned char*)malloc(sizeof(unsigned char) * block_size);

	int i = 0;
	//printf("I am here 2\n");
	while(read(fd1, wbuff, block_size) > 0)
	{
		int block_no = pointer_block[i];

		write_to_block(fd, block_no, wbuff);
		i = i + 1;
	}

	close(fd1);

	return;
}

unsigned char *read_from_block(int fd, int block_no)
{
	unsigned char *buff = (unsigned char*)malloc(sizeof(unsigned char) * block_size);

	lseek(fd, block_no * block_size, SEEK_SET);

	read(fd, buff, block_size);

	return buff;
}

void read_from_data_blocks(int fd, char* filename)
{
	int flag_bytes= ceil((double)disk_size/block_size)/8, first_block = 0;

        int bytes_to_move = block_size + flag_bytes;

        lseek(fd, bytes_to_move, SEEK_SET);
        FMD *fmd = (FMD*)malloc(sizeof(FMD));

        int no_of_fmds = ceil((double)(ceil((double)disk_size/100)*3)/block_size)*4;

        for(int i = 0; i < no_of_fmds; i++)
        {
                read(fd, fmd, sizeof(FMD));

                if(strcmp(fmd -> file_name, filename) == 0)
                {
                        first_block = fmd -> block_no;
                        i = no_of_fmds;
                }
        }

        lseek(fd, block_size * first_block, SEEK_SET);

        int* pointer_block = (int*)malloc(sizeof(int) * (block_size / 4));

        read(fd, pointer_block, block_size);

	int fd2 = open("dest.pdf", O_CREAT | O_RDWR, 00700);

	unsigned char* rbuff = (unsigned char*)malloc(sizeof(unsigned char) * block_size);

	for(int i = 0; i < (block_size / 8); i++)
	{
		int block_no = pointer_block[i];

		rbuff = read_from_block(fd, block_no);

		write(fd2, rbuff, block_size); 
	}

	close(fd2);
	
	return;
}

void write_file(char *filename, int filesize)
{
//	printf("File name = %s\n", filename);
//	printf("File size = %dkb\n", filesize);
	
	int fd = open("disk", O_RDWR);
	SB *sb = (SB*)malloc(sizeof(SB));
	read(fd, sb, sizeof(SB));

	disk_size = sb -> disk_size;
	block_size = sb -> block_size;

//	printf("Disk_size = %ld, Block size = %d\n",disk_size, block_size);

	unsigned char *bmap = get_bmap(fd);
	
	int first_block = check_bits_availability(fd, filesize, bmap);
//	printf("First block = %d\n", first_block);

	find_and_write_fmd(fd, filename, filesize, first_block);

	write_data_blocks_to_first_block(fd, bmap, first_block);	

//	read_first_block(fd, filename);

	write_data_to_blocks(fd, filename);
	
	read_from_data_blocks(fd, filename);

	return;
}

int main(int argc, char **argv)
{
	write_file(argv[1], atoi(argv[2]));

	return 0;
}
