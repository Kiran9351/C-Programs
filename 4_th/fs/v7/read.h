
/*

This is implementation to read the file from virtual disk and create another file on physical disk.

*/

int find_file(int fd, char *filename)
{
	int total_fmds = 0, i = 0, first_block = -1;

	lseek(fd, blocksize, SEEK_SET);
	
	total_fmds = no_of_fmd_blocks * (blocksize / sizeof(FMD));

	FMD *fmd = (FMD*)malloc(sizeof(FMD));	

	while(i < total_fmds)
	{
		read(fd, fmd, sizeof(FMD));
		
		if(strcmp(fmd -> file_name, filename) == 0)
		{
			first_block = fmd -> block_no;
//			printf("Filesize= %ld\n",fmd->file_size);
			fs = fmd -> file_size;
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
	read(fd, buff, blocksize);
	
	return buff;
}


void rd_pointer_block(int fd, int fd3, int block_no, int no_of_ints)
{
        int i = 0, *bit_arr = NULL;
	unsigned char* buff = NULL;

        bit_arr = (int*)malloc(sizeof(int) * no_of_ints);
    
        lseek(fd, block_no * blocksize, SEEK_SET);
        read(fd, bit_arr, sizeof(int)*no_of_ints);

        while(i < no_of_ints)
        {
		buff = read_data_block(fd, bit_arr[i]);
		if(fs >= blocksize)
		{
			write(fd3, buff, blocksize);
			fs = fs - blocksize;
		}
		else
		{
			write(fd3, buff, fs);
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

void rd_parent(int fd, int fd3, int block_no, int no_of_ints, int arr_index);

void rd_child(int fd, int fd3, int block_no, int arr_index)
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
                rd_pointer_block(fd, fd3, block_no, no_of_ints);
        }
        else
        {
                rd_parent(fd, fd3, block_no, no_of_ints, arr_index);
        }

        return;
}


void rd_parent(int fd, int fd3, int block_no, int no_of_ints, int arr_index)
{
        int i = 0, *iarr = NULL;

        lseek(fd, block_no * blocksize, SEEK_SET);
        iarr = (int*)malloc(sizeof(int)*no_of_ints);

        read(fd, iarr, sizeof(int)*no_of_ints);

        while(i < no_of_ints)
        {
                rd_child(fd, fd3, iarr[i], arr_index - 1);
                i = i + 1;
        }

        return;
}



void rd_file(int fd, char* dest, int block_no, int no_of_ints, int arr_index)
{
	int fd3 = 0;

	fd3 = open(dest, O_CREAT | O_RDWR, 00600);
	rd_parent(fd, fd3, block_no, no_of_ints, arr_index);

	close(fd3);
	//printf("fs = %d\n",fs);

	return;
}


void read_file(char *filename, char *dest)
{
	int fd = 0, first_block = 0;

	fd = open("disk", O_RDWR);

	initialize_global_variables(fd);

	first_block = find_file(fd, filename);
//	printf("R_first_block = %d\n",first_block);
	
	if(first_block == -1)
	{
		printf("File is not there\n");
		return;
	}
	
	int a = arr[0];
	total_blocks_to_write = arr[0];
	rd_file(fd, dest, first_block, arr[arr_size - 2], arr_size - 2);
//	printf("a = %d, counts = %d\n",a, counts);
	
	close(fd);	

	return;
}
/*
int main(int argc, char** argv)
{
	//./a.out fname dname
	read_file(argv[1], argv[2]);

	return 0;
}*/
