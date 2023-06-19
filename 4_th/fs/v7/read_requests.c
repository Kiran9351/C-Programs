
/*

This code take requests from request.txt file and call the function accordingly.

*/

#include"headers.h"
#include"global_variables.h"
#include"operations.h"

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

char** tokenise(char *buff)
{
	char **tokens = NULL;
	int val = 5, i = 0, j = 0, k = 0;

	tokens = (char **)malloc(sizeof(char *) * val);
	
	for(i = 0; i < val; i++)
	{
		tokens[i] = (char*)malloc(sizeof(char) * 10);
	}

	i = 0;
	j = 0;
	k = 0;

	while(buff[i] != '\0')
	{
		if(buff[i] == ' ')
		{
			tokens[j][k] = '\0';
			j = j + 1;
			k = 0;
		}
		else
		{
			tokens[j][k] = buff[i];
			k = k + 1;
		}

		i = i + 1;
	}

	tokens[j][k] = '\0';
	tokens[j+1] = NULL;

	return tokens;
}

void print_tokens(char **tokens)
{
	int i = 0;

	while(tokens[i] != NULL)
	{
		printf("%s\n", tokens[i]);
		i = i + 1;
	}

	return;
}

char compare_first_word(char *word)
{
	char ch = '\0';

	if(word[0] == 'r' || word[0] == 'R')
	{
		ch = 'r';		
	}
	else if(word[0] == 'w' || word[0] == 'W')
	{
		ch = 'w';
	}
	else if(word[0] == 's' || word[0] == 'S')
	{
		ch = 's';
	}
	else if(word[0] == 'g' || word[0] == 'G')
	{
		ch = 'g';
	}
	else if(word[0] == 'd' || word[0] == 'D')
	{
		ch = 'd';
	}
	else
	{
		ch = '\0';
	}

	return ch;
}

void call_function(char ch, char **tokens)
{
	if(ch == 'r')
	{
		read_file(tokens[1], tokens[2]);
	}
	else if(ch == 'w')
	{
		write_file(tokens[1], tokens[2]);
	}
	else if(ch == 's')
	{
		get_file_info(tokens[1], tokens[2]);
	}
	else if(ch == 'g')
	{
		get_file_names(tokens[2]);
	}
	else
	{
		delete_file(tokens[1]);
	}

	return;
} 

int length(char *buff)
{
	int len = 0;

	while(buff[len] != '\0')
	{
		len = len + 1;
	}	

	return len;
}

void read_request_file(int fr, int fd)
{
        char *buff = NULL, **tokens = NULL;
    
        int iret = 0, cur_loc = 0, end_loc = 0, sp = 0; 

        cur_loc = lseek(fr, 0, SEEK_END);
        end_loc = lseek(fr, 0, SEEK_END);
        lseek(fr, 0, SEEK_END); 

        while(1)
        {
                if(cur_loc != end_loc)
                {
                        buff = rd_line(fr);
                        //printf("%s\n",buff);
                	tokens = tokenise(buff);
			//print_tokens(tokens);
		        char ret = compare_first_word(tokens[0]);
                       
			if(ret != '\0')
                        {
                                call_function(ret, tokens);
                        }
			
			strcat(buff, " - done\n");
			int len = length(buff);
			write(fd, buff, len);
			printf("\n%s",buff);
                }

                cur_loc = lseek(fr, 0, SEEK_CUR);
                end_loc = lseek(fr, 0, SEEK_END);
                sp = lseek(fr, 0, SEEK_SET);
                lseek(fr, cur_loc - sp, SEEK_SET);
        }

        return ;
}

int main()
{
	int fr = 0, fd = 0;

	fr = open("request.txt", O_CREAT | O_RDWR, 00600);
	fd = open("result.txt", O_CREAT | O_RDWR, 00600);

	read_request_file(fr, fd);

	return 0;
}
