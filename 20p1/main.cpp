#include <iostream>
#include <iterator>
#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <cstddef>
#include <sstream>
#include <vector>

using namespace std;

namespace
{
    int parseValue(const string &s)
    {
        stringstream ss(s);
        int res = 0;
        ss>>res;
        return res;
    }

    vector<string> split(const string &str, char sep)
    {
        vector<string> res;
        for(size_t p = 0; p < str.size();)
        {
            auto nextPos = str.find(sep, p);
            if(nextPos == string::npos)
            {
                nextPos = str.size();
            }
            res.push_back(str.substr(p, nextPos-p));
            while(nextPos < str.size() && str[nextPos] == sep)
                ++nextPos;
            p = nextPos;
        }
        return res;
    }

    using vect = array<int, 3>;
    struct particle
    {
        vect pos;
        vect velocity;
        vect accel;
    };
    using particle_col = vector<particle>;

    vect parse_vect(const string &s)
    {
        const auto ep = split(s, '<');
        assert(ep.size() == 2);
        const auto dt = split(ep[1], '>');
        assert(dt.size() >= 1);
        const auto values = split(dt[0], ',');
        assert(values.size() == 3);
        vect res{0, 0, 0};
        for(size_t i =0; i < res.size(); ++i)
            res[i] = parseValue(values[i]);
        return res;
    }

    inline int length_sq(const vect &v)
    {
        int res = 0;
        for(auto r : v)
            res += r*r;
        return res;
    }

    size_t solve(const particle_col &particles)
    {
        assert(!particles.empty());
        auto iter = min_element(begin(particles), end(particles),
            [](const particle &left, const particle &right){
                return length_sq(left.accel) < length_sq(right.accel);
            });
        return iter - begin(particles);
    }
}
int main()
{
    particle_col particles;
    while(true)
    {
        string s;
        getline(cin, s);
        if(cin.eof())
            break;
        const auto parts = split(s, ' ');
        assert(parts.size()==3);
        particles.push_back(particle{
            parse_vect(parts[0]),
            parse_vect(parts[1]),
            parse_vect(parts[2])
            });
    }
    cout<<solve(particles)<<endl;
    return 0;
}
