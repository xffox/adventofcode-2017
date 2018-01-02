#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

using namespace std;

namespace
{
    using uint = unsigned int;
    using stringcol = vector<string>;

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

    uint distance(const stringcol &dirs)
    {
        int x = 0;
        int y = 0;
        for(const auto &dir : dirs)
        {
            if(dir == "n")
            {
                y += 1;
            }
            else if(dir == "s")
            {
                y -= 1;
            }
            else if(dir == "nw")
            {
                x += 1;
            }
            else if(dir == "se")
            {
                x -= 1;
            }
            else if(dir == "ne")
            {
                x -= 1;
                y += 1;
            }
            else if(dir == "sw")
            {
                x += 1;
                y -= 1;
            }
            else
            {
                assert(false);
            }
        }
        return max(abs(x), abs(y));
    }
}
int main()
{
    string s;
    getline(cin, s);
    cout<<distance(split(s, ','))<<endl;
    return 0;
}
