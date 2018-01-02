#include <iostream>
#include <vector>

using namespace std;

namespace
{
    using intcol = vector<int>;

    intcol sieve(int sz)
    {
        intcol s(sz+1, true);
        for(long long int i = 2; i*i <= sz; ++i)
        {
            if(s[i])
            {
                for(long long int j = i*i; j <= sz; j+=i)
                    s[j] = false;
            }
        }
        return s;
    }

    int solve()
    {
        int b = 99;
        b *= 100;
        b += 100000;
        int c = b;
        c += 17000;
        const auto s = sieve(c);
        int h = 0;
        for(; b <= c; b += 17)
        {
            if(!s[b])
            {
                ++h;
            }
        }
        return h;
    }
}
int main()
{
    cout<<solve()<<endl;
    return 0;
}
