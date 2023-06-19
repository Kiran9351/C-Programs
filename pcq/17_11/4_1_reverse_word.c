

#include<stdio.h>
#include<stdlib.h>

char* reverse_word(char *str)
{
	int start = 0, end = 0;
	char ch = '\0';

	while(str[end] != '\0')
	{
		end++;
	}
	
	end--;
	
	while(start < end)
	{
		ch = str[start];
		str[start] = str[end];
		str[end] = ch;
	}
	
	return str;
}

int main()
{
	char *str = (char*)malloc(sizeof(char) * 100);

	printf("Enter word : ");
	scanf(" %[^\n]s",str);
	
	printf("%s\n",str);
	
	str = reverse_word(str);

	printf("%s\n",str);

	return 0;
}
