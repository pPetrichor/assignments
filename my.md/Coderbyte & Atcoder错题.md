# Coderbyte & Atcoder错题

#### 1、Longest Word

Have the function LongestWord(**sen**) take the **sen** parameter being passed and return the largest word in the string. If there are two or more words that are the same length, return the first word from the string with that length. Ignore punctuation and assume **sen** will not be empty.

WA:

```C
#include <iostream>
#include <string>
using namespace std;

string LongestWord(string sen) {

  // code goes here
  int n=0;
  string abc,maxing;
  for(int i=0;i<sen.length();i++)
  {
        if((sen[i]<'a'||sen[i]>'z')&&(sen[i]<'A'||sen[i]>'Z'))
      {
          if(abc.length()>maxing.length())
            maxing=abc;
          abc.clear();
          continue;
      }
        abc+=sen[i];
  }
  if(maxing.length() < abc.length())maxing=abc;
  return maxing;

}

int main(void) { 
   
  // keep this function call here
  cout << LongestWord(gets(stdin));
  return 0;
    
}
```

AC:

```C
#include <iostream>
#include <cctype>
#include <cstring>
using namespace std;

string LongestWord(string sen) { 

  // code goes here   
  string sen2 = "";
  for(int i=0;i<sen.length();i++)
  {
    if(isalpha(sen[i])||sen[i]==' '||'0'<=sen[i]&&sen[i]<='9')
    {
      sen2.append(sen.substr(i,1));
    }
  }
  char* sench = (char*)sen2.c_str();
  string longest = "";
  int longestLen = 0;
  for(const char* pch=strtok(sench," ");pch;pch=strtok(NULL," "))
  {
    if(strlen(pch)>longestLen)
    {
      longest = pch;
      longestLen=strlen(pch);
    }
  }
  return longest; 
            
}

int main() { 
  
  // keep this function call here
  cout << LongestWord(gets(stdin));
  return 0;
    
}
```

## Atcoder Beginner Contest 140

#### 2、A - Password

### Problem Statement

Takahashi is going to set a 3-character password.

How many possible passwords are there if each of its characters must be a digit between 1 and N (inclusive)?



### Constraints

- 1≤N≤9
- N is an integer.

WA:

```C++
#include <bits/stdc++.h>
using namespace std;

#define rep(i, n) for (int i = 0; i < (n); i++)
#define MOD 1000000007
typedef long long ll;
typedef unsigned long long ull;

int main() {
  int n;
  cin >> n;
  cout << n * 3;
}
```

AC:

```C++
#include <bits/stdc++.h>
using namespace std;

#define rep(i, n) for (int i = 0; i < (n); i++)
#define MOD 1000000007
typedef long long ll;
typedef unsigned long long ull;

int main() {
  int n;
  cin >> n;
  cout << pow(n, 3);
}
```



#### **3、F - Many Slimes**

### Problem Statement

We have one slime.

You can set the *health* of this slime to any integer value of your choice.

A slime reproduces every second by spawning another slime that has strictly less health. You can freely choose the health of each new slime. The first reproduction of our slime will happen in one second.

Determine if it is possible to set the healths of our first slime and the subsequent slimes spawn so that the multiset of the healths of the 2Nslimes that will exist in N seconds equals a multiset S.

Here SS is a multiset containing 2N (possibly duplicated) integers: S1, S2, ..., S2N



### Constraints

- All values in input are integers.
- 1≤N≤18
- 1≤Si≤10^9

WA:

