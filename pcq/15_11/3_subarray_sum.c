

#include<stdio.h>
#include<stdlib.h>

int *sub_array(int *arr, int n, int sum)
{
	if(arr == NULL)
	{
		return NULL;
	}

	int i = 0, j = 0, s = 0, *ret = NULL;

	ret = (int*)malloc(2 * sizeof(int));
	ret[0] = -1, ret[1] = -1;

	while(j < n)
	{
		s = s + arr[j];

		while(s > sum)
		{
			s = s - arr[i];
			i++;
		}

		if(s == sum)
		{
			break;
		}

		j++;
	}

	if(s == sum)
	{
		ret[0] = i;
		ret[1] = j;
	}

	return ret;
}

int main()
{
	int n = 0, sum = 0, i = 0, *arr = NULL, *ret = NULL;

	printf("Enter the size of array: ");
	scanf("%d",&n);
	
	arr = (int*)malloc(n * sizeof(int));

	printf("Enter elements of array:\n");
	for(i = 0; i < n; ++i)
	{
		printf("Enter element at index %d : ", i);
		scanf("%d",(arr + i));
	}

	printf("Enter sum : ");
	scanf("%d",&sum);

	ret = sub_array(arr,n,sum);
	if(ret != NULL)
	{
		printf("i = %d, j = %d\n", ret[0],ret[1]);
	}
	
	return 0;
}
