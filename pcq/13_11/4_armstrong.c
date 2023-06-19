
#include<stdio.h>
#include<stdbool.h>

bool armstrong(int n)
{
	int digit = 0, sum = 0, temp = n;

	while(n != 0)
	{
		digit = n % 10;

		sum = sum + (digit * digit * digit);

		n = n / 10;
	}

	if(temp == sum)
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
	int n = 0;
	bool bRet = false;

	printf("Enter number : ");
	scanf("%d",&n);

	bRet = armstrong(n);
	if(bRet == true)
	{
		printf("%d is armstrong number\n",n);
	}
	else
	{
		printf("%d is not armstrong number\n",n);
	}

	return 0;
}
