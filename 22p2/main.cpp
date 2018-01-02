#include <iostream>
#include <iterator>
#include <tuple>
#include <map>
#include <vector>
#include <complex>
#include <cassert>
#include <string>

using namespace std;

namespace
{
    template<typename T>
    class matrix
    {
    public:
        matrix(T init = T())
            :data(), init(init)
        {}
        T &ix(int row, int column)
        {
            return data[make_tuple(row, column)];
        }
        const T &ix(int row, int column) const
        {
            auto iter = data.find(make_tuple(row, column));
            if(iter != end(data))
                return iter->second;
            return init;
        }
    private:
        using valcol = map<tuple<int, int>, T>;
        valcol data;
        T init;
    };

    enum class state
    {
        CLEAN,
        WEAKENED,
        INFECTED,
        FLAGGED
    };

    using board = matrix<state>;

    size_t solve(const board &b)
    {
        board cur(b);
        constexpr size_t BURSTS = 10000000;
        using pos = complex<int>;
        pos p(0, 0);
        pos dir(-1, 0);
        size_t res = 0;
        for(size_t i = 0; i < BURSTS; ++i)
        {
            auto &v = cur.ix(p.real(), p.imag());
            switch(v)
            {
            case state::CLEAN:
                dir *= pos(0, 1);
                v = state::WEAKENED;
                break;
            case state::WEAKENED:
                v = state::INFECTED;
                ++res;
                break;
            case state::FLAGGED:
                dir *= pos(-1, 0);
                v = state::CLEAN;
                break;
            case state::INFECTED:
                dir *= pos(0, -1);
                v = state::FLAGGED;
                break;
            default:
                assert(false);
            }
            p = p + dir;
        }
        return res;
    }
}
int main()
{
    using stringcol= vector<string>;
    stringcol values;
    while(true)
    {
        string s;
        getline(cin, s);
        if(cin.eof())
            break;
        values.push_back(move(s));
    }
    assert(!values.empty());
    board b;
    for(int i = 0; static_cast<size_t>(i) < values.size(); ++i)
    {
        for(int j = 0; static_cast<size_t>(j) < values[i].size(); ++j)
        {
            b.ix(i - (values.size()-1)/2, j - (values[i].size()-1)/2) =
                (values[i][j] == '#')?state::INFECTED:state::CLEAN;
        }
    }
    cout<<solve(b)<<endl;
    return 0;
}
