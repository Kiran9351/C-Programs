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
	long long disk_size;
	int block_size;
	long long no_of_blocks;
	int no_of_fmds;
	int btmt_bits;
	int no_of_flag_bytes;
}SB;

typedef struct FMD
{
	char file_name[240];
	long long file_size;
	int block_no;
	unsigned char flag;
}FMD;

long long disksize;
int blocksize;
long long no_of_blocks;
int no_of_fmds;
int btmt_bits;
int no_of_flag_bytes;
int *arr = NULL;
int arr_size;
unsigned char *bmap;
int count_bits;
int fs;

void initialize_global_variables(int fd)
{
	SB *sb = (SB*)malloc(sizeof(SB));
	lseek(fd, 0, SEEK_SET);

	read(fd, sb, sizeof(SB));

	disksize = sb -> disk_size;
	blocksize = sb -> block_size;
	no_of_blocks = sb -> no_of_blocks;
	no_of_fmds = sb -> no_of_fmds;
	btmt_bits = sb -> btmt_bits;
	no_of_flag_bytes = sb -> no_of_flag_bytes;

	int total_blocks = ceil((double)disksize/blocksize);
	int bptr_in_one_block = blocksize/sizeof(int);
	
	int size_of_arr = 0;

	while(total_blocks > 1)
	{
		size_of_arr = size_of_arr + 1;
		total_blocks = ceil((double) total_blocks/bptr_in_one_block);
	}

	size_of_arr += 1;

	arr = (int*)malloc(sizeof(int) * (size_of_arr));

	return;
}

unsigned char* get_bmap(int fd)
{
	lseek(fd, btmt_bits, SEEK_SET);
	unsigned char *buf = (unsigned char*)malloc(sizeof(unsigned char) * no_of_flag_bytes);

	read(fd, buf, no_of_flag_bytes);

	return buf;
}

void print_bmap()
{
	int i = 0;

	for(i = 39000; i < 40000; i++)
	{
		printf("%x\t",bmap[i]);
	}

	printf("\n");
	
	return;
}

int generate_arr(int filesize)
{
	int i = 0, data_blocks = 0, bptr_in_one_block = 0, blocks_need = 0, first_block = -1;

	data_blocks = ceil((double)filesize/blocksize);
        bptr_in_one_block = blocksize/sizeof(int);
        blocks_need = data_blocks;

//      printf("Data blocks = %d\n", data_blocks);

        while(data_blocks > 1)
        {
//              printf("I am here\n");
                arr[i] = data_blocks;
                data_blocks = ceil((double)data_blocks/bptr_in_one_block);
                blocks_need = blocks_need + data_blocks;

//              printf("blocks need = %d, data_blokcks = %d, arr[%d] = %d\n",blocks_need, data_blocks, i, arr[i]);
                i = i + 1;
        }

        arr[i] = data_blocks;
        arr_size = i+1;
	
	return blocks_need;
}

int check_bits_availability(int filesize)
{
	int i = 0, first_block = -1, blocks_need = 0;
/*
	data_blocks = ceil((double)filesize/blocksize);
	bptr_in_one_block = blocksize/sizeof(int);
	blocks_need = data_blocks;

//	printf("Data blocks = %d\n", data_blocks);
	
	while(data_blocks > 1)
	{
//		printf("I am here\n");
		arr[i] = data_blocks;
		data_blocks = ceil((double)data_blocks/bptr_in_one_block);
		blocks_need = blocks_need + data_blocks;

//		printf("blocks need = %d, data_blokcks = %d, arr[%d] = %d\n",blocks_need, data_blocks, i, arr[i]);
		i = i + 1;
	}

	arr[i] = data_blocks;
	arr_size = i+1;
*/
	
	blocks_need = generate_arr(filesize);

	if(blocks_need < no_of_blocks)
	{
		for(i = 0; i < no_of_flag_bytes; i++)
		{
			for(int j = 0; j < 8; j++)
			{
				if(bmap[i] & ( 1 << j ) == ( 1 << j ))
				{
					first_block = (i * 8) + (7-j);
					bmap[i] = bmap[i] & (~(1 << j));
					no_of_blocks = no_of_blocks - 1;
					j = 8;
					i = no_of_flag_bytes;
				}
			}
		}
	}

/*
	printf("arr size = %d\n",arr_size);
	for(i = 0; i < arr_size; i++)
	{
		printf("%d = %d\n",i, arr[i]);
	} 
*/
	return first_block;
	
}

