
/*

These are functions to delete file.

*/

void reset_bmap(int block_no)
{	
	int bmap_index = 0,  bit_no= 0;

	bmap_index = block_no / 8;
	bit_no = block_no % 8;

	bmap[bmap_index] = bmap[bmap_index] | (1 << (7 - bit_no));

	return; 
}

void resetpointer_block(int fd, int block_no, int no_of_ints)
{
	int i = 0, *bit_arr = NULL;

	bit_arr = (int*)malloc(sizeof(int) * no_of_ints);

	lseek(fd, block_no * blocksize, SEEK_SET);
	read(fd, bit_arr, sizeof(int) * no_of_ints);

	while(i < no_of_ints)
	{
		reset_bmap(bit_arr[i]);

		i = i + 1;
	}

	progress = progress + no_of_ints;
	per = 0;
	if(progress >= ((total_blocks_to_delete * per)/100))
	{
		printf("--%d%%--",per);
		per = per + 10;
	}

	return;
}

void resetpar(int fd, int block_no, int no_of_ints, int arr_index);

void resetchild(int fd, int block_no, int arr_index)
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
		resetpointer_block(fd, block_no, no_of_ints);
		
		reset_bmap(block_no);
	}
	else
	{
		resetpar(fd, block_no, no_of_ints, arr_index);
	}

	return;
}

void resetpar(int fd, int block_no, int no_of_ints, int arr_index)
{
	int i = 0, *iarr = NULL;

	lseek(fd, block_no * blocksize, SEEK_SET);
	iarr = (int*)malloc(sizeof(int) * no_of_ints);

	read(fd, iarr, sizeof(int) * no_of_ints);
	
	
	reset_bmap(block_no);
	while(i < no_of_ints)
	{
		resetchild(fd, iarr[i], arr_index - 1);
		//need to reset block_no.	
		
		i = i + 1;
	}


	return;
}

void write_updated_fmd(int fd, char *filename)
{
	int total_fmds = 0, i = 0;

        FMD *fmd = (FMD*)malloc(sizeof(FMD));

        lseek(fd, blocksize, SEEK_SET);
    
        total_fmds = no_of_fmd_blocks * (blocksize / sizeof(FMD));

        while(i < total_fmds)
        {
                read(fd, fmd, sizeof(FMD));

                if(strcmp(fmd -> file_name, filename) == 0)
                {
                        i = total_fmds;
                        
			lseek(fd, -(sizeof(FMD)), SEEK_CUR);

			strcpy(fmd->file_name, "\0");
			fmd -> file_size = -1;
			fmd -> block_no  = -1;
			fmd -> flag = 0;

			write(fd, fmd, sizeof(FMD));
                }

                i = i + 1;
        }

}

void delete_file(char *filename)
{
	int fd = 0, first_block = 0;

	fd = open("disk", O_RDWR);

	initialize_global_variables(fd);
	bmap = get_bmap(fd);

	FMD *fmd  = get_file(fd, filename);
	
	if(fmd == NULL)
	{
		printf("%s file is not on disk\n",filename);
	}
	else
	{

		printf("D first block = %d\n",fmd -> block_no);
		int bn = generate_arr(fmd -> file_size);
		resetpar(fd, fmd -> block_no , arr[arr_size - 2], arr_size - 2); 
	}

	write_updated_fmd(fd, filename);
	
	//print_bmap();
	write_bitmap(fd);
	
	return;
}
