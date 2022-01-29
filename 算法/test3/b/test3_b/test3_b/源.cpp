#include<iostream>
#include<string>
using namespace std;
class mystring
{
public:
	string str;
	mystring()
	{
		;
	}
	bool operator >(const mystring &s)
	{
		unsigned len = str.length();
		bool f = 0;
		if (len > s.str.length()) { len = s.str.length(); f = 1; }
		for (int i = 0; i < len; ++i)
		{
			if (str[i] > s.str[i]) return true;
			if (str[i] < s.str[i]) return false;
		}
		if (f) return true;
		else return false;
	}
};
void merge(mystring* A, long long left, long long cut, long long right)
{
	long long x = cut - left + 1, y = right - cut;
	mystring *L = new mystring[x + 1];
	mystring *R = new mystring[y + 1];
	for (long long i = 0; i < x; ++i) L[i] = A[left + i];
	for (long long i = 0; i < y; ++i) R[i] = A[cut + i + 1];
	L[x].str = "~~~~~~~~~~";
	R[y].str = "~~~~~~~~~~";
	long long i = 0, j = 0;
	for (long long t = left; t <= right; ++t)
	{
		if (L[i] > R[j])
		{
			A[t] = R[j];
			j++;
		}
		else
		{
			A[t] = L[i];
			i++;
		}
	}
	delete[]L;
	delete[]R;
}
void mergersort(mystring* A, long long start, long long end)
{
	if (start < end)
	{
		long long cut = (start + end) / 2;
		mergersort(A, start, cut);
		mergersort(A, cut + 1, end);
		merge(A, start, cut, end);
	}
}
int main()
{
	long long n;
	int k;
	cin >> n >> k;
	int need = 2 * k;
	mystring*strings = new mystring[n];
	for (int i = 0; i < n; ++i)
	{
		cin >> strings[i].str;
	}
	long long ret = 0;
	mergersort(strings, 0, n - 1);
	int mid = (n - 1) / 2;
	int l = mid - k;
	int r = mid + k;
	while(l<=r)
	{
		cout << strings[l].str << " ";
		l++;
	}
	delete[]strings;
	return 0;
}