#include <iostream>
#include <set>
#include <algorithm>
#include <vector>
#include <cassert>
#include <cstddef>

using namespace std;

namespace
{
    using uint = unsigned int;
    using uintcol = vector<uint>;

    uint solve(const uintcol &vs)
    {
        assert(!vs.empty());
        using seenset = set<uintcol>;
        uintcol t(vs);
        seenset seen{t};
        uint iters = 0;
        while(true)
        {
            auto iter = max_element(begin(t), end(t));
            const auto v = *iter;
            const auto n = v/t.size();
            const auto offset = iter - begin(t);
            t[offset] = 0;
            for(size_t i = 1; i <= t.size(); ++i)
            {
                t[(offset + i)%t.size()] += n + (i <= v%t.size());
            }
            ++iters;
            if(!seen.insert(t).second)
                break;
        }
        return iters;
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
        vs.push_back(v);
    }
    cout<<solve(vs)<<endl;
    return 0;
}
