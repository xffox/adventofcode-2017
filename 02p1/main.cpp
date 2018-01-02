#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

namespace
{
    using uint = unsigned int;
    using uintcol = vector<uint>;
    using board = vector<uintcol>;

    uint checksum(const board &b)
    {
        uint s=0;
        for(const auto &r : b)
        {
            if(!r.empty())
            {
                const auto its = minmax_element(begin(r), end(r));
                s += *its.second - *its.first;
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
