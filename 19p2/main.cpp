#include <iostream>
#include <array>
#include <cctype>
#include <complex>
#include <tuple>
#include <cassert>
#include <cstddef>
#include <vector>
#include <string>

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
    using board = numutil::Matrix<char>;

    size_t solve(const board &b)
    {
        using pos = complex<int>;
        using dirarray = array<pos, 3>;
        assert(b.rows() > 0);
        int beginpos = 0;
        for(; static_cast<size_t>(beginpos) < b.columns(); ++beginpos)
        {
            if(b.ix(0, beginpos) != ' ')
                break;
        }
        assert(static_cast<size_t>(beginpos) < b.columns());
        pos curpos(0, beginpos);
        pos dir(1, 0);
        size_t steps =0;
        while(true)
        {
            ++steps;
            const dirarray nextpositions{dir, dir*pos(0, 1), dir*pos(0, -1)};
            bool found = false;
            for(const auto &nd : nextpositions)
            {
                const auto np = curpos + nd;
                if(np.real() >= 0 && static_cast<size_t>(np.real()) < b.rows() &&
                    np.imag() >= 0 && static_cast<size_t>(np.imag()) < b.columns() &&
                    b.ix(np.real(), np.imag()) != ' ')
                {
                    curpos = np;
                    dir = nd;
                    found = true;
                    break;
                }
            }
            if(!found)
                break;
        }
        return steps;
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
    board b(values.size(), values.front().size());
    for(size_t i = 0; i < values.size(); ++i)
    {
        assert(values[i].size() <= b.columns());
        for(size_t j = 0; j < values[i].size(); ++j)
        {
            b.ix(i, j) = values[i][j];
        }
    }
    cout<<solve(b)<<endl;
    return 0;
}
