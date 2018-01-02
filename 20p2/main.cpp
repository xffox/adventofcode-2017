#include <iostream>
#include <utility>
#include <limits>
#include <unordered_set>
#include <unordered_map>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <array>
#include <cassert>
#include <string>
#include <cstddef>
#include <sstream>
#include <vector>
#include <map>

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

    bool sqrtnat(int &r, int val)
    {
        int v = 0;
        while(v*v < val)
        {
            ++v;
        }
        if(v*v == val)
        {
            r = v;
            return true;
        }
        return false;
    }

    vector<int> solve_sq(int a, int b, int c)
    {
        if(a == 0)
        {
            if(b != 0 && c%b ==0)
                return vector<int>{(-c)/b};
            return vector<int>();
        }
        const auto d = b*b - 4*a*c;
        if(d > 0)
        {
            int dd = 0;
            if(sqrtnat(dd, d))
            {
                vector<int> res;
                if((-b + dd)%(2*a) == 0)
                    res.push_back((-b + dd)/(2*a));
                if((-b - dd)%(2*a) == 0)
                    res.push_back((-b - dd)/(2*a));
                return res;
            }
        }
        else if(d == 0)
        {
            if((-b)%(2*a) == 0)
                return vector<int>{(-b)/(2*a)};
        }
        return vector<int>();
    }

    bool intertime(int &t, const particle &left, const particle &right)
    {
        using valmap = unordered_map<int, size_t>;
        valmap seen;
        size_t count = left.pos.size();
        for(size_t i = 0; i < left.pos.size(); ++i)
        {
            const auto a = (left.accel[i]-right.accel[i]);
            const auto b = 2*(left.velocity[i]-right.velocity[i]) +
                    (left.accel[i] - right.accel[i]);
            const auto c = 2*(left.pos[i] - right.pos[i]);
            if(a == 0 && b == 0 && c== 0)
            {
                --count;
                continue;
            }
            const auto r = solve_sq(a, b, c);
            for(auto v : r)
            {
                if(v >= 0)
                {
                    seen[v] += 1;
                }
            }
        }
        bool found = false;
        int minv = numeric_limits<int>::max();
        for(const auto &p : seen)
        {
            if(p.second == count)
            {
                found = true;
                if(p.first < minv)
                    minv = p.first;
            }
        }
        if(count == 0)
        {
            t = 0;
            return true;
        }
        if(found)
        {
            t = minv;
            return true;
        }
        return false;
    }

    size_t solve(const particle_col &particles)
    {
        using posset = unordered_set<size_t>;
        using timemap = map<int, posset>;
        timemap times;
        for(auto iter = begin(particles); iter != end(particles); ++iter)
        {
            auto nextiter = iter;
            ++nextiter;
            for(; nextiter != end(particles); ++nextiter)
            {
                int t = 0;
                if(intertime(t, *iter, *nextiter))
                {
                    times[t].insert(iter - begin(particles));
                    times[t].insert(nextiter - begin(particles));
                }
            }
        }
        posset seen;
        for(const auto &p : times)
        {
            size_t count = 0;
            for(auto idx : p.second)
            {
                if(seen.find(idx) == end(seen))
                {
                    ++count;
                }
            }
            if(count > 1)
            {
                for(auto idx : p.second)
                {
                    seen.insert(idx);
                }
            }
        }
        return particles.size() - seen.size();
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
