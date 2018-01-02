#include <iostream>
#include <cstddef>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <utility>

using namespace std;

namespace
{
    using uint = unsigned int;
    using uintcol = vector<uint>;
    using board = vector<uintcol>;

    inline pair<uint, bool> trydiv(uint a, uint b)
    {
        if(a < b)
            return trydiv(b, a);
        if(a%b == 0)
        {
            return make_pair(a/b, true);
        }
        else
        {
            return make_pair(0, false);
        }
    }

    uint checksum(const board &b)
    {
        uint s=0;
        for(const auto &r : b)
        {
            for(size_t i = 1; i < r.size(); ++i)
            {
                for(size_t j = i; j > 0; --j)
                {
                    const auto d = trydiv(r[i], r[j-1]);
                    if(d.second)
                    {
                        s += d.first;
                    }
                }
            }
        }
        return s;
    }
}

int main()
{
    board b;
    while(cin)
    {
        string line;
        getline(cin, line);
        stringstream ss(line);
        uintcol r;
        while(ss)
        {
            uintcol::value_type v = 0;
            ss>>v;
            r.push_back(v);
            if(ss.eof())
                break;
        }
        b.push_back(move(r));
    }
    cout<<checksum(b)<<endl;
    return 0;
}
