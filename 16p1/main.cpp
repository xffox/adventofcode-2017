#include <iostream>
#include <cassert>
#include <sstream>
#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

namespace
{
    size_t parseValue(const string &s)
    {
        stringstream ss(s);
        size_t res = 0;
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

    class solve
    {
        static constexpr size_t SZ = 16;
    public:
        solve()
            :posits()
        {
            for(size_t i = 0; i < 16; ++i)
            {
                const auto c = 'a' + i;
                posits[c] = i;
            }
        }

        void spin(size_t sz)
        {
            const auto begin = SZ - sz;
            for(auto &p : posits)
            {
                if(p.second < begin)
                {
                    p.second = sz + p.second;
                }
                else
                {
                    p.second = p.second - begin;
                }
            }
        }

        void swapPos(size_t a, size_t b)
        {
            for(auto &p : posits)
            {
                if(p.second == a)
                {
                    p.second = b;
                }
                else if(p.second == b)
                {
                    p.second = a;
                }
            }
        }

        void swapName(char a, char b)
        {
            auto &left = posits[a];
            auto &right = posits[b];
            const auto t = left;
            left = right;
            right =t;
        }

        string getState() const
        {
            string res(SZ, '\0');
            for(const auto &p : posits)
            {
                res[p.second] = p.first;
            }
            return res;
        }

    private:
        using posmap = unordered_map<char, size_t>;

        posmap posits;
    };
}
int main()
{
    string s;
    getline(cin, s);
    const auto inp = split(s, ',');
    solve solve;
    for(const auto &c : inp)
    {
        if(c[0] == 's')
        {
            const auto sz = parseValue(c.substr(1));
            solve.spin(sz);
        }
        else if(c[0] == 'x')
        {
            const auto parts = split(c.substr(1), '/');
            assert(parts.size() == 2);
            solve.swapPos(parseValue(parts[0]), parseValue(parts[1]));
        }
        else if(c[0] == 'p')
        {
            assert(c.size() == 4);
            solve.swapName(c[1], c[3]);
        }
        else
        {
            assert(false);
        }
    }
    cout<<solve.getState()<<endl;
    return 0;
}
