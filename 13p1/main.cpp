#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <cassert>
#include <string>
#include <cstddef>
#include <unordered_map>

using namespace std;

namespace
{
    using uint = unsigned int;
    using uintmap = unordered_map<uint, uint>;

    uint parseValue(const string &s)
    {
        stringstream ss(s);
        uint res = 0;
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

    uint severity(const uintmap &ranges)
    {
        uint res =0;
        for(const auto &p : ranges)
        {
            if(p.second != 0)
            {
                const auto pos = p.first%(p.second*2 - 2);
                if(pos == 0)
                {
                    res += p.second*p.first;
                }
            }
        }
        return res;
    }
}
int main()
{
    string s;
    uintmap ranges;
    while(true)
    {
        getline(cin, s);
        if(cin.eof())
            break;
        const auto line = split(s, ' ');
        assert(line.size() == 2);
        ranges[parseValue(line[0])] = parseValue(line[1]);
    }
    cout<<severity(ranges)<<endl;
    return 0;
}
