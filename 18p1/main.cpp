#include <iostream>
#include <cmath>
#include <unordered_map>
#include <cstddef>
#include <cassert>
#include <vector>
#include <sstream>
#include <string>
#include <cctype>

using namespace std;

namespace
{
    using stringcol = vector<string>;

    int parseValue(const string &s)
    {
        stringstream ss(s);
        int res = 0;
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

    int solve(const stringcol &cmds)
    {
        using registermap = unordered_map<char, long long int>;
        registermap registers;
        auto parseArg = [&registers](const string &s){
            int val = 0;
            if(isalpha(s[0]))
            {
                val = registers[s[0]];
            }
            else
            {
                val = parseValue(s);
            }
            return val;
        };
        int freq = 0;
        int pos = 0;
        while(true)
        {
            assert(pos >= 0 && static_cast<unsigned int>(pos) < cmds.size());
            const auto r = split(cmds[pos], ' ');
            if(r[0] == "snd")
            {
                assert(r.size() == 2);
                freq = parseArg(r[1]);
            }
            else if(r[0] == "set")
            {
                assert(r.size() == 3);
                registers[r[1][0]] = parseArg(r[2]);
            }
            else if(r[0] == "add")
            {
                assert(r.size() == 3);
                registers[r[1][0]] += parseArg(r[2]);
            }
            else if(r[0] == "mul")
            {
                assert(r.size() == 3);
                registers[r[1][0]] *= parseArg(r[2]);
            }
            else if(r[0] == "mod")
            {
                assert(r.size() == 3);
                auto &tgt = registers[r[1][0]];
                const auto v = parseArg(r[2]);
                tgt = abs(tgt%v);
            }
            else if(r[0] == "rcv")
            {
                assert(r.size() == 2);
                if(parseArg(r[1]) != 0)
                {
                    return freq;
                }
            }
            else if(r[0] == "jgz")
            {
                assert(r.size() == 3);
                if(parseArg(r[1]) > 0)
                {
                    pos += (parseArg(r[2])-1);
                }
            }
            else
            {
                assert(false);
            }
            pos += 1;
        }
        return 0;
    }
}
int main()
{
    stringcol cmds;
    while(true)
    {
        string cmd;
        getline(cin, cmd);
        if(cin.eof())
            break;
        cmds.push_back(move(cmd));
    }
    cout<<solve(cmds)<<endl;
    return 0;
}
