#include <iostream>
#include <cstddef>
#include <vector>

using namespace std;

namespace
{
    using uint = unsigned int;
    using uintcol = vector<uint>;

    uint solve(const uintcol &vs)
    {
        constexpr uint SZ = 256;
        uintcol tgt;
        for(uint i = 0; i < SZ; ++i)
            tgt.push_back(i);
        uint skip = 0;
        uint pos = 0;
        for(auto v : vs)
        {
            for(size_t i = 0; i < v/2; ++i)
            {
                auto &left = tgt[(pos+i)%tgt.size()];
                auto &right = tgt[(pos+(v-i)-1)%tgt.size()];
                const auto t = left;
                left = right;
                right = t;
            }
            pos = (pos + v + skip)%tgt.size();
            skip +=1;
        }
        return tgt[0]*tgt[1];
    }
}
int main()
{
    uintcol vs;
    while(true)
    {
        uint v = 0;
        cin>>v;
        if(cin.eof())
            break;
        char c = '\0';
        cin>>c;
        vs.push_back(v);
    }
    cout<<solve(vs)<<endl;
    return 0;
}