int find_and_write_fmd(int fd, char* filename, int filesize, int first_block)
{

	if(no_of_fmds <= 0)
	{
		return -1;
	}

	int i = 0;

	FMD *fmd = (FMD*)malloc(sizeof(FMD));
	
	lseek(fd, blocksize, SEEK_SET);

//	printf("No of fmds = %d\n", no_of_fmds);
	
	while(i < no_of_fmds)
	{
		read(fd, fmd, sizeof(FMD));
	
		if(fmd -> flag == 0)
		{
			strcpy(fmd -> file_name, filename);
			fmd -> file_size = filesize;
			fmd -> block_no = first_block;
			fmd -> flag = 1;

			lseek(fd, -(sizeof(FMD)), SEEK_CUR);
	
			write(fd, fmd, sizeof(FMD));

			no_of_fmds = no_of_fmds - 1;
		
			i = no_of_fmds;
		}

		i = i + 1;
	}	

//	printf("No. of fmds = %d\n", no_of_fmds);

//	lseek(fd, blocksize, SEEK_SET);
	
//     	read(fd, fmd, sizeof(FMD));
    
//   	printf("File name = %s\n", fmd -> file_name);
//      printf("File size = %lld\n", fmd -> file_size);
//      printf("First block = %d\n", fmd -> block_no);
//	printf("Flag = %d\n", fmd -> flag);

	return 1;
}

void write_pointer_block(int fd, int block_no, int *arr, int no_of_bits)
{
	lseek(fd, block_no * blocksize, SEEK_SET);
	
	write(fd, arr, sizeof(int)*no_of_bits);	

	return;
}

int *get_bit_indices(int no_of_bits)
{
	int *bit_arr = (int*)malloc(sizeof(int) * no_of_bits);

	int i = 0, j = 0, n = 0;

	for(i = 0; i < no_of_flag_bytes; i++)
	{
		for(j = 0; j < 8; j++)
		{

			//printf("i = %d, j = %d\t", i,j);
			if((bmap[i] & ( 1 << j )) == ( 1 << j ))
			{
//				printf("1 : %d, %x, %x, %x\n",i, bmap[i], (1 << j), ~(1 << j));
				bit_arr[n] = (i * 8) + (7 - j);
				bmap[i] = bmap[i] & (~(1 << j));
//				printf("2 : %d, %x, %x, %x\n", i, bmap[i], (1<<j), ~(1 <<j)); 
				n = n + 1;
//				count_bits = count_bits + 1;
//				printf("%d\n",count_bits);
				if(n == no_of_bits)
				{
					j = 8;
					i = no_of_flag_bytes;
				}	
			}
		}
	}

	no_of_blocks = no_of_blocks - no_of_bits;

	return bit_arr;
}

void write_first_block(int fd, int first_block, int no_of_bits)
{
	int *bit_arr = NULL;

	//print_bmap();

	bit_arr = get_bit_indices(no_of_bits);

	write_pointer_block(fd, first_block, bit_arr,no_of_bits);
	
	return;
}

void write_parent(int fd, int block_no, int no_of_ints, int arr_index);

void write_child(int fd, int block_no, int arr_index)
{
	int no_of_ints = 0, *bit_arr = NULL;

	if(arr[arr_index] < (blocksize/sizeof(int)))
	{
		no_of_ints = arr[arr_index];
		arr[arr_index] = arr[arr_index] - no_of_ints;
	}
	else
	{
		no_of_ints = (blocksize/sizeof(int));
		arr[arr_index] = arr[arr_index] - (blocksize/sizeof(int));
	}

	bit_arr = get_bit_indices(no_of_ints);
		
	write_pointer_block(fd, block_no, bit_arr, no_of_ints);

//	if(arr_index == 0)
//	{
//		read_pointer_block(fd, block_no, no_of_ints);
//	}
	
	if(arr_index != 0)
	{
		write_parent(fd, block_no, no_of_ints, arr_index);
	}

	return; 
}

