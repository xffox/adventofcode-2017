#include <iostream>
#include <utility>
#include <array>
#include <queue>
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

    using registermap = unordered_map<char, long long int>;
    using msgqueue = queue<long long int>;

    struct state
    {
        int pos;
        registermap registers;
        msgqueue messages;
        bool stopped;
        bool waiting;
    };

    int solve(const stringcol &cmds)
    {
        array<state, 2> threads{
            state{0, registermap{make_pair('p', 0L)}, msgqueue(), false, false},
            state{0, registermap{make_pair('p', 1L)}, msgqueue(), false, false}
        };
        auto parseArg = [](registermap &registers, const string &s){
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
        size_t tid = 0;
        int res = 0;
        while(true)
        {
            auto &curthread = threads[tid];
            if(curthread.stopped || curthread.waiting)
            {
                tid = !tid;
                if(threads[tid].stopped || threads[tid].waiting)
                    break;
                continue;
            }
            if(curthread.pos < 0 ||
                static_cast<unsigned int>(curthread.pos) >= cmds.size())
            {
                curthread.stopped = true;
                tid = !tid;
                continue;
            }
            const auto r = split(cmds[curthread.pos], ' ');
            bool switchthread = false;
            if(r[0] == "snd")
            {
                assert(r.size() == 2);
                threads[!tid].messages.push(parseArg(curthread.registers, r[1]));
                threads[!tid].waiting = false;
                switchthread = true;
                if(tid == 1)
                    ++res;
            }
            else if(r[0] == "set")
            {
                assert(r.size() == 3);
                curthread.registers[r[1][0]] =
                    parseArg(curthread.registers, r[2]);
            }
            else if(r[0] == "add")
            {
                assert(r.size() == 3);
                curthread.registers[r[1][0]] +=
                    parseArg(curthread.registers, r[2]);
            }
            else if(r[0] == "mul")
            {
                assert(r.size() == 3);
                curthread.registers[r[1][0]] *=
                    parseArg(curthread.registers, r[2]);
            }
            else if(r[0] == "mod")
            {
                assert(r.size() == 3);
                auto &tgt = curthread.registers[r[1][0]];
                const auto v = parseArg(curthread.registers, r[2]);
                tgt = abs(tgt%v);
            }
            else if(r[0] == "rcv")
            {
                assert(r.size() == 2);
                if(curthread.messages.empty())
                {
                    --curthread.pos;
                    curthread.waiting = true;
                    switchthread = true;
                }
                else
                {
                    curthread.registers[r[1][0]] = curthread.messages.front();
                    curthread.messages.pop();
                }
            }
            else if(r[0] == "jgz")
            {
                assert(r.size() == 3);
                if(parseArg(curthread.registers, r[1]) > 0)
                {
                    curthread.pos += (parseArg(curthread.registers, r[2])-1);
                }
            }
            else
            {
                assert(false);
            }
            curthread.pos += 1;
            if(switchthread)
                tid = !tid;
        }
        return res;
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
