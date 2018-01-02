#include <iostream>
#include <algorithm>
#include <complex>
#include <cassert>
#include <cmath>

using namespace std;

namespace
{
    using uint = unsigned int;

    uint dist(uint square)
    {
        assert(square > 0);
        --square;
        using pos = complex<int>;
        pos cur(0, 0);
        uint dist = 1;
        uint changer = 0;
        pos dir(1, 0);
        while(square > 0)
        {
            const int m = min(dist, square);
            cur += dir*pos(m, 0);
            square -= m;
            dir *= pos(0, -1);
            changer = (changer + 1)%2;
            if(changer == 0)
                ++dist;
        }
        return abs(cur.real()) + abs(cur.imag());
    }
}
int main()
{
    uint square = 0;
    cin>>square;
    cout<<dist(square)<<endl;
    return 0;
}