void write_parent(int fd, int block_no, int no_of_ints, int arr_index)
{
	int i = 0, *iarr = NULL;

	lseek(fd, block_no * blocksize, SEEK_SET);
	iarr = (int*)malloc(sizeof(int) * no_of_ints);
	read(fd, iarr, sizeof(int)*no_of_ints);
	
	while(i < no_of_ints)
	{
		write_child(fd, iarr[i], arr_index - 1); 
		i = i + 1;
	}

	return;
}

/*
void read_pointer_block(int fd, int block_no, int no_of_ints)
{
	int i = 0, *bit_arr = NULL;

	bit_arr = (int*)malloc(sizeof(int) * no_of_ints);
	
	lseek(fd, block_no * blocksize, SEEK_SET);
	read(fd, bit_arr, sizeof(int)*no_of_ints);

//	while(i < no_of_ints)
//	{
//		printf("%d\t",bit_arr[i]);
//		i = i + 1;
//	}

//	printf("\n");

	return;
}

void read_parent(int fd, int block_no, int no_of_ints, int arr_index);

void read_child(int fd, int block_no, int arr_index)
{
	int no_of_ints = 0;

	if(arr[arr_index] < (blocksize/sizeof(int)))
	{
		no_of_ints = arr[arr_index];
		arr[arr_index] = arr[arr_index] - no_of_ints;
	}
	else
	{
		no_of_ints = (blocksize/sizeof(int));
		arr[arr_index] = arr[arr_index] - (blocksize/sizeof(int));
	}

	if(arr_index == 0)
	{
		read_pointer_block(fd, block_no, no_of_ints);
	}
	else
	{
		read_parent(fd, block_no, no_of_ints, arr_index);
	}

	return;
}

void read_parent(int fd, int block_no, int no_of_ints, int arr_index)
{
	int i = 0, *iarr = NULL;

	lseek(fd, block_no * blocksize, SEEK_SET);
	iarr = (int*)malloc(sizeof(int)*no_of_ints);
	
	read(fd, iarr, sizeof(int)*no_of_ints);
	
	while(i < no_of_ints)
	{
		read_child(fd, iarr[i], arr_index - 1);
		i = i + 1;
	}

	return;
}
*/

void write_data_block(int fd, int block_no, unsigned char* buff, int cnt_bytes)
{
	lseek(fd, block_no * blocksize, SEEK_SET);
	
	write(fd, buff, cnt_bytes);
	
	return ;
}

void read_pointer_block(int fd, int fd2, int block_no, int no_of_ints)
{
        int i = 0, *bit_arr = NULL, cnt_bytes = 0;
	unsigned char *buff = NULL;

	buff = (unsigned char*)malloc(sizeof(unsigned char) * blocksize);

        bit_arr = (int*)malloc(sizeof(int) * no_of_ints);

        lseek(fd, block_no * blocksize, SEEK_SET);
        read(fd, bit_arr, sizeof(int)*no_of_ints);


	while(i < no_of_ints)
	{
		read(fd2, buff, blocksize);
	
		write_data_block(fd, bit_arr[i], buff, blocksize);
	
		i = i + 1;
	}	

        return;
}

void read_parent(int fd, int fd2, int block_no, int no_of_ints, int arr_index);

void read_child(int fd, int fd2, int block_no, int arr_index)
{
        int no_of_ints = 0;

        if(arr[arr_index] < (blocksize/sizeof(int)))
        {
                no_of_ints = arr[arr_index];
                arr[arr_index] = arr[arr_index] - no_of_ints;
        }
        else
        {
                no_of_ints = (blocksize/sizeof(int));
                arr[arr_index] = arr[arr_index] - (blocksize/sizeof(int));
        }

        if(arr_index == 0)
        {
                read_pointer_block(fd, fd2, block_no, no_of_ints);
        }
        else
        {
                read_parent(fd, fd2, block_no, no_of_ints, arr_index);
        }

        return;
}

