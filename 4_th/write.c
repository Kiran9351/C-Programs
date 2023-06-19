#include<stdio.h>
#include<fcntl.h>
#include<sys/types.h>
#include<stdlib.h>
#include<unistd.h>

void write_to_file(int fd)
{
	char ch = '\0';
	
	char buff[] = "Hello world\n";
	while(ch != 'q')
	{
		
		int c = write(fd, buff, sizeof(buff)-1);
		printf("c = %d\n",c);
		scanf(" %c",&ch);
	}

	return;
}

int main()
{
	int fd = open("check.txt", O_CREAT | O_RDWR, 00777);

	write_to_file(fd);

	close(fd);

	return 0;
}
