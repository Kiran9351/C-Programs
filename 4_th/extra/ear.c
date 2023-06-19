#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void listen(int fd)
{
	char buff[100];

	scanf(" %[^\n]s", buff);
	
	int i = 0;

	while(buff[i] != '\0')
	{
		i = i + 1;
	}
	
	buff[i] = '\n';

	write(fd, buff, strlen(buff));

	return;
}

int main()
{	
	int fd = 0;

	fd = open("e_to_b.txt", O_CREAT | O_RDWR, 00700);
	
	listen(fd);
	
	close(fd);

	return 0;
}