void read_parent(int fd, int fd2, int block_no, int no_of_ints, int arr_index)
{
        int i = 0, *iarr = NULL;

        lseek(fd, block_no * blocksize, SEEK_SET);
        iarr = (int*)malloc(sizeof(int)*no_of_ints);

        read(fd, iarr, sizeof(int)*no_of_ints);

        while(i < no_of_ints)
        {
                read_child(fd, fd2, iarr[i], arr_index - 1);
                i = i + 1;
        }

        return;
}

void create_file(int fd,char *source, int block_no, int no_of_ints, int arr_index)
{
	int fd2 = 0, fd3 = 0;

	fd2 = open(source, O_RDWR);
	
	read_parent(fd, fd2,block_no, no_of_ints, arr_index);

	close(fd2);
	
	return;
}

int* copy_arr(int *arr, int size)
{
	int i = 0, *brr = NULL;
	
	brr = (int*)malloc(sizeof(int)*size);

	while(i < size)
	{
		brr[i] = arr[i];
		i = i + 1;
	}

	return brr;
}

void write_metadata(int fd)
{
	SB *sb = (SB*)malloc(sizeof(SB));

	sb -> disk_size = disksize;
	sb -> block_size = blocksize;
	sb -> no_of_blocks = no_of_blocks;
	sb -> no_of_fmds = no_of_fmds;
	sb -> btmt_bits = btmt_bits;
	sb -> no_of_flag_bytes = no_of_flag_bytes;

	lseek(fd, 0, SEEK_SET);
	write(fd, sb, sizeof(SB));

	lseek(fd, btmt_bits, SEEK_SET);
		
	write(fd, bmap, no_of_flag_bytes);

	return;
}

int filesize_bytes(float filesize, char *post)
{
	int fs = 0;
	
	if(strcmp(post, "b") == 0)
	{
		fs = filesize;
	}
	else if(strcmp(post, "k") == 0)
	{
		fs = ceil(filesize * 1024);
	}
	else if(strcmp(post, "m") == 0)
	{
		fs = ceil(filesize * 1024 * 1024);
	}
	else
	{
		fs = ceil(filesize * 1024 * 1024 * 1024);		
	}

	return fs;
}

int get_size(char *source)
{
	int fd = 0, fs = 0;

	fd = open(source, O_RDWR);
	fs = lseek(fd, 0, SEEK_END);

	return fs;
}

/////////////////////////////////////////////////////////////////////

//void read_file(char* filename);

///void write_file(char *filename, float filesize, char* post, char *source)
void write_file(char* filename, char *source)
{
//	printf("File name = %s\n", filename);
//	printf("File size = %lld", filesize);

	int fd = 0, first_block = 0, check = 0, filesize  = 0;

	//filesize = filesize_bytes(filesize, post);
	filesize = get_size(source);
	fd = open("disk", O_RDWR);

	initialize_global_variables(fd);

	//create bmap
	bmap = get_bmap(fd);

	first_block = check_bits_availability(filesize);
	
	if(first_block == -1)
	{
		printf("Not enough space in disk\n");
		return;
	}

//	printf("First block = %d\n",first_block);

	check = find_and_write_fmd(fd, filename, filesize, first_block);

	if(check == -1)
	{
		printf("fmd is not there\n");
		return;	
	}
	
	int bef = no_of_blocks;
	
	write_first_block(fd, first_block, arr[arr_size - 2]);
	int *brr = copy_arr(arr,arr_size);

	write_parent(fd, first_block, arr[arr_size - 2], arr_size - 2);
	
//	printf("cnt = %d, bef = %d, no_of_blocks = %lld\n",count_bits, bef, no_of_blocks);
	
	arr = copy_arr(brr,arr_size);
//	read_parent(fd, first_block, arr[arr_size-2], arr_size - 2);

	create_file(fd, source, first_block, arr[arr_size - 2], arr_size - 2);

	write_metadata(fd);	

	close(fd);

//	read_file(filename);

	return;
}

//////////////////////////////////////////////////////////////////////

