#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

void read_and_write(int fd_r, int fd_w)
{
	char buff[100], ch = '\0';
	int i = 0;

	while((read(fd_r, &ch, 1) > 0) && (ch != '\n') && (i != 100))
	{
		buff[i] = ch;
		i = i + 1;
	}

	if(strcmp(buff, "Hello") == 0)
	{
		strcpy(buff, "Hello, how are you!\0\n");
	}
	
	printf("%ld",strlen(buff));
	
	write(fd_w, buff, strlen(buff));

	return ;
}

int main()
{
	int fd_r = 0, fd_w = 0;

	fd_r = open("e_to_b.txt", O_RDONLY);
	fd_w = open("b_to_m.txt", O_CREAT | O_RDWR, 00700);
	
	read_and_write(fd_r, fd_w);
	
	close(fd_r);
	close(fd_w);
	
	return 0;
}
