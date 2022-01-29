#include<iostream>
using namespace std;
int partition(int* vals,int l,int r)
{
	int pivot = vals[r];
	int i = l - 1;
	for (int j = l; j <= r; ++j)
	{
		if (vals[j] < pivot)
		{
			i++;
			int temp = vals[i];
			vals[i] = vals[j];
			vals[j] = temp;
		}
	}
	int t = vals[i + 1];
	vals[i + 1] = vals[r];
	vals[r] = t;
	return i + 1;
}
void quicksort(int* vals,int start,int end)
{
	if (start < end)
	{
		int q = partition(vals, start, end);
		quicksort(vals, start, q - 1);
		quicksort(vals, q, end);
	}
}
int main()
{
	int n, target;
	int nums = 0;
	cin >> n >> target;
	int *vals = new int[n];
	for (int i = 0; i < n; ++i) cin >> vals[i];
	quicksort(vals, 0, n-1);
	for (int i = 0; i < n; ++i)
	{
		int need = target - vals[i];
		int left = i + 1;
		int right = n - 1;
		while (left < right)
		{
			int val = vals[left] + vals[right];
			if (val == need)
			{
				nums++;
				left++; right--;
			}
			else if (val < need)
			{
				left++;
			}
			else
				right--;
		}
	}
	cout << "wo yi yue du guan yu chao xi de shuo ming" << endl;
	cout << nums;
	system("pause");
	return 0;
}