#include <iostream>
#include <algorithm>
#include <cstddef>
#include <sstream>
#include <unordered_map>
#include <limits>
#include <cassert>
#include <vector>

using namespace std;

namespace
{
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

    class machine
    {
        static int parseValue(const string &s)
        {
            stringstream ss(s);
            int res = 0;
            ss>>res;
            return res;
        }

    public:
        machine()
            :registers()
        {
        }

        void exec(const string &line)
        {
            constexpr size_t ARGS = 7;
            const auto r = split(line, ' ');
            assert(r.size() == ARGS);
            const auto &op = r[5];
            const auto arg = parseValue(r[6]);
            const auto diff = parseValue(r[2]);
            auto &tgt = registers[r[0]];
            const auto argVal = registers[r[4]];
            bool succ = false;
            int sign = r[1] == "inc" ? 1 : -1;
            if (op == ">")
            {
                succ = argVal > arg;
            }
            else if(op == "<")
            {
                succ = argVal < arg;
            }
            else if(op == ">=")
            {
                succ = argVal >= arg;
            }
            else if(op == "<=")
            {
                succ = argVal <= arg;
            }
            else if(op == "==")
            {
                succ = argVal == arg;
            }
            else if(op == "!=")
            {
                succ = argVal != arg;
            }
            else
            {
                assert(false);
            }
            if(succ)
            {
                tgt += sign*diff;
                maxval=max(maxval, tgt);
            }
        }

        int maxregvalue() const
        {
            return maxval;
        }

    private:
        using registermap = unordered_map<string, int>;
        registermap registers;
        int maxval = numeric_limits<int>::min();
    };
}
int main()
{
    string line;
    machine m;
    while(true)
    {
        getline(cin, line);
        if(cin.eof())
            break;
        m.exec(line);
    }
    cout<<m.maxregvalue()<<endl;
    return 0;
}
