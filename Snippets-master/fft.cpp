
#include<bits/stdc++.h>
#define ll          long long
#define pb          push_back
#define eb          emplace_back
#define c(P)        cout<<P<<"\n"
#define pii         pair<ll,ll>
#define mi          map<ll,ll>
#define mii         map<pii,ll>
#define F           first
#define S           second
#define mp(x,y)     make_pair(x,y) 
#define mem(a,val)  memset(a,val,sizeof(a))
#define fr(i,a,n)   for(ll i=a;i<n;i++)
#define frr(i,a,n)  for(ll i=n-1;i>=a;i--)


const ll N   =  500005;
const ll mod = 1e9 + 7;

using namespace std;

typedef vector<int> VI;
double PI = acos(0) * 2;

class com
{
public:
  double a, b;
  com() {a = 0.0; b = 0.0;}
  com(double na, double nb) {a = na; b = nb;}
  const com operator+(const com &c) const
    {return com(a + c.a, b + c.b);}
  const com operator-(const com &c) const
    {return com(a - c.a, b - c.b);}
  const com operator*(const com &c) const
    {return com(a*c.a - b*c.b, a*c.b + b*c.a);}
  double magnitude() {return sqrt(a*a+b*b);}
  void print() {printf("(%.3f %.3f)\n", a, b);}
};

class FFT
{
public:
  vector<com> data;
  vector<com> roots;
  VI rev;
  int s, n;

  void setSize(int ns)
  {
    s = ns;
    n = (1 << s);
    int i, j;
    rev = VI(n);
    data = vector<com> (n);
    roots = vector<com> (n+1);
    for (i = 0; i < n; i++)
      for (j = 0; j < s; j++)
        if ((i & (1 << j)) != 0)
          rev[i] += (1 << (s-j-1));
    roots[0] = com(1, 0);
    com mult = com(cos(2*PI/n), sin(2*PI/n));
    for (i = 1; i <= n; i++)
      roots[i] = roots[i-1] * mult;
  }

  void bitReverse(vector<com> &array)
  {
    vector<com> temp(n);
    int i;
    for (i = 0; i < n; i++)
      temp[i] = array[rev[i]];
    for (i = 0; i < n; i++)
      array[i] = temp[i];
  }

  void transform(bool inverse = false)
  {
    bitReverse(data);
    int i, j, k;
    for (i = 1; i <= s; i++) {
      int m = (1 << i), md2 = m / 2;
      int start = 0, increment = (1 << (s-i));
      if (inverse) {
        start = n;
        increment *= -1;
      }
      com t, u;
      for (k = 0; k < n; k += m) {
        int index = start;
        for (j = k; j < md2+k; j++) {
          t = roots[index] * data[j+md2];
          index += increment;
          data[j+md2] = data[j] - t;
          data[j] = data[j] + t;
        }
      }
    }
    if (inverse)
      for (i = 0; i < n; i++) {
        data[i].a /= n;
        data[i].b /= n;
      }
  }

  static VI convolution(VI &a, VI &b)
  {
    int alen = a.size(), blen = b.size();
    int resn = alen + blen - 1; // size of the resulting array
    int s = 0, i;
    while ((1 << s) < resn) s++;  // n = 2^s
    int n = 1 << s; // round up the the nearest power of two

    FFT pga, pgb;
    pga.setSize(s); // fill and transform first array
    for (i = 0; i < alen; i++) pga.data[i] = com(a[i], 0);
    for (i = alen; i < n; i++)  pga.data[i] = com(0, 0);
    pga.transform();

    pgb.setSize(s); // fill and transform second array
    for (i = 0; i < blen; i++)  pgb.data[i] = com(b[i], 0);
    for (i = blen; i < n; i++)  pgb.data[i] = com(0, 0);
    pgb.transform();

    for (i = 0; i < n; i++) pga.data[i] = pga.data[i] * pgb.data[i];
    pga.transform(true);  // inverse transform
    VI result = VI (resn);  // round to nearest integer
    for (i = 0; i < resn; i++)  result[i] = (int) (pga.data[i].a + 0.5);

    int actualSize = resn - 1;  // find proper size of array
    while (result[actualSize] == 0)
      actualSize--;
    if (actualSize < 0) actualSize = 0;
    result.resize(actualSize+1);
    return result;
  }
};


int main()
{
  VI a = VI ({0,1,1,0,0,0,1,0}),b = VI({0,0,1,1,0});

  VI c = FFT::convolution(a, b);
  /* 1 8 34 104 259 560 1092 1968 3333
  5368 8052 11120 14259 17104 19234 20168 19361 16200 10000*/
  for (int i = 0; i < c.size(); i++)
    printf("%d ", c[i]);
  return 0;
}