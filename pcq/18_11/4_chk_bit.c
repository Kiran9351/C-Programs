

#include<stdio.h>
#include<stdbool.h>

bool chk_bit(int val)
{
	int n = 0, a = 1;

	printf("Enter bit no: ");
	scanf("%d",&n);
	n = a << n;
	if(n == (val & n))
	{
		return true;
	}	
	
	return false;

	/*

	if(val = (val & ~(a << n)))
	{
		return false;
	}	
	
	return true;
	
	*/
}

int main()
{

	int val = 0;
	bool bRet = false;

	printf("Enter num: ");
	scanf("%d",&val);

	bRet = chk_bit(val);
	if(bRet == true)
	{
		printf("Bit is set\n");
	}
	else
	{
		printf("Bit is not set\n");
	}
	
	return 0;
}
