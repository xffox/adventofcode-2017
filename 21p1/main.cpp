#include <iostream>
#include <queue>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <utility>
#include <string>
#include <unordered_map>
#include <cmath>

#include <vector>
#include <stdexcept>
#include <cstddef>
#include <limits>

namespace numutil
{
    template<typename T>
    class Matrix
    {
    public:
        Matrix(std::size_t rows, std::size_t columns, const T &init = T())
            :data((rows == 0 ||
                    std::numeric_limits<std::size_t>::max()/rows >= columns)
                ?rows*columns:throw std::invalid_argument(
                    "invalid dimensions"), init),
            row_count(rows), column_count(columns) {}
        T &ix(std::size_t row, std::size_t column)
        {
            if(row >= row_count)
                throw std::invalid_argument("invalid row");
            if(column >= column_count)
                throw std::invalid_argument("invalid column");
            return data[row*column_count + column];
        }
        const T &ix(std::size_t row, std::size_t column) const
        {
            return const_cast<Matrix*>(this)->ix(row, column);
        }
        std::size_t rows() const{return row_count;}
        std::size_t columns() const{return column_count;}
    private:
        typedef std::vector<T> DataCollection;
        DataCollection data;
        std::size_t row_count;
        std::size_t column_count;
    };
}

using namespace std;

namespace
{
    using rulemap = unordered_map<string, string>;
    using board = numutil::Matrix<char>;

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

    string rotate(const string &s)
    {
        string res(s.size(), '\0');
        const size_t sz = round(sqrt(s.size()));
        for(size_t i = 0; i < sz; ++i)
        {
            for(size_t j = 0; j < sz; ++j)
            {
                res[sz*j + sz-1-i] = s[sz*i + j];
            }
        }
        return res;
    }

    string flip_hor(const string &s)
    {
        string res(s.size(), '\0');
        const size_t sz = round(sqrt(s.size()));
        for(size_t i = 0; i < sz; ++i)
        {
            for(size_t j = 0; j < sz; ++j)
            {
                res[sz*i + sz-1-j] = s[sz*i + j];
            }
        }
        return res;
    }

    string flip_vert(const string &s)
    {
        string res(s.size(), '\0');
        const size_t sz = round(sqrt(s.size()));
        for(size_t i = 0; i < sz; ++i)
        {
            for(size_t j = 0; j < sz; ++j)
            {
                res[sz*(sz-1 - i) + j] = s[sz*i + j];
            }
        }
        return res;
    }

    string extract(const board &b, const pair<size_t, size_t> &offset,
        const pair<size_t, size_t> &size)
    {
        string res;
        for(size_t i = 0;i < size.first; ++i)
        {
            for(size_t j=  0; j < size.second; ++j)
            {
                res.push_back(b.ix(offset.first + i, offset.second+j));
            }
        }
        return res;
    }

    board &putvals(board &b, const pair<size_t, size_t> &offset,
        const std::string &vals)
    {
        const size_t sz = round(sqrt(vals.size()));
        for(size_t i = 0;i< sz; ++i)
        {
            for(size_t j = 0; j < sz; ++j)
            {
                b.ix(offset.first+ i, offset.second+j) = vals[sz*i + j];
            }
        }
        return b;
    }

    size_t count(const rulemap &rules)
    {
        const string initial(".#...####");
        const auto initsz = round(sqrt(initial.size()));
        board b(initsz, initsz);
        constexpr size_t ITERS = 5;
        {
            for(size_t i = 0; i < b.rows(); ++i)
            {
                for(size_t j = 0; j < b.columns(); ++j)
                {
                    b.ix(i, j) = initial[i*initsz + j];
                }
            }
        }
        for(size_t i = 0; i < ITERS; ++i)
        {
            const auto nextsz = b.rows()%2 == 0 ? b.rows()/2*3 : b.rows()/3*4;
            const auto curblock = b.rows()%2 == 0 ? 2 : 3;
            const auto nextblock = b.rows()%2 == 0 ? 3 : 4;
            board nextboard(nextsz, nextsz, '.');
            for(size_t blrow = 0; blrow < nextboard.rows()/nextblock; ++blrow)
            {
                for(size_t blcol = 0; blcol < nextboard.columns()/nextblock; ++blcol)
                {
                    auto cur = extract(b, make_pair(curblock*blrow, curblock*blcol),
                        make_pair(curblock, curblock));
                    for(size_t rot = 0; rot < 4; ++rot)
                    {
                        {
                            auto iter = rules.find(cur);
                            if(iter != end(rules))
                            {
                                putvals(nextboard, make_pair(nextblock*blrow, nextblock*blcol),
                                    iter->second);
                                break;
                            }
                        }
                        {
                            auto iter = rules.find(flip_hor(cur));
                            if(iter != end(rules))
                            {
                                putvals(nextboard, make_pair(nextblock*blrow, nextblock*blcol),
                                    iter->second);
                                break;
                            }
                        }
                        {
                            auto iter = rules.find(flip_vert(cur));
                            if(iter != end(rules))
                            {
                                putvals(nextboard, make_pair(nextblock*blrow, nextblock*blcol),
                                    iter->second);
                                break;
                            }
                        }
                        cur = rotate(cur);
                    }
                }
            }
            b = nextboard;
        }
        size_t res = 0;
        for(size_t i = 0; i < b.rows(); ++i)
        {
            for(size_t j = 0; j< b.columns(); ++j)
                res += (b.ix(i, j) != '.');
        }
        return res;
    }
}
int main()
{
    rulemap rules;
    while(true)
    {
        string s;
        getline(cin, s);
        if(cin.eof())
            break;
        const auto r = split(s, '=');
        assert(r.size() == 2);
        const auto fp = split(r[0], ' ');
        assert(!fp.empty());
        const auto sp = split(r[1], ' ');
        assert(sp.size() >= 2);
        string src;
        string dst;
        auto filter = [](char c){return c != '/';};
        copy_if(begin(fp[0]), end(fp[0]), back_inserter(src), filter);
        copy_if(begin(sp[1]), end(sp[1]), back_inserter(dst), filter);
        rules.insert(make_pair(src, dst));
    }
    cout<<count(rules)<<endl;
    return 0;
}
