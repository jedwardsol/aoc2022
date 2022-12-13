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



int getInt(std::string_view &walk)
{
    int  i=stoi(walk);

    if(walk[0]==',')
    {
        walk.remove_prefix(1);
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

    walk.remove_prefix(1);  // ']'

    if(   !walk.empty()
       && walk[0]==',')
    {
        walk.remove_prefix(1);
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

        return lhs.size() <=> rhs.size();
    }

    std::strong_ordering    operator()(int const &lhs, int const &rhs)
    {
        return lhs <=> rhs;
    }

    std::strong_ordering    operator()(List const &lhs, int const &rhs)
    {
        return (*this)(lhs, List{rhs});
    }

    std::strong_ordering    operator()(int const &lhs, List const &rhs)
    {
        return (*this)(List{lhs}, rhs);
    }
};


int main()
try
{
    auto const data = getDataLines();

    int sumOfOrderedPackets{};

    for(int i=0;i<data.size();i+=3)
    {
        auto packet1 = getPacket(data[i]);
        auto packet2 = getPacket(data[i+1]);

        auto ordering = ElementComparer{}(packet1 ,packet2);

        if(ordering == std::strong_ordering::less)
        {
            sumOfOrderedPackets+=1+i/3;
        }
    }

    print("Part 1 : {}\n",sumOfOrderedPackets );


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
