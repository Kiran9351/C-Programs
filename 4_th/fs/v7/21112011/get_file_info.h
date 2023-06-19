
/*

These functions helps to get complete file information including block nos. which are used to write a file.

*/

void rpointer_block(int fd, int fdest, int block_no, int no_of_ints)
{
        int i = 0, *bit_arr = NULL;

        bit_arr = (int*)malloc(sizeof(int) * no_of_ints);
        
        lseek(fd, block_no * blocksize, SEEK_SET);
        read(fd, bit_arr, sizeof(int)*no_of_ints);
	char ch = '\t', no[32] = "", nl = '\n';

	char a[] = "Level 0 : Data blocks are :\n\n";	
	write(fdest, a, sizeof(a)-1);
      	
	while(i < no_of_ints)
	{	
		int val = bit_arr[i];
		sprintf(no, "%d", val);
		write(fdest, no, strlen(no));
		write(fdest, &ch , 1);
	//	printf("%d\t", bit_arr[i]);

		i = i + 1;
      	}

	write(fdest, &nl, 1);

//    	printf("\n");

        return;
}

void rparent(int fd, int fdest, int block_no, int no_of_ints, int arr_index);

void rchild(int fd, int fdest, int block_no, int arr_index)
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
                rpointer_block(fd, fdest, block_no, no_of_ints);
        }
        else
        {
                rparent(fd, fdest, block_no, no_of_ints, arr_index);
        }

        return;
}

void rparent(int fd, int fdest, int block_no, int no_of_ints, int arr_index)
{
        int i = 0, *iarr = NULL;
	char no[32] = "", s = ' ', tab = '\t', col = ':', nl = '\n', level[] = "LEVEL ", no2[32] = "";

        lseek(fd, block_no * blocksize, SEEK_SET);
        iarr = (int*)malloc(sizeof(int)*no_of_ints);
        
        read(fd, iarr, sizeof(int)*no_of_ints);
        
        while(i < no_of_ints)
        {
		for(int j = 0; j < (arr_size - 2) - arr_index; j++) 
		{
			write(fdest, &tab, 1);
		}

		write(fdest, level, strlen(level));
		int val = arr_index;
		sprintf(no2, "%d", val);
		write(fdest, no2, strlen(no2));
		write(fdest, &s, 1);

		val = iarr[i];
		sprintf(no, "%d", val);
		
		write(fdest, no, strlen(no));
		write(fdest, &col, 1);
		write(fdest, &nl, 1);		
	
                rchild(fd, fdest, iarr[i], arr_index - 1);
                i = i + 1;
        }

        return;
}

FMD *get_file(int fd, char *filename)
{
	int total_fmds = 0, i = 0, ch = 0;

	FMD *fmd = (FMD*)malloc(sizeof(FMD));

	lseek(fd, blocksize, SEEK_SET);
	
	total_fmds = no_of_fmd_blocks * (blocksize / sizeof(FMD));

	while(i < total_fmds)
	{
		read(fd, fmd, sizeof(FMD));

		if(strcmp(fmd -> file_name, filename) == 0)
		{
			i = total_fmds;
			ch = 1;
		}

		i = i + 1;
	}

	if(ch == 0)
	{
		fmd = NULL;
	}

	return fmd;
}

void write_fmd_to_dest(int fdest, FMD *fmd)
{
	char ch = '\t', no[32], nl = '\n';
	long int filesize = fmd -> file_size; 
	int i = 0;
	int first_block = fmd -> block_no;
	
	while(fmd -> file_name[i] != '\0')
	{
		i = i + 1;
	}

	write(fdest, fmd, i);
	write(fdest, &ch, 1);
	sprintf(no, "%ld", filesize);
	write(fdest, no, strlen(no));
	write(fdest, &ch, 1);
	sprintf(no, "%d", first_block);
	write(fdest, no, strlen(no));
	write(fdest, &nl, 1);

	return;
}


void get_file_info(char *filename, char *dest)
{
	int fd = 0, first_block = 0, fdest = 0;
	fd = open("disk", O_RDWR);
	fdest = open(dest, O_CREAT | O_RDWR, 00600);

	initialize_global_variables(fd);	
	
	FMD *fmd = get_file(fd, filename);
	
	if(fmd == NULL)
	{
		printf("File is not there\n");
	}	
	else
	{
		write_fmd_to_dest(fdest, fmd);
		int bn = generate_arr(fmd -> file_size);
		rparent(fd, fdest, fmd -> block_no, arr[arr_size - 2], arr_size - 2); 
	}

	close(fd);
	close(fdest);
	
	return;
}