```
#include <bits/stdc++.h>
using namespace std;
#define _GLIBCXX_DEBUG
#define rep2(i, m, n) for(int i=int(m); i<int(n); i++)
#define rep(i, n) rep2(i, 0, n)
#define all(a) a.begin(), a.end()
using ll = long long;
using ld = long double;
using V = vector<int>;
using Vll = vector<ll>;
using Vld = vector<ld>;
using VV = vector<V>;
using VVll = vector<Vll>;
using VVld = vector<Vld>;
using P = pair<int, int>;
using Pll = pair<ll, ll>;
using Pld = pair<ld, ld>;
const int INF = 1<<30;
const ll INFll = 1ll<<62;
const ld EPS = 1e-10;
const int MOD = int(1e9)+7;
template<typename T> inline bool chmin(T& a, T b) {if(a>b) {a=b; return true;} return false;}
template<typename T> inline bool chmax(T& a, T b) {if(a<b) {a=b; return true;} return false;}


int main() {
  ll n; cin >> n;
  ll m = 1ll<<n;
  V s(m);
  rep(i, m) cin >> s[i];
  sort(all(s));
  reverse(all(s));

  V ind;
  ind.push_back(0);
  s[0] = -1;

  rep(i, n) {
    sort(all(ind));
    int j = 0;
    for (auto e : ind) {
      while (j < m && (s[j] < 0 || s[e] <= s[j])) j++;
      if (j == m) {
        cout << "No" << endl;
        return 0;
      }
      ind.push_back(j);
      s[j] = -1;      
    }
  }
  cout << "Yes" << endl;
  return 0;
}
```

AC:

```
#include <bits/stdc++.h>
using namespace std;
#define _GLIBCXX_DEBUG
#define rep2(i, m, n) for(int i=int(m); i<int(n); i++)
#define rep(i, n) rep2(i, 0, n)
#define all(a) a.begin(), a.end()
using ll = long long;
using ld = long double;
using V = vector<int>;
using Vll = vector<ll>;
using Vld = vector<ld>;
using VV = vector<V>;
using VVll = vector<Vll>;
using VVld = vector<Vld>;
using P = pair<int, int>;
using Pll = pair<ll, ll>;
using Pld = pair<ld, ld>;
const int INF = 1<<30;
const ll INFll = 1ll<<62;
const ld EPS = 1e-10;
const int MOD = int(1e9)+7;
template<typename T> inline bool chmin(T& a, T b) {if(a>b) {a=b; return true;} return false;}
template<typename T> inline bool chmax(T& a, T b) {if(a<b) {a=b; return true;} return false;}


int main() {
  ll n; cin >> n;
  ll m = 1ll<<n;
  V s(m);
  rep(i, m) cin >> s[i];
  sort(all(s));
  reverse(all(s));

  vector<bool> used(m, false);
  set<int> ind;
  used[0] = true;
  ind.insert(0);

  rep(i, n) {
    int j = 0;
    for (auto e : ind) {
      for (; j < m; j++) {
        if (used[j]) continue;
        if (s[e] <= s[j]) continue;
        used[j] = true;
        break;
      }
      if (j == m) {
        cout << "No" << endl;
        return 0;
      }
    }
    rep(j, m) {
      if (used[j]) ind.insert(j);
    }
  }
  cout << "Yes" << endl;
  return 0;
}
```

#### 4、**C - Maximal Value**

### Problem Statement

There is an integer sequence A of length N whose values are unknown.

Given is an integer sequence B of length N−1 which is known to satisfy the following:

Bi≥max(Ai,Ai+1)

Find the maximum possible sum of the elements of A.



### Constraints

- All values in input are integers.
- 2≤N≤100
- 0≤Bi≤10^5

WA:

```
#include <bits/stdc++.h>
#define rep(i,n) for (int i=0; i<(n); ++i)
using namespace std;
using ll=long long;
int main(){
    int n;
    cin>>n;
    vector<int>b(n-1);
    rep(i,n-1){
        cin>>b[i];
    }
    vector<int>a(n);
    a[0]=b[0];
    rep(i,n){
        a[i]=min(b[i-1],b[i]);
    }
    int ans=0;
    rep(i,n){
        ans+=a[i];
    }
    cout<<ans<<endl;
}
```

AC:

```
#include <bits/stdc++.h>
#define rep(i,n) for (int i=0; i<(n); ++i)
using namespace std;
using ll=long long;
int main(){
    int n;
    cin>>n;
    vector<int>b(n-1);
    rep(i,n-1){
        cin>>b[i];
    }
    vector<int>a(n);
    for(int i=1; i<n-1; ++i){
        a[i]=min(b[i-1],b[i]);
    }
    a[0]=b[0];
    a[n-1]=b[n-2];
    long ans=0;
    rep(i,n){
        ans+=a[i];
    }
    cout<<ans<<endl;
}
```

#### 5、B-Buffet

**Problem Statement**

Takahashi went to an all-you-can-eat buffet with N kinds of dishes and ate all of them (Dish 11, Dish 22, ……, Dish N) once.

The ii-th dish (1≤i≤N) he ate was Dish Ai.

When he eats Dish ii (1≤i≤N), he gains Bi satisfaction points.

Additionally, when he eats Dish i+1 just after eating Dish ii (1≤i≤N−1), he gains Cimore satisfaction points.

Find the sum of the satisfaction points he gained.

### Constraints

- All values in input are integers.
- 2≤N≤20
- 1≤Ai≤N
- A1,A2,...,AN are all different.
- 1≤Bi≤50
- 1≤Ci≤50

WA:

```
#include <bits/stdc++.h>
using namespace std;
struct Fast {
    Fast() {
        cin.tie(0);
        ios::sync_with_stdio(false);
        cout << fixed << setprecision(20);
    }
} fast;

int main() {
    long a, b, n, ans = 0;
    cin >> n;
    vector<long> as(n);
    vector<long> bs(n);
    vector<long> cs(n - 1);
    for (int i = 0; i < n; i++) {
        cin >> as[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> bs[i];
        ans += bs[i];
    }
    for (int i = 0; i < n - 1; i++) {
        cin >> cs[i];
    }
    for (int i = 0; i < n - 1; i++) {
        if (as[i] + 1 == as[i + 1]) {
            ans += cs[i - 1];
        }
    }
    cout << ans << endl;
}
```

AC:

```
#include <bits/stdc++.h>
using namespace std;
struct Fast {
    Fast() {
        cin.tie(0);
        ios::sync_with_stdio(false);
        cout << fixed << setprecision(20);
    }
} fast;

int main() {
    long a, b, n, ans = 0;
    cin >> n;
    vector<long> as(n);
    vector<long> bs(n);
    vector<long> cs(n - 1);
    for (int i = 0; i < n; i++) {
        cin >> as[i];
    }
    for (int i = 0; i < n; i++) {
        cin >> bs[i];
        ans += bs[i];
    }
    for (int i = 0; i < n - 1; i++) {
        cin >> cs[i];
    }
    for (int i = 0; i < n - 1; i++) {
        if (as[i] + 1 == as[i + 1]) {
            ans += cs[as[i] - 1];
        }
    }
    cout << ans << endl;
}
```

#### 6、**D - Face Produces Unhappiness**

**Problem Statement**

There are  N people standing in a queue from west to east.

Given is a string S of length  N representing the directions of the people. The  i -th person from the west is facing west if the  i -th character of SS S is `L`, and east if that character of SS S is `R`.

A person is happy if the person in front of him/her is facing the same direction. If no person is standing in front of a person, however, he/she is not happy.

You can perform the following operation any number of times between  0 and K (inclusive):

Operation: Choose integers  l and  r such that 1≤l≤r≤N , and rotate by 180 degrees the part of the queue: the  l -th,  (l+1) -th, ...... ... , r -th persons. That is, for each i=0,1,...,r−l, the (l + i) -th person from the west will stand the(r - i) -th from the west after the operation, facing east if he/she is facing west now, and vice versa.What is the maximum possible number of happy people you can have?

**Constraints**

N is an integer satisfying 1≤N≤10^5

