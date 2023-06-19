

#include<stdio.h>
#include<stdbool.h>

bool chk_substring(char *str, char *substring)
{
	int i = 0, j = 0;

	while (str[i] != '\0' && substring[j] != '\0')
	{
		if(str[i] == substring[j])
		{
			j++;
		}
		else
		{
			j = 0;
		}
		i++;
	}

	if(substring[j] == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
	char str[100], substring[20];
	bool bRet =false;
	
	printf("Enter string : ");
	scanf(" %[^\n]s",str);

	printf("Enter substring : ");
	scanf(" %[^\n]s",substring);

	bRet = chk_substring(str,substring);

	if(bRet == true)
	{
		printf("'%s' is substring of '%s'\n",substring,str);
	}
	else
	{
		printf("'%s' is not substring of '%s'\n",substring,str);
	}

	return 0; 
}
