#include <iostream>
#include <string>
#include <iterator>
#include <ios>
#include <iomanip>
#include <cstddef>
#include <vector>

using namespace std;

namespace
{
    using uchar = unsigned char;
    using ucharcol = vector<uchar>;

    ucharcol knothash(const ucharcol &val)
    {
        constexpr size_t ROUNDS = 64;
        constexpr size_t SZ = 256;
        constexpr size_t BLOCKSZ = 16;
        ucharcol tgt;
        for(size_t i = 0; i < SZ; ++i)
            tgt.push_back(i);
        size_t skip = 0;
        size_t pos = 0;
        ucharcol extval(val);
        extval.insert(end(extval), {17, 31, 73, 47, 23});
        for(size_t round = 0; round < ROUNDS; ++round)
        {
            for(auto v : extval)
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
        }
        ucharcol res;
        for(size_t i = 0; i < tgt.size();)
        {
            uchar cur = 0;
            do
            {
                cur ^= tgt[i];
                ++i;
            }
            while(i%BLOCKSZ != 0);
            res.push_back(cur);
        }
        return res;
    }
}
int main()
{
    string s;
    getline(cin, s);
    ucharcol val;
    for(auto c : s)
        val.push_back(c);
    const auto h = knothash(val);
    for(auto v : h)
        cout<<hex<<setw(2)<<setfill('0')<<static_cast<unsigned int>(v);
    cout<<endl;
    return 0;
}
