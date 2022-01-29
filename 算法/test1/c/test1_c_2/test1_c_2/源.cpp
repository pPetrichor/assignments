#include<iostream>
using namespace std;
int binary_search(int a[], int start, int end, int max_, int target)
{
	if (start < end)
	{
		int mid = (start + end) / 2;
		if (a[mid] == target) return target;
		else
		{
			if (a[mid] > target) return binary_search(a, start, mid, max_, target);
			else return binary_search(a, mid+1, end, max_, target);
		}
	}
	else
		return max_;
}
int main()
{
	int n; cin >> n;
	int *_array = new int[n];
	for (int i = 0; i < n; ++i) cin >> _array[i];
	int t; cin >> t;
	long long mymax = _array[n - 1] + 1;
	for(int i=0;i<n;++i)
	{
		int need = binary_search(_array, i, n, mymax, t - _array[i]);
		if (need != mymax) cout << _array[i] << " " << need << endl;
	}
	return 0;
}