

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

bool chk_pow(int val)
{
	int rem = 0;

	while(val != 1 && rem == 0)
	{
		rem = val % 2;
		val = val / 2;

		//printf("rem = %d, val = %d\n",rem,val);
	}
	
	if(rem == 0 && val == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool chk(int val)
{
	val = val & (val - 1);
	if(val == 0)
	{
		return true;
	}
	
	return falsee
}

int main()
{
	bool bRet = false;
	int val = 0;

	printf("Enter the val = ");
	scanf("%d",&val);

	bRet = chk_pow(val);
	if(bRet == true)
	{
		printf("%d is pow of 2\n",val);
	}
	else
	{	
		printf("%d is not pow of 2\n",val);
	}

	bRet = chk(val);
	if(bRet == true)
	{
		printf("%d is power of 2\n",val);
	}
	else
	{
		printf("%d is not power of 2\n",val);
	}

	return 0;
}
