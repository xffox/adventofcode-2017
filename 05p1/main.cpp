#include <iostream>
#include <vector>
#include <cstddef>

using namespace std;

namespace
{
    using intcol = vector<int>;

    uint steps(const intcol &cmds)
    {
        intcol cs(cmds);
        int pos = 0;
        uint steps = 0;
        while(true)
        {
            if(pos < 0 || pos >= cs.size())
                break;
            pos += (cs[pos]++);
            ++steps;
        }
        return steps;
    }
}

int main()
{
    intcol cmds;
    while(true)
    {
        int v = 0;
        cin>>v;
        if(cin.eof())
            break;
        cmds.push_back(v);
    }
    cout<<steps(cmds)<<endl;
}
