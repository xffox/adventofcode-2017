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
        constexpr uint STEPS = 2017;
        uintcol values{0};
        uint pos = 0;
        for(uint i = 1; i <= STEPS; ++i)
        {
            pos = (pos + offset)%values.size();
            values.insert(begin(values) + pos + 1, i);
            ++pos;
        }
        return values[(pos + 1)%values.size()];
    }
}
int main()
{
    uint offset = 0;
    cin>>offset;
    cout<<solve(offset)<<endl;
    return 0;
}
