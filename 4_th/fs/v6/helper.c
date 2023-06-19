#include"headers.c"

void initialize_global_variables(int fd)
{
	SB *sb = (SB*)malloc(sizeof(SB));
	lseek(fd, 0, SEEK_SET);

	read(fd, sb, sizeof(SB));

	disksize = sb -> disk_size;
	blocksize = sb -> block_size;
	no_of_fmd_blocks = sb -> no_of_fmd_blocks;
	no_of_blocks = sb -> no_of_blocks;
	no_of_fmds = sb -> no_of_fmds;
	no_of_flag_bytes = sb -> no_of_flag_bytes;
	no_of_files = sb -> no_of_files;
/*	
	printf("Disk size = %lld\n",sb -> disk_size);
	printf("Block size = %d\n",sb -> block_size);
	printf("No of fmd blocks = %d\n",sb -> no_of_fmd_blocks);
	printf("No of blocks = %lld\n",sb -> no_of_blocks);
	printf("No of fmds = %d\n",sb -> no_of_fmds);
	printf("No of flag bytes = %d\n\n",sb -> no_of_flag_bytes);
*/

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
	
	progress = 0;
	per = 10;

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
