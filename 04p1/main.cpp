#include <iostream>
#include <string>
#include <cstddef>
#include <unordered_set>

using namespace std;

namespace
{
    bool valid(const string &s)
    {
        using wordset = unordered_set<string>;
        string::size_type p = 0;
        wordset seen;
        while(p < s.size())
        {
            auto np = s.find(' ', p);
            if(np == string::npos)
                np = s.size();
            if(!seen.insert(s.substr(p, np-p)).second)
                return false;
            while(np < s.size() && s[np] == ' ')
                ++np;
            p = np;
        }
        return true;
    }
}

int main()
{
    size_t count = 0;
    while(cin)
    {
        string s;
        getline(cin, s);
        if(!s.empty())
        {
            if(valid(s))
                ++count;
        }
    }
    cout<<count<<endl;
    return 0;
}
