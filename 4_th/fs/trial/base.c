#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>

int add(int a, int b)
{
	printf("a + b = %d\n", a + b);

	return a+b;
}

int sub(int a, int b)
{
	printf("a - b = %d\n", a - b);

	return a-b;
}

int mul(int a, int b)
{
	printf("a * b = %d\n", a * b);

	return a*b;
}

int division(int a, int b)
{
	printf("a / b = %d\n", a/b);

	return a/b;
}

char* rd_line(int fd)
{
	char ch = '\0', *buff = (char*)malloc(sizeof(char)*50);
	int cnt = 0;

	while((ch != '\n') &&  (ch != EOF))
	{
		read(fd, &ch, 1);
		buff[cnt] = ch;
		cnt = cnt + 1;
	}
	
	return buff;
	
}

char compare_buff(char *buff)
{
	char ret = '\0', i = 0;

	if(buff[i] == 'a' || buff[i] == 'A')
	{
		ret = 'a';
	}
	else if(buff[i] == 's' || buff[i] == 'S')
	{
		ret = 's';
	}
	else if(buff[i] == 'm' || buff[i] == 'M')
	{
		ret = 'm';	
	}
	else if(buff[i] == 'd' || buff[i] == 'D')
	{
		ret = 'd';
	}
	else
	{
		ret = '0';
	}

	return ret;
		
}
/*
char** tokenise_buff(char *buff)
{
	int i = 0, j = 0, k = 0;
	

	while(buff[i] != '\n')
	{
		if(buff[i] != ' ')
		{
			tokens[j][k] = buff[i];
			k = k + 1;
		}
		
		if(buff[i] == ' ')
		{
			tokens[j][k] = '\0';
			j = j + 1;
			k = 0;
		}
		
 		i = i + 1;
	}
	
	tokens[j][k] = '\0';
	tokens[j+1][0] = '\0';

	i = 0;
	while(tokens[i][0] != '\0')
	{
		j = 0;
		while(tokens[i][j] != '\0')
		{
			printf("%c",tokens[i][j]);
			j = j + 1;
		}

		i = i + 1;
		
		printf("\n\n");
	}
	
	return (char **)tokens;
}
*/
int call_function(char ret, char*buff)
{
	//char **tokens = tokenise_buff(buff);
	
	char *command = NULL, num1[5], num2[5];
	int i = 0, j = 0, iret = 0;

	while(buff[i] != ' ')
	{
		i = i + 1;
	}

	i = i + 1;
	while(buff[i] != ' ')
	{
		num1[j] = buff[i];
		i = i + 1;
		j = j + 1; 
	}
	num1[j] = '\0';

	j = 0;
	i = i + 1;

	while(buff[i] != '\n')
	{
		num2[j] = buff[i];
		i = i + 1;
 		j = j + 1;
	}

	num2[j] = '\0';

	if(ret == 'a')
	{
		iret = add(atoi(num1), atoi(num2));
	}
	else if(ret == 's')
	{
		iret = sub(atoi(num1), atoi(num2));
	}
	else if(ret == 'm')
	{
		iret = mul(atoi(num1), atoi(num2));
	}
	else 
	{
		iret = division(atoi(num1), atoi(num2));
	}

	return iret;
}

void read_request_file()
{
	char *buff = NULL;
	
	int fd = open("request.txt", O_RDONLY), iret = 0, cur_loc = 0, end_loc = 0, sp = 0;

	cur_loc = lseek(fd, 0, SEEK_SET);
	end_loc = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);	

	while(1)
	{
		if(cur_loc != end_loc)
		{
			buff = rd_line(fd);
			//printf("%s\n",buff);
			char ret = compare_buff(buff);
			if(ret != '\0')
			{
				iret = call_function(ret, buff);
			}

			printf("%s = %d\n",buff, iret);
		}

		cur_loc = lseek(fd, 0, SEEK_CUR);
                end_loc = lseek(fd, 0, SEEK_END);
                sp = lseek(fd, 0, SEEK_SET);
                lseek(fd, cur_loc - sp, SEEK_SET);
	}

	return ;
}

int main()
{
	read_request_file();
	
	return 0;
}
