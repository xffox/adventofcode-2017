#include <iostream>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <unordered_set>
#include <cassert>
#include <queue>

using namespace std;

namespace
{
    using uint = unsigned int;

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
        using stringset = unordered_set<string>;
        using childrenmap = unordered_map<string, stringset>;
        using weightmap = unordered_map<string, uint>;

        static uint parseWeight(const string &s)
        {
            stringstream ss(s);
            char c = '\0';
            ss>>c;
            uint res = 0;
            ss>>res;
            return res;
        }

        pair<uint, uint> unbalancedWeight(const string &cur) const
        {
            auto iter = conns.find(cur);
            auto weightIter = weights.find(cur);
            assert(weightIter != end(weights));
            uint total = 0;
            if(iter != end(conns))
            {
                vector<pair<uint, string>> ws;
                for(const auto &child : iter->second)
                {
                    const auto r = unbalancedWeight(child);
                    if(r.second != 0)
                        return r;
                    ws.push_back(make_pair(r.first, child));
                    total += r.first;
                }
                for(size_t i = 0; i < ws.size(); ++i)
                {
                    bool eq = false;
                    for(size_t j = 1; j < ws.size(); ++j)
                    {
                        if(ws[(i+j)%ws.size()].first == ws[i].first)
                        {
                            eq = true;
                            break;
                        }
                    }
                    if(!eq)
                    {
                        const int diff = static_cast<int>(ws[i].first) -
                            static_cast<int>(ws[(i+1)%ws.size()].first);
                        return make_pair(0, weights.at(ws[i].second) - diff);
                    }
                }
            }
            total += weightIter->second;
            return make_pair(total, 0);
        }

    public:
        solve()
            :conns(), weights(), seen(), children()
        {}

        void add(const string &s)
        {
            const auto vs = split(s, ' ');
            assert(vs.size() >= 2);
            weights[vs[0]] = parseWeight(vs[1]);
            seen.insert(vs[0]);
            for(size_t i = 3; i < vs.size(); ++i)
            {
                auto cur = vs[i];
                if(!cur.empty() && cur.back() == ',')
                {
                    cur.resize(cur.size()-1);
                }
                conns[vs[0]].insert(cur);
                children.insert(cur);
            }
        }

        uint unbalanced() const
        {
            return unbalancedWeight(head()).second;
        }

        string head() const
        {
            stringset cur(seen);
            for(const auto &c : children)
                cur.erase(c);
            assert(cur.size() == 1);
            return *begin(cur);
        }

    private:
        childrenmap conns;
        weightmap weights;
        stringset seen;
        stringset children;
    };
}
int main()
{
    solve solve;
    while(true)
    {
        string s;
        getline(cin,s);
        if(cin.eof())
            break;
        solve.add(s);
    }
    cout<<solve.unbalanced()<<endl;
    return 0;
}