int find_file(int fd, char *filename)
{
	int total_fmds = 0, i = 0, first_block = -1;

	lseek(fd, blocksize, SEEK_SET);
	
	total_fmds = (btmt_bits - blocksize)/sizeof(FMD);

	FMD *fmd = (FMD*)malloc(sizeof(FMD));	

	while(i < total_fmds)
	{
		read(fd, fmd, sizeof(FMD));
		
		if(strcmp(fmd -> file_name, filename) == 0)
		{
			first_block = fmd -> block_no;
			printf("Filesize= %lld\n",fmd->file_size);
			//fs = fmd -> file_size;
			int bn = generate_arr(fmd->file_size);
		}

		i = i + 1;
	}
/*
	for(int j = 0; j < arr_size; j++)
	{
		printf("%d\n",arr[j]);
	}
*/
	return first_block;
}

unsigned char* read_data_block(int fd, int block_no)
{
	unsigned char* buff = NULL;

	buff = (unsigned char*)malloc(sizeof(unsigned char) * blocksize);

	lseek(fd, block_no * blocksize, SEEK_SET);
	if(fs > blocksize)
	{
		read(fd, buff, blocksize);
		fs = fs - blocksize;
	}
	else
	{
		read(fd, buff, fs);
		fs  = fs - fs;
	}
	return buff;
}

void rd_pointer_block(int fd, int fd2, int block_no, int no_of_ints)
{
        int i = 0, *bit_arr = NULL;
        unsigned char *buff = NULL;

        buff = (unsigned char*)malloc(sizeof(unsigned char) * blocksize);

        bit_arr = (int*)malloc(sizeof(int) * no_of_ints);

        lseek(fd, block_no * blocksize, SEEK_SET);
        read(fd, bit_arr, sizeof(int)*no_of_ints);


        while(i < no_of_ints)
        {
                buff = read_data_block(fd, bit_arr[i]);
         
                write(fd2, buff, blocksize);

		i = i + 1;
        }
 
        return;
}

void rd_parent(int fd, int fd2, int block_no, int no_of_ints, int arr_index);

void rd_child(int fd, int fd2, int block_no, int arr_index)
{
        int no_of_ints = 0;

        if(arr[arr_index] < (blocksize/sizeof(int)))
        {
                no_of_ints = arr[arr_index];
                arr[arr_index] = arr[arr_index] - no_of_ints;
        }
        else
        {
                no_of_ints = (blocksize/sizeof(int));
                arr[arr_index] = arr[arr_index] - (blocksize/sizeof(int));
        }

        if(arr_index == 0)
        {
                rd_pointer_block(fd, fd2, block_no, no_of_ints);
        }
        else
        {
                rd_parent(fd, fd2, block_no, no_of_ints, arr_index);
        }

        return;
}

void rd_parent(int fd, int fd2, int block_no, int no_of_ints, int arr_index)
{
        int i = 0, *iarr = NULL;

        lseek(fd, block_no * blocksize, SEEK_SET);
        iarr = (int*)malloc(sizeof(int)*no_of_ints);

        read(fd, iarr, sizeof(int)*no_of_ints);

        while(i < no_of_ints)
        {
                rd_child(fd, fd2, iarr[i], arr_index - 1);
                i = i + 1;
        }

        return;
}

void rd_file(int fd, char* dest, int first_block, int no_of_ints, int arr_index)
{
	int fd2 = 0;

	fd2 = open(dest, O_CREAT | O_RDWR, 00700);

	rd_parent(fd, fd2, first_block, no_of_ints, arr_index);

	close(fd2);

	return;
}


void read_file(char *filename, char *dest)
{
	int fd = 0, first_block = 0;

	fd = open("disk", O_RDWR);

	initialize_global_variables(fd);

	first_block = find_file(fd, filename);
	
	if(first_block == -1)
	{
		printf("File is not there\n");
		return;
	}

	rd_file(fd, dest, first_block, arr[arr_size - 2], arr_size - 2);

	close(fd);	

	return;
}

int main(int argc, char **argv)
{
	// gcc pname -w fname sfile
	if(strcmp(argv[1], "-w") == 0)
	{
		write_file(argv[2], argv[3]);
	}
	else
	{
		//gcc pname -r fname dfile
		read_file(argv[2], argv[3]);
	}
	return 0;
}

