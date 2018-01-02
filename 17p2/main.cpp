#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

namespace
{
    using uint = unsigned int;
    using uintcol = vector<uint>;

    uint solve(uint offset)
    {
        constexpr uint STEPS = 50000000;
        uint sz = 1;
        uint pos = 0;
        uint val = 0;
        for(uint i = 1; i <= STEPS; ++i)
        {
            pos = (pos + offset)%sz;
            if(pos == 0)
                val = i;
            ++pos;
            ++sz;
        }
        return val;
    }
}
int main()
{
    uint offset = 0;
    cin>>offset;
    cout<<solve(offset)<<endl;
    return 0;
}
