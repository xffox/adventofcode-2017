#include <iostream>
#include <cassert>
#include <sstream>
#include <regex>
#include <string>
#include <array>
#include <unordered_map>

using namespace std;

namespace
{
    using statemap = unordered_map<int, int>;
    struct transition
    {
        int val;
        int step;
        char state;
    };
    using condtransition = array<transition, 2>;
    using transmap = unordered_map<char, condtransition>;

    int parse_value(const string &s)
    {
        stringstream ss(s);
        int res = 0;
        ss>>res;
        return res;
    }

    string find(const regex &reg, const string &s)
    {
        smatch match;
        const auto r = regex_search(s, match, reg);
        assert(r && match.size() == 2);
        return match.str(1);
    }

    int run(const transmap &trans, int steps, char begin_state)
    {
        int pos = 0;
        char state = begin_state;
        statemap states;
        for(int i =0; i < steps; ++i)
        {
            const auto &t = trans.at(state);
            auto &v = states[pos];
            assert(v == 0 || v == 1);
            state = t[v].state;
            const auto step = t[v].step;
            v= t[v].val;
            pos += step;
        }
        int res = 0;
        for(const auto &p : states)
            res += p.second;
        return res;
    }
}

int main()
{
    regex begin_state_regex("Begin in state (.)");
    regex checksum_regex("Perform a diagnostic checksum after ([0-9]*) steps");
    regex from_state_regex("In state (.)");
    regex current_value_regex("If the current value is ([0-9])");
    regex write_regex("Write the value ([0-9])");
    regex move_regex("Move one slot to the ([a-z]*)");
    regex continue_state_regex("Continue with state (.)");
    string s;
    getline(cin, s);
    auto begin_state = find(begin_state_regex, s)[0];
    getline(cin, s);
    auto steps = parse_value(find(checksum_regex, s));
    transmap trans;
    while(true)
    {
        getline(cin, s);
        if(cin.eof())
            break;
        if(s.empty())
            continue;
        auto from_state = find(from_state_regex, s)[0];
        condtransition cnd{transition{}, transition{}};
        for(int i=0; i < 2; ++i)
        {
            getline(cin, s);
            auto value = parse_value(find(current_value_regex, s));
            getline(cin, s);
            auto write = parse_value(find(write_regex, s));
            getline(cin, s);
            auto move_dir = find(move_regex, s);
            getline(cin, s);
            auto cont = find(continue_state_regex, s)[0];
            cnd[value] = transition{write, (move_dir == "left"?-1:1), cont};
        }
        trans[from_state] = move(cnd);
    }
    cout<<run(trans, steps, begin_state)<<endl;
    return 0;
}
