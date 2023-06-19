
/*

This is code to get list of filename along with filesize and first block no.

*/

void file_names(int fd, int fdest)
{
	int total_fmds = 0, i = 0;
	char buff[] = "Filename\tfilesize\tFirst block\n", a = '\n';

	lseek(fd, blocksize, SEEK_SET);

	total_fmds = no_of_fmd_blocks * (blocksize / sizeof(FMD));
		
	FMD *fmd = (FMD*)malloc(sizeof(FMD));
	write(fdest, buff, sizeof(buff)-1);

	while(i < total_fmds)
	{
		read(fd, fmd, sizeof(FMD));
		if(fmd -> flag != 0)
		{
			write_fmd_to_dest(fdest, fmd);
			write(fdest, &a, 1);
		}
		
		i = i + 1;
	} 

	return ;
}

void get_file_names(char *dest)
{
	int fd = 0, fdest = 0;

	fd = open("disk", O_RDWR);
	fdest = open(dest, O_CREAT | O_RDWR, 00600);

	initialize_global_variables(fd);

	file_names(fd, fdest);	

	close(fd);
	close(fdest);

	return;
}
