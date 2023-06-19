
/*

This is implemenation to write a file on virtual disk.

*/

#include"bitmap.h"

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
	int total_fmds = no_of_fmd_blocks * (blocksize / sizeof(FMD));	

	while(i < total_fmds)
	{
		read(fd, fmd, sizeof(FMD));
	
		if(fmd -> flag == 0)
		{
			strcpy(fmd -> file_name, filename);
			fmd -> file_size = filesize;
			fs = fmd->file_size;		
//			printf("Fs = %d\n",fs);
			fmd -> block_no = first_block;
			fmd -> flag = 1;

			lseek(fd, -(sizeof(FMD)), SEEK_CUR);
	
			write(fd, fmd, sizeof(FMD));

			no_of_fmds = no_of_fmds - 1;
		
			i = total_fmds;
		}

		i = i + 1;
	}	

	return 1;
}

void write_pointer_block(int fd, int block_no, int *arr, int no_of_bits)
{
	lseek(fd, block_no * blocksize, SEEK_SET);
	
	write(fd, arr, sizeof(int)*no_of_bits);	

	return;
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

void write_data_block(int fd, int block_no, unsigned char* buff, int cnt_bytes)
{
	lseek(fd, block_no * blocksize, SEEK_SET);
	
	write(fd, buff, cnt_bytes);
	
	return ;
}


void read_pointer_block(int fd, int fd4, int block_no, int no_of_ints)
{
        int i = 0, *bit_arr = NULL;
	unsigned char *buff = NULL, *buff_1 = NULL;

	buff = (unsigned char*)malloc(sizeof(unsigned char) * blocksize);
	
        bit_arr = (int*)malloc(sizeof(int) * no_of_ints);	

        lseek(fd, block_no * blocksize, SEEK_SET);
        read(fd, bit_arr, sizeof(int)*no_of_ints);

        while(i < no_of_ints)
        {
		if(fs >= blocksize)
		{
			read(fd4, buff, blocksize);
			write_data_block(fd, bit_arr[i], buff, blocksize);
			fs = fs - blocksize;
		}
		else
		{
			buff_1 = (unsigned char*)malloc(sizeof(unsigned char) * fs);
			read(fd4, buff_1, fs);
			write_data_block(fd, bit_arr[i], buff_1, fs);
			fs = fs - fs;
		}

		i = i + 1;
                counts = counts + 1;
        }
	
	progress = progress + no_of_ints;

	if(progress >= ((total_blocks_to_write * per)/100))
	{
		printf("--%d%%--",per);
		per = per + 10;
	}

        return;
}

void read_parent(int fd, int fd4, int block_no, int no_of_ints, int arr_index);

void read_child(int fd, int fd4, int block_no, int arr_index)
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
                read_pointer_block(fd, fd4, block_no, no_of_ints);
        }
        else
        {
                read_parent(fd, fd4, block_no, no_of_ints, arr_index);
        }

        return;
}

void read_parent(int fd, int fd4, int block_no, int no_of_ints, int arr_index)
{
        int i = 0, *iarr = NULL;

        lseek(fd, block_no * blocksize, SEEK_SET);
        iarr = (int*)malloc(sizeof(int)*no_of_ints);

        read(fd, iarr, sizeof(int)*no_of_ints);

        while(i < no_of_ints)
        {
                read_child(fd, fd4, iarr[i], arr_index - 1);
                i = i + 1;
        }

        return;
}


