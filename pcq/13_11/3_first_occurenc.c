
// First occurence

#include<stdio.h>
#include<stdlib.h>

int first_occ(int *arr, int n, int val)
{
	if(arr == NULL)
	{
		return -1;
	}

	int i = 0;
	while( arr[i] != val && i < n)
	{
		i++;
	}

	if(i == n)
	{
		return -1;
	}
	else
	{
		return i;
	}
}

int main()
{
	int *arr = NULL, n = 0, i = 0, val = 0, ret = 0;

	printf("Enter size of array : ");
	scanf("%d",&n);

	arr = (int*)malloc(n * sizeof(int));

	printf("Enter elements of an array:\n");
	for(i = 0; i < n; ++i)
	{
		printf("Enter element at index %d : ", i);
		scanf("%d",(arr + i));
	}

	printf("Enter element to search : ");
	scanf("%d",&val);

	ret = first_occ(arr,n,val);
	printf("First occ. of %d is : %d", val,ret);

	return 0;
}
