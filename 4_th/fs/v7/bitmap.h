
/*

These are functions related with bitmap and operations on bitmap.

*/

unsigned char* get_bmap(int fd)
{
	lseek(fd, blocksize + (no_of_fmd_blocks * blocksize), SEEK_SET);
	unsigned char *buf = (unsigned char*)malloc(sizeof(unsigned char) * no_of_flag_bytes);

//	printf("get bmap = %d\n",no_of_flag_bytes);
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

int check_bits_availability(int filesize)
{
	int i = 0, first_block = -1, blocks_need = 0;
	
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

	return first_block;
	
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
				bit_arr[n] = (i * 8) + (7 - j);
				bmap[i] = bmap[i] & (~(1 << j));
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