void create_file(int fd,char *source, int block_no, int no_of_ints, int arr_index)
{
	int fd4 = 0;

	fd4 = open(source,O_RDWR);
	
	read_parent(fd, fd4, block_no, no_of_ints, arr_index);

	close(fd4);

	//printf("Written bytes = %d\n", count_written_bytes);
	
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

void write_superblock(int fd)
{
	SB *sb = (SB*)malloc(sizeof(SB));

	sb -> disk_size = disksize;
	sb -> block_size = blocksize;
	sb -> no_of_blocks = no_of_blocks;
	sb -> no_of_fmd_blocks = no_of_fmd_blocks;
	sb -> no_of_fmds = no_of_fmds;
	sb -> no_of_flag_bytes = no_of_flag_bytes;
	sb -> no_of_files = sb -> no_of_files + 1;

	lseek(fd, 0, SEEK_SET);
	write(fd, sb, sizeof(SB));	
	
	return;
}

void write_bitmap(int fd)
{
	lseek(fd, blocksize + (no_of_fmd_blocks * blocksize), SEEK_SET);
//	printf("Write bitmap no of flag bytes = %d\n",no_of_flag_bytes);
	write(fd, bmap, no_of_flag_bytes);
	
	return;
}

void read_superblock(int fd) 
{
        SB *sb = (SB*)malloc(sizeof(SB));
	printf("size of sb = %ld\n",sizeof(SB));

        lseek(fd, 0, SEEK_SET);

        read(fd, sb, sizeof(SB));

        printf("Disk size = %lld\n",sb -> disk_size);
        printf("block size = %d\n", sb -> block_size);
        printf("No of blocks = %lld\n",sb -> no_of_blocks);
        printf("No of fmd blocks = %d\n", sb ->no_of_fmd_blocks);
        printf("NO of fmds = %d\n",sb -> no_of_fmds);
        printf("No of flag bytes = %d\n", sb -> no_of_flag_bytes);
	printf("No of files = %d\n", sb -> no_of_files);

        return;
}


int get_size(char *source)
{
	int fd = 0, fs = 0;

	fd = open(source, O_RDWR);
	fs = lseek(fd, 0, SEEK_END);

	close(fd);

	return fs;
}

char check_file(int fd, char* filename)
{
        lseek(fd, blocksize, SEEK_SET);

        FMD *fmd = (FMD*)malloc(sizeof(FMD));

        int total_fmds = no_of_fmd_blocks * (blocksize / sizeof(FMD)), j = 0, i = 0;

        char ch = 'n';

        while(i < total_fmds)
        {
                read(fd, fmd, sizeof(FMD));

                if(fmd -> flag == 1)
                {
                        if(strcmp(fmd -> file_name, filename) == 0)
                        {
                                ch = 'y';
                                i = total_fmds;
                        }

                        j = j + 1;
                }

                if(j == no_of_files)
                {
                        i = total_fmds;
                }

                i = i + 1;
        }

        return ch;
}


/////////////////////////////////////////////////////////////////////


void write_file(char* filename, char *source)
{

	int fd = 0, first_block = 0, check = 0, filesize  = 0;
	char ich = '\0';

	//filesize = filesize_bytes(filesize, post);
	filesize = get_size(source);
	fd = open("disk", O_RDWR);

	initialize_global_variables(fd);

	ich = check_file(fd, filename);
	if(ich == 'y')
	{
		printf("File with this name is already there\n");
		return;
	}
	
	//create bmap
	bmap = get_bmap(fd);

	first_block = check_bits_availability(filesize);
	printf("First block = %d\n",first_block);
	
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

	total_blocks_to_write = arr[0];
	write_parent(fd, first_block, arr[arr_size - 2], arr_size - 2);
//	printf("cnt = %d, bef = %d, no_of_blocks = %lld\n",count_bits, bef, no_of_blocks);
	
	arr = copy_arr(brr,arr_size);
	int a = arr[0];
//	printf("First_block = %d\n",first_block);
	create_file(fd, source, first_block, arr[arr_size - 2], arr_size - 2);
	
//	printf("a = %d, Counts = %d\n",a, counts);
	write_superblock(fd);
//	read_superblock(fd);

	//print_bmap();
	
	write_bitmap(fd);

	close(fd);

	return;
}

/*
int main(int argc, char **argv)
{
	//./a.out fname sname
	write_file(argv[1], argv[2]);
	
	return 0;
}
*/
