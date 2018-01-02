#include <iostream>
#include <tuple>
#include <queue>
#include <string>
#include <iterator>
#include <ios>
#include <iomanip>
#include <cstddef>
#include <vector>

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
    using uchar = unsigned char;
    using ucharcol = vector<uchar>;
    using board = numutil::Matrix<int>;

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

    size_t groups(const string &h)
    {
        constexpr size_t SZ = 128;
        constexpr size_t VALSZ = numeric_limits<uchar>::digits;
        board disk(SZ, SZ);
        for(size_t i = 0; i < SZ; ++i)
        {
            const auto val = h + '-' + to_string(i);
            const auto linehash = knothash(ucharcol(begin(val), end(val)));
            size_t j = 0;
            for(auto c : linehash)
            {
                for(size_t k = 0; k < VALSZ; ++k)
                {
                    disk.ix(i, j + VALSZ - (k+1)) = c%2;
                    c /= 2;
                }
                j += VALSZ;
            }
        }
        board seen(SZ, SZ);
        size_t res = 0;
        using front = queue<tuple<size_t, size_t>>;
        for(size_t i = 0; i < disk.rows(); ++i)
        {
            for(size_t j = 0; j < disk.columns(); ++j)
            {
                if(seen.ix(i, j) || !disk.ix(i, j))
                    continue;
                ++res;
                seen.ix(i, j) = true;
                front f;
                f.push(make_tuple(i, j));
                while(!f.empty())
                {
                    const auto cur = f.front();
                    f.pop();
                    for(int k = 0; k < 4; ++k)
                    {
                        int dr = (k/2)*(k%2*2 - 1);
                        int dc = (1 - k/2)*(k%2*2-1);
                        const auto nr = static_cast<int>(get<0>(cur)) + dr;
                        const auto nc = static_cast<int>(get<1>(cur)) + dc;
                        if(nr >= 0 && static_cast<size_t>(nr) < disk.rows() &&
                            nc >= 0 && static_cast<size_t>(nc) < disk.columns() &&
                            !seen.ix(nr, nc) && disk.ix(nr, nc))
                        {
                            seen.ix(nr, nc) = true;
                            f.push(make_tuple(nr, nc));
                        }
                    }
                }
            }
        }
        return res;
    }
}
int main()
{
    string h;
    getline(cin, h);
    cout<<groups(h)<<endl;
    return 0;
}
