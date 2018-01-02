#include <iostream>
#include <cstddef>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cassert>

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

    class solve
    {
        using stringset = unordered_set<string>;
    public:
        solve()
            :seen(), children()
        {}

        void add(const string &s)
        {
            const auto vs = split(s, ' ');
            if(!vs.empty())
            {
                seen.insert(vs[0]);
                for(size_t i = 3; i < vs.size(); ++i)
                {
                    auto cur = vs[i];
                    if(!cur.empty() && cur.back() == ',')
                    {
                        cur.resize(cur.size()-1);
                    }
                    children.insert(cur);
                }
            }
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
    cout<<solve.head()<<endl;
    return 0;
}
