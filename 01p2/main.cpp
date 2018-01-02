#include <iostream>
#include <vector>
#include <string>
#include <cstddef>

using namespace std;

namespace
{
    using uint = unsigned int;
    using uintcol = vector<uint>;

    uint solve(const uintcol &ds)
    {
        const auto offset = ds.size()/2;
        uint s = 0;
        for(size_t i = 0; i < ds.size(); ++i)
        {
            if(ds[i] == ds[(i+offset)%ds.size()])
                s += ds[i];
        }
        return s;
    }
}

int main()
{
    string s;
    getline(cin, s);
    uintcol ds;
    ds.reserve(s.size());
    for(auto c : s)
    {
        ds.push_back(c - '0');
    }
    cout<<solve(ds)<<endl;
    return 0;
}