K is an integer satisfying 1≤K≤10^5

|S|=N

 Each character of  S is `L` or `R`.

WA:

```
#include<bits/stdc++.h>
using namespace std;
int n,k,ans;
string s;
int main(){
    cin>>n>>k>>s;
    for(int i=1;i<=n;i++)
        if(s[i]==s[i-1]&&s[i]=='L') ans++;
    for(int i=n;i>=1;i--)
        if(s[i]==s[i+1]&&s[i]=='R') ans++;
    ans=min(n-1,ans+2*k);
    cout<<ans;
}
```

AC:

```
#include<bits/stdc++.h>
using namespace std;
int n,k,ans;
char s[100005];
int main(){
    cin>>n>>k;
    for(int i=1;i<=n;i++) cin>>s[i];
    for(int i=1;i<=n;i++)
        if(s[i]==s[i-1]&&s[i]=='L') ans++;
    for(int i=n;i>=1;i--)
        if(s[i]==s[i+1]&&s[i]=='R') ans++;
    ans=min(n-1,ans+2*k);
    cout<<ans;
}
```

## AtCoder Beginner Contest 158

#### 7、**A - Station and Bus**

### Problem Statement

In AtCoder City, there are three stations numbered 1, 2, and 3.

Each of these stations is operated by one of the two railway companies, A and B. A string S of length 3 represents which company operates each station. If Si is `A`, Company A operates Station ii; if Si is `B`, Company B operates Station i.

To improve the transportation condition, for each pair of a station operated by Company A and one operated by Company B, there will be a bus service connecting them.

Determine if there is a pair of stations that will be connected by a bus service.



### Constraints

- Each character of S is `A` or `B`.
- |S|=3

WA:

```
#include <bits/stdc++.h>
using namespace std;

string s;

int main() {
	if (s[0] == s[1] && s[1] == s[2])
    cout << "No" << endl;
  else
    cout << "Yes" << endl;
}
```

AC:

```
#include <bits/stdc++.h>
using namespace std;


int main() {
    string s;
    cin >> s;
    int a=0;
    int b=0;
    for(int i=0;i<3;i++){
        if('A'==s[i]){
            a++;
        }else if('B'==s[i]){
            b++;
        }
    }
    if(a==3||b==3){
        cout << "No" << endl;
        return 0;
    }else{
        cout << "Yes" << endl;
    }
    return 0;
}
```

#### 8、**B - Count Balls**

### Problem Statement

Takahashi has many red balls and blue balls. Now, he will place them in a row.

Initially, there is no ball placed.

Takahashi, who is very patient, will do the following operation 10^100 times:

- Place A blue balls at the end of the row of balls already placed. Then, place B red balls at the end of the row.

How many blue balls will be there among the first N balls in the row of balls made this way?



### Constraints

- 1≤N≤10^18
- A,B≥0
- 0<A+B≤10^18
- All values in input are integers.

WA:

```
#include <utility>
#include <map>
#include <string>
#include <math.h>
using namespace std;
typedef long long ll;


int main() {

    int N,A,B;
    cin >> N >> A >> B;
    int ans = 0;
    
    ans = (N/(A+B))*A;
    if(N%(A+B)>A){
        ans += A;
    }else{
        ans += N%(A+B);
    }
    cout << ans << endl;
    return 0;
}
```

AC:

```
#include <iostream>
#include <algorithm>
#include <stack>
#include <queue>
#include <vector>
#include <utility>
#include <map>
#include <string>
#include <math.h>
using namespace std;
typedef long long ll;


int main() {

    ll N,A,B;
    cin >> N >> A >> B;
    ll ans = 0;
    
    ans = (N/(A+B))*A;
    if(N%(A+B)>A){
        ans += A;
    }else{
        ans += N%(A+B);
    }
    cout << ans << endl;
    return 0;
}
```

#### 9、**C - Tax Increase**

