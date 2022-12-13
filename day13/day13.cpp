#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/stringstuff.h"

#include <vector>
#include <variant>
#include <string>
#include <string_view>
#include <cassert>
#include <compare>

class Element;
using List = std::vector<Element>;

class Element : public std::variant<int,List>
{
public:
    using Base = std::variant<int,List>;
    using Base::Base;
    using Base::operator=;
};

// Parse

// would be neater with proper tokeniser
int getInt(std::string_view &walk)
{
    int  i=stoi(walk);                      // consumes the digits

    if(walk[0]==',')
    {
        walk.remove_prefix(1);              // ','
    }

    return i;
}

List  getList(std::string_view &walk)
{
    List ret;

    while(walk[0]!=']')
    {
        if(walk[0]=='[')
        {
            walk.remove_prefix(1);          // '['
            ret.push_back(getList(walk));
        }
        else
        {
            ret.push_back(getInt(walk));
        }
    }

    walk.remove_prefix(1);                  // ']'

    if(  !walk.empty()
       && walk[0]==',')
    {
        walk.remove_prefix(1);              // ','
    }

    return ret;
}

// A packet is always a list
List  getPacket(std::string const &line)
{
    std::string_view    walk{line};

    assert(walk[0] == '['); 
    walk.remove_prefix(1);  // '['

    return getList(walk);
}


// Compare

struct ElementComparer
{
    std::strong_ordering    operator()(List const &lhs, List const &rhs)
    {
        auto extent = std::min(lhs.size(), rhs.size());

        for(int i=0;i<extent;i++)
        {
            auto result = std::visit(*this,lhs[i],rhs[i]);;

            if(result != std::strong_ordering::equal)
            {
                return result;
            }
        }

        // equal up to now.  shorter lists are less than longer lists
        return lhs.size() <=> rhs.size();
    }

    std::strong_ordering    operator()(int const &lhs, int const &rhs)
    {
        return lhs <=> rhs;
    }

    // mixed : promote the int to a list containing just the int
    std::strong_ordering    operator()(List const &lhs, int const &rhs)
    {
        return (*this)(lhs, List{rhs});
    }

    std::strong_ordering    operator()(int const &lhs, List const &rhs)
    {
        return (*this)(List{lhs}, rhs);
    }
};


std::strong_ordering operator<=>(List const &lhs, List const &rhs)
{
    return ElementComparer{}(lhs,rhs);
}


int main()
try
{
    auto const data = getDataLines();


    std::vector<List>       packets;

    for(int i=0;i<data.size();i+=3)
    {
        packets.push_back(getPacket(data[i]));
        packets.push_back(getPacket(data[i+1]));
    }


// part1 

    int sumOfOrderedPackets{};

    for(int i=0;i<packets.size();i+=2)
    {
        auto const &packet1 = packets[i];
        auto const &packet2 = packets[i+1];

        // "ordered" from the puzzle means "less than" here
        if (packet1 < packet2 )
        {
            sumOfOrderedPackets+=   1+i/2;              // indices are 1-based
        }
    }

    print("Part 1 : {}\n",sumOfOrderedPackets );

// part2

    auto const divider1 = getPacket("[[2]]");
    auto const divider2 = getPacket("[[6]]");

    packets.push_back(divider1);
    packets.push_back(divider2);

    std::ranges::sort(packets);

    auto pos1 = 1+std::distance(packets.begin(), std::ranges::find(packets,divider1));
    auto pos2 = 1+std::distance(packets.begin(), std::ranges::find(packets,divider2));

    print("Part 2 : {}\n",pos1*pos2);
}
catch(std::exception const &e)
{
    print("{}",e.what());
}


#include <sstream>

// --------------------------
std::istringstream testInput{
R"([1,1,3,1,1]
[1,1,5,1,1]

[[1],[2,3,4]]
[[1],4]

[9]
[[8,7,6]]

[[4,4],4,4]
[[4,4],4,4,4]

[7,7,7,7]
[7,7,7]

[]
[3]

[[[]]]
[[]]

[1,[2,[3,[4,[5,6,7]]]],8,9]
[1,[2,[3,[4,[5,6,0]]]],8,9]
)"};
