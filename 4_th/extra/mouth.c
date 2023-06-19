#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

void speak(int fd_r)
{
	char buff[100], ch = '\0';

	int i = 0;
	
	while((read(fd_r, &ch, 1) > 0) && (ch != '\n') && (i < 100))
	{
		buff[i] = ch;
		i = i + 1;
	}

	printf("%s",buff);

	return;
}

int main()
{
	int fd_r = 0;

	fd_r = open("b_to_m.txt", O_RDONLY);
	
	speak(fd_r);
	
	close(fd_r);
	
	return 0;
}