### Problem Statement

Find the price of a product before tax such that, when the consumption tax rate is 8 percent and 10 percent, the amount of consumption tax levied on it is A yen and B yen, respectively. (Yen is the currency of Japan.)

Here, the price before tax must be a positive integer, and the amount of consumption tax is rounded down to the nearest integer.

If multiple prices satisfy the condition, print the lowest such price; if no price satisfies the condition, print `-1`.



### Constraints

- 1≤A≤B≤100
- A and B are integers.

WA:

```
#include<bits/stdc++.h>
using namespace std;

int main()
{
    int a,b;
    cin>>a>>b;

    for(int i = 1;i<=1009;i++)
    {
        int na = a * i;
        int nb = b * i;
        if(a == na && b == nb)
        {
            cout<<i<<endl;
            return 0;
        }
    }

    cout<<-1<<endl;
    return 0;

}
```

AC:

```
#include<bits/stdc++.h>
using namespace std;

int main()
{
    int a,b;
    cin>>a>>b;

    for(int i = 1;i<=1009;i++)
    {
        int na = i * 8/100;
        int nb = i * 10/100;
        if(a == na && b == nb)
        {
            cout<<i<<endl;
            return 0;
        }
    }

    cout<<-1<<endl;
    return 0;

}
```



#### 10、**D - String Formation**

**Problem Statement**

Takahashi has a string S consisting of lowercase English letters.

Starting with this string, he will produce a new one in the procedure given as follows.

The procedure consists of  Q operations. In Operation  i (1≤i≤Q) , an integer  T_i is provided, which means the following:

If Ti = 1 : reverse the string S .

If Ti=2 : An integer Fi and a lowercase English letter Ci are additionally provided.

​	If Fi=1 : Add Ci to the beginning of the string S .

​	If Fi=2 : Add Ci to the end of the string S 

.Help Takahashi by finding the final string that results from the procedure.

**Constraints**

1≤|S|≤10^5

S consists of lowercase English letters.

1≤Q≤2×10^5

Ti=1= 1 or 2 .

Fi=1 or 2 , if provided.

Ci is a lowercase English letter, if provided.

WA:

```
#include"bits/stdc++.h"

using namespace std; 

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<string> VS;
typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef long long LL;
typedef priority_queue<int> PQ_DESC;
typedef priority_queue<int, vector<int>, greater<int>> PQ_ASC;
typedef priority_queue<PII> PQ_DESC_PII;
typedef priority_queue<PII, vector<PII>, greater<PII>> PQ_ASC_PII;

#define ALL(c) (c).begin(),(c).end()
#define PB push_back
#define MP make_pair
#define SORT_ASC(c) sort(ALL(c))
#define SORT_DESC(c) sort(ALL(c), greater<typeof(*((c).begin()))>())
#define SIZE(a) int((a).size())

#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define ROF(i,a,b) for(int i=(b-1);i>=(a);--i)
#define REP(i,n) FOR(i,0,n)
#define PER(i,n) ROF(i,0,n)

const double EPS = 1e-10;
const double PI  = acos(-1.0);
const int LARGE_INT = 1e9+100;
const int INF = 2e9+100;
const LL INF_LL = (LL)INF * 300 * 300;
const int MOD = 1e9+7;

//debug
#define dump(x)  cerr << #x << " = " << (x) << endl;
#define debug(x) cerr << #x << " = " << (x) << " (L" << __LINE__ << ")" << " " << __FILE__ << endl;

LL modpow(LL a, LL n) {
    LL res = 1;
    while (n > 0) {
        if (n & 1) res = res * a % MOD;
        a = a * a % MOD;
        n >>= 1;
    }
    return res;
}

LL modinv(LL a) {
    LL b = MOD, u = 1, v = 0;
    while (b) {
        LL t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= MOD;
    if (u < 0) u += MOD;
    return u;
}

LL moddev(LL a, LL b){
	return a * modinv(b) % MOD;
}

void Main()
{
    string s;cin>>s;
    int q;cin>>q;

    int r = 0;

    REP(i,q){
        int t;cin>>t;
        if(t==1){
            r = (r+1)%2;

        }
        else{
            int f;cin>>f;
            string c;cin>>c;

            if((r+f)%2 == 1){
                s = c+s;
            }
            else{
                s = s+c;
            }

        }
    }

    if(r==1){
        //reverse(s.begin(),s.end());
        PER(i,SIZE(s)){
            cout<<s[i];
        }
        cout<<endl;
    }
    else{
        cout<<s<<endl;
    }

	return;
}

int main()
{
	cin.tie(nullptr);
	ios_base::sync_with_stdio(false);
	cout << fixed << setprecision(15);
	Main();
    return 0;
}
```

