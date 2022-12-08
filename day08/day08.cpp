#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <span>

struct Tree
{
    int height;
    bool visibleFromLeft;
    bool visibleFromRight;
    bool visibleFromTop;
    bool visibleFromBottom;
};

struct Forest
{
    Forest(size_t size) : size{size},trees(size*size, Tree{})
    {}

    auto operator[](size_t row)
    {
        return std::span<Tree>{trees.begin()+(row*size),
                               trees.begin()+((row+1)*size)};
    }

    size_t              size;
    std::vector<Tree>   trees;


};

int main()
try
{
//    auto const lines = getDataLines(TestData{});
    auto const lines = getDataLines();

    auto const size = lines.size();

    Forest forest{size};

    for(int row=0;row<size;row++)
    {
        for(int col=0;col<size;col++)
        {
            forest[row][col].height=lines[row][col]-'0';
        }
    }

    for(int i=0;i<size;i++)
    {
        forest[i]     [0].visibleFromLeft        =true;
        forest[i]     [size-1].visibleFromRight  =true;
        forest[0]     [i].visibleFromTop         =true;
        forest[size-1][i].visibleFromBottom      =true;
    }
    
    for(int line=0;line<size;line++)
    {
        int max = forest[line][0].height;

        for(int tree=1;tree<size;tree++)
        {
            if(forest[line][tree].height > max)
            {
                forest[line][tree].visibleFromLeft=true;
                max=forest[line][tree].height;
            }
        }

        max = forest[line][size-1].height;

        for(int tree=size-2;tree>=0;tree--)
        {
            if(forest[line][tree].height > max)
            {
                forest[line][tree].visibleFromRight=true;
                max=forest[line][tree].height;
            }
        }



        max = forest[0][line].height;

        for(int tree=1;tree<size;tree++)
        {
            if(forest[tree][line].height > max)
            {
                forest[tree][line].visibleFromTop=true;
                max=forest[tree][line].height;
            }
        }



        max = forest[size-1][line].height;

        for(int tree=size-2;tree>=0;tree--)
        {
            if(forest[tree][line].height > max)
            {
                forest[tree][line].visibleFromBottom=true;
                max=forest[tree][line].height;
            }
        }
    }


    auto visableAtAll = [](Tree const&tree)
    {
        return    tree.visibleFromLeft
               || tree.visibleFromRight
               || tree.visibleFromTop
               || tree.visibleFromBottom;
    };

    for(int row=0;row<size;row++)
    {
        for(int col=0;col<size;col++)
        {
            if(visableAtAll(forest[row][col]))
            {
                print("V");
            }
            else
            {
                print(" ");
            }
        }

        print("\n");
    }


    auto visible = std::ranges::count_if(forest.trees,visableAtAll);

    print("part 1 : {}\n",visible);

}
catch(std::exception const &e)
{
    print("{}",e.what());
}



// --------------------------
std::istringstream testInput{
R"(30373
25512
65332
33549
35390
)"};
