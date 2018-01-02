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
    constexpr size_t SZ = 16;

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
    public:
        solve()
            :posits(), idxes()
        {
            for(size_t i = 0; i < SZ; ++i)
            {
                const auto c = 'a' + i;
                posits[c] = i;
                idxes[i] = c;
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
                idxes[p.second] = p.first;
            }
        }

        void swapPos(size_t a, size_t b)
        {
            auto &leftVal = idxes[a];
            auto &rightVal = idxes[b];
            auto &leftPos = posits[leftVal];
            auto &rightPos = posits[rightVal];
            {
                const auto t = leftPos;
                leftPos = rightPos;
                rightPos =t;
            }
            {
                const auto t = leftVal;
                leftVal = rightVal;
                rightVal = t;
            }
        }

        void swapName(char a, char b)
        {
            auto &leftPos = posits[a];
            auto &rightPos = posits[b];
            auto &leftVal = idxes[leftPos];
            auto &rightVal = idxes[rightPos];
            {
                const auto t = leftPos;
                leftPos = rightPos;
                rightPos =t;
            }
            {
                const auto t = leftVal;
                leftVal = rightVal;
                rightVal = t;
            }
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
        using idxmap = unordered_map<size_t, char>;

        posmap posits;
        posmap idxes;
    };
}
int main()
{
    string s;
    getline(cin, s);
    const auto inp = split(s, ',');
    solve solve;
    string init;
    for(size_t i = 0; i < SZ; ++i)
    {
        init.push_back('a' + i);
    }
    constexpr size_t ITERS = 1000000000;
    size_t it = 0;
    vector<string> prevs{init};
    for(; it < ITERS;)
    {
        ++it;
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
        prevs.push_back(solve.getState());
        if(prevs.back() == init)
        {
            break;
        }
    }
    const auto realit = ITERS % it;
    cout<<prevs[realit]<<endl;
    return 0;
}
