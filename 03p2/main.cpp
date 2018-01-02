#include <iostream>
#include <algorithm>
#include <complex>
#include <cassert>
#include <cmath>
#include <map>
#include <utility>

using namespace std;

namespace
{
    using uint = unsigned int;
    using pos = complex<int>;
    using board = map<tuple<int, int>, uint>;

    uint dist(uint target)
    {
        pos cur(0, 0);
        uint dist = 1;
        uint changer = 0;
        pos dir(1, 0);
        board b{make_pair(make_tuple(0, 0), 1)};
        while(true)
        {
            for(uint curdist = dist; curdist > 0; --curdist)
            {
                cur += dir;
                uint s = 0;
                for(int r = -1; r <= 1; ++r)
                {
                    for(int c = -1; c <= 1; ++c)
                    {
                        if(r == 0 && c == 0)
                            continue;
                        const auto p = cur + pos(r, c);
                        auto iter = b.find(make_tuple(p.real(), p.imag()));
                        if(iter != end(b))
                            s += iter->second;
                    }
                }
                if(s > target)
                    return s;
                b.insert(make_pair(make_tuple(cur.real(), cur.imag()), s));
            }
            dir *= pos(0, -1);
            changer = (changer + 1)%2;
            if(changer == 0)
                ++dist;
        }
        assert(false);
        return 0;
    }
}
int main()
{
    uint target = 0;
    cin>>target;
    cout<<dist(target)<<endl;
    return 0;
}
