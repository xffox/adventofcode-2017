#include <iostream>
#include <cstddef>
#include <sstream>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

namespace
{
    using ulint = unsigned long long;

    ulint parseValue(const string &s)
    {
        stringstream ss(s);
        ulint res = 0;
        ss>>res;
        return res;
    }

    vector<string> split(const string &str, char sep)
    {
        vector<string> res;
        for(size_t p = 0; p < str.size();)
        {
            auto nextPos = str.find(sep, p);
            if(nextPos == string::npos)
            {
                nextPos = str.size();
            }
            res.push_back(str.substr(p, nextPos-p));
            while(nextPos < str.size() && str[nextPos] == sep)
                ++nextPos;
            p = nextPos;
        }
        return res;
    }

    ulint count(ulint g1, ulint g2)
    {
        constexpr ulint f1 = 16807;
        constexpr ulint f2 = 48271;
        constexpr ulint mask = (1<<16)-1;
        constexpr ulint mod = 2147483647;
        constexpr size_t sz = 40000000;
        ulint c = 0;
        for(size_t i = 0; i < sz; ++i)
        {
            g1 = g1*f1%mod;
            g2 = g2*f2%mod;
            if((g1&mask) == (g2&mask))
                ++c;
        }
        return c;
    }
}
int main()
{
    string s;
    getline(cin, s);
    const auto r1 = split(s, ' ');
    assert(r1.size() == 5);
    ulint g1 = parseValue(r1[4]);
    getline(cin, s);
    const auto r2 = split(s, ' ');
    assert(r2.size() == 5);
    ulint g2 = parseValue(r2[4]);
    cout<<count(g1, g2)<<endl;
    return 0;
}
