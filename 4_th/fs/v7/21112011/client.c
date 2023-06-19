
/*

This is code to write a request/command to request file.

*/


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>


int get_length(char *buff)
{
	int i = 0;
	char ch = 'a';

	while(ch != '\0')
	{
		ch = buff[i];
		i = i + 1;
	}

	buff[i] = '\n';

	return i;
	
}

void write_request(int fd)
{
	char buff[50]; 

	scanf(" %[^\n]s",buff);
	
	int len = get_length(buff);
	
	lseek(fd, 0, SEEK_END);
	write(fd, buff, len + 1); 
	
	return;
}

int main()
{
	int fd = 0;

	fd = open("request.txt", O_RDWR);

	write_request(fd);

	close(fd);

	return 0;
}
