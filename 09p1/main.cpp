#include <iostream>
#include <string>
#include <cassert>

using namespace std;

namespace
{
    using uint = unsigned int;

    class solve
    {
    public:
        solve()
            :score(0)
        {}

        void add(const string &line)
        {
            uint curscore = 0;
            uint cur = 0;
            bool garbage = false;
            bool escape = false;
            for(auto v : line)
            {
                if(!escape)
                {
                    if(v == '!')
                    {
                        escape = true;
                        continue;
                    }
                    if(garbage)
                    {
                        if(v == '>')
                            garbage = false;
                    }
                    else
                    {
                        if(v == '<')
                        {
                            garbage = true;
                        }
                        else if(v == '{')
                        {
                            cur += 1;
                        }
                        else if(v == '}')
                        {
                            assert(cur > 0);
                            curscore += cur;
                            --cur;
                        }
                    }
                }
                else
                {
                    escape = false;
                }
            }
            score += curscore;
        }

        uint getscore() const
        {
            return score;
        }
    private:
        uint score;
    };
}

int main()
{
    solve solver;
    while(true)
    {
        string s;
        getline(cin, s);
        if(cin.eof())
            break;
        solver.add(s);
    }
    cout<<solver.getscore()<<endl;
    return 0;
}
