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
		for(int i=0;i<10;++i)
		{
			if (str[i] > s.str[i]) return true;
			if (str[i] < s.str[i]) return false;
		}
	}
};
long long merge(mystring* A,long long left,long long cut,long long right)
{
	long long x = cut - left + 1, y = right - cut;
	long long ret = 0;
	mystring *L = new mystring[x + 1];
	mystring *R = new mystring[y + 1];
	for (long long i = 0; i < x; ++i) L[i] = A[left + i];
	for (long long i = 0; i < y; ++i) R[i] = A[cut + i + 1];
	L[x].str = "~~~~~~~~~~";
	R[y].str = "~~~~~~~~~~";
	long long i = 0, j = 0;
	for(long long t=left;t<=right;++t)
	{
		if (L[i] > R[j])
		{
			A[t] = R[j];
			ret += (x - i);
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
	return ret;
}
void mergersort(mystring* A,long long start,long long end,long long &ret)
{
	if (start < end)
	{
		long long cut = (start + end) / 2;
		mergersort(A, start, cut, ret);
		mergersort(A, cut + 1, end, ret);
		ret += merge(A, start, cut, end);
	}
}
int main()
{
	long long n;
	cin >> n;
	mystring*strings = new mystring[n];
	for(int i=0;i<n;++i)
	{
		cin >> strings[i].str;
	}
	cout << "wo yi yue du guan yu chao xi de shuo ming" << endl;
	long long ret = 0;
	mergersort(strings, 0, n - 1, ret);
	cout << ret;
}