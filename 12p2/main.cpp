#include <iostream>
#include <iterator>
#include <cstddef>
#include <vector>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>

using namespace std;

namespace
{
    using uint = unsigned int;

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

    class solve
    {
    private:
        using idset = unordered_set<uint>;

    public:
        solve()
            :connections()
        {}

        void addconnection(uint left, uint right)
        {
            connections[left].insert(right);
            connections[right].insert(left);
        }

        uint groups() const
        {
            using frontqueue = queue<uint>;
            idset unseen;
            for(const auto &p : connections)
                unseen.insert(p.first);
            uint groups = 0;
            while(!unseen.empty())
            {
                ++groups;
                frontqueue front;
                auto startiter = begin(unseen);
                front.push(*startiter);
                unseen.erase(startiter);
                while(!front.empty())
                {
                    const auto cur = front.front();
                    front.pop();
                    auto iter = connections.find(cur);
                    if(iter != end(connections))
                    {
                        for(const auto nextid : iter->second)
                        {
                            if(unseen.erase(nextid) > 0)
                            {
                                front.push(nextid);
                            }
                        }
                    }
                }
            }
            return groups;
        }

    private:
        using conmap = unordered_map<uint, idset>;

        conmap connections;
    };
}
int main()
{
    solve solve;
    while(true)
    {
        string s;
        getline(cin, s);
        if(cin.eof())
            break;
        const auto r = split(s, ' ');
        assert(r.size() >= 3);
        const auto from = parseValue(r[0]);
        for(size_t i = 2; i < r.size(); ++i)
        {
            solve.addconnection(from, parseValue(r[i]));
        }
    }
    cout<<solve.groups()<<endl;
    return 0;
}
