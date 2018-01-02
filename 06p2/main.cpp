#include <iostream>
#include <map>
#include <utility>
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
        using seenmap = map<uintcol, uint>;
        uintcol t(vs);
        uint iters = 0;
        seenmap seen{make_pair(t, iters)};
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
            const auto res = seen.insert(make_pair(t, iters));
            if(!res.second)
            {
                return iters - res.first->second;
            }
        }
        return 0;
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