AC:

```
#include"bits/stdc++.h"

using namespace std; 

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<string> VS;
typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef long long LL;
typedef priority_queue<int> PQ_DESC;
typedef priority_queue<int, vector<int>, greater<int>> PQ_ASC;
typedef priority_queue<PII> PQ_DESC_PII;
typedef priority_queue<PII, vector<PII>, greater<PII>> PQ_ASC_PII;

#define ALL(c) (c).begin(),(c).end()
#define PB push_back
#define MP make_pair
#define SORT_ASC(c) sort(ALL(c))
#define SORT_DESC(c) sort(ALL(c), greater<typeof(*((c).begin()))>())
#define SIZE(a) int((a).size())

#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define ROF(i,a,b) for(int i=(b-1);i>=(a);--i)
#define REP(i,n) FOR(i,0,n)
#define PER(i,n) ROF(i,0,n)

const double EPS = 1e-10;
const double PI  = acos(-1.0);
const int LARGE_INT = 1e9+100;
const int INF = 2e9+100;
const LL INF_LL = (LL)INF * 300 * 300;
const int MOD = 1e9+7;

//debug
#define dump(x)  cerr << #x << " = " << (x) << endl;
#define debug(x) cerr << #x << " = " << (x) << " (L" << __LINE__ << ")" << " " << __FILE__ << endl;

LL modpow(LL a, LL n) {
    LL res = 1;
    while (n > 0) {
        if (n & 1) res = res * a % MOD;
        a = a * a % MOD;
        n >>= 1;
    }
    return res;
}

LL modinv(LL a) {
    LL b = MOD, u = 1, v = 0;
    while (b) {
        LL t = a / b;
        a -= t * b; swap(a, b);
        u -= t * v; swap(u, v);
    }
    u %= MOD;
    if (u < 0) u += MOD;
    return u;
}

LL moddev(LL a, LL b){
	return a * modinv(b) % MOD;
}

void Main()
{
    vector<char> s(7*100000);
    int left=3*100000;
    int right=3*100000;
    string tmp;cin>>tmp;
    REP(i,SIZE(tmp)){
        s[right]=tmp[i];
        right++;
    }
    int q;cin>>q;

    int r = 0;

    REP(i,q){
        int t;cin>>t;
        if(t==1){
            r = (r+1)%2;

        }
        else{
            int f;cin>>f;
            char c;cin>>c;

            if((r+f)%2 == 1){
                left--;
                s[left]=c;
                //s = c+s;
            }
            else{
                s[right]=c;
                right++;
                //s = s+c;
            }

        }
    }

    if(r==1){
        //reverse(s.begin(),s.end());
        ROF(i,left,right){
            cout<<s[i];
        }
        cout<<endl;
    }
    else{
        FOR(i,left,right){
            cout<<s[i];
        }
        cout<<endl;
    }

	return;
}

int main()
{
	cin.tie(nullptr);
	ios_base::sync_with_stdio(false);
	cout << fixed << setprecision(15);
	Main();
    return 0;
}
```