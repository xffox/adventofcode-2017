#include <iostream>
#include <sstream>
#include <cstddef>
#include <cassert>
#include <string>
#include <tuple>
#include <vector>
#include <iterator>
#include <unordered_set>

using namespace std;

namespace
{
    using uint = unsigned int;

    uint parse_value(const string &s)
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

    using component = tuple<uint, uint>;
    using componentcol = vector<component>;

    uint strongest(const componentcol &cmps)
    {
        using idxset = unordered_set<size_t>;
        using stcol = vector<size_t>;
        uint tgt = 0;
        idxset seen;
        stcol st{0};
        uint ms = 0;
        uint curs = 0;
        uint ml = 0;
        uint curl = 0;
        while(!st.empty())
        {
            while(st.back() < cmps.size() &&
                (seen.find(st.back()) != end(seen) ||
                 (get<0>(cmps[st.back()]) != tgt &&
                  get<1>(cmps[st.back()]) != tgt)))
            {
                ++st.back();
            }
            if(st.back() < cmps.size())
            {
                if(get<0>(cmps[st.back()]) == tgt)
                {
                    tgt = get<1>(cmps[st.back()]);
                }
                else
                {
                    tgt = get<0>(cmps[st.back()]);
                }
                seen.insert(st.back());
                curs += get<0>(cmps[st.back()]) + get<1>(cmps[st.back()]);
                st.push_back(0);
                ++curl;
                if(curl > ml)
                {
                    ml = curl;
                    ms = curs;
                }
                else if(curl == ml)
                {
                    if(curs > ms)
                        ms = curs;
                }
            }
            else
            {
                st.pop_back();
                if(!st.empty())
                {
                    seen.erase(st.back());
                    if(get<0>(cmps[st.back()]) == tgt)
                    {
                        tgt = get<1>(cmps[st.back()]);
                    }
                    else
                    {
                        tgt = get<0>(cmps[st.back()]);
                    }
                    curs -= get<0>(cmps[st.back()]) + get<1>(cmps[st.back()]);
                    --curl;
                    ++st.back();
                }
            }
        }
        return ms;
    }
}

int main()
{
    componentcol cmps;
    while(true)
    {
        string s;
        getline(cin, s);
        if(cin.eof())
            break;
        const auto r = split(s, '/');
        assert(r.size() == 2);
        cmps.push_back(make_tuple(parse_value(r[0]), parse_value(r[1])));
    }
    cout<<strongest(cmps)<<endl;
    return 0;
}
