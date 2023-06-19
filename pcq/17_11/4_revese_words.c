

#include<stdio.h>
#include<stdlib.h>

void reverseWords(char *str)
{
	int i = 0, p = 0, q = 0;
	char ch = '\0';

	while(str[i] != '\0')
	{	
		printf("%c ",str[i]);
		while(str[i] == ' ' && str[i] != '\0')
		{
			i++;
		}
	

		p = i;
		q = i;

		while(str[q] != ' ' || str[q] != '\0')
		{
			q++;
		}
		i = q;
		q -= 1;
		while(p < q)
		{
			ch = str[p];
			str[p] = str[q];
			str[q] = ch;	
			p++;
			q--;
		}
	}

}

int main()
{
	char str[100];

	printf("Enter the string : ");
	scanf(" %[^\n]s",str);
	printf("%s\n",str);
	reverseWords(str);
	printf("%s\n",str);
	
	return 0;
}
