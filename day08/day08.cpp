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
    bool visibleFromOutside;
};


struct Pos
{
    int row,column;

    void up()
    {
        row--;
    }

    void down()
    {
        row++;
    }

    void left()
    {
        column--;
    }

    void right()
    {
        column++;
    }



};

struct Forest
{

    bool outside(Pos pos)
    {
        return    pos.row < 0
               || pos.row >= height
               || pos.column < 0
               || pos.column >= width;
    }

    Forest(size_t width, size_t height) : width{width},height{height},trees(width*height, Tree{})
    {}

    auto operator[](size_t row)
    {
        return std::span<Tree>{trees.begin()+(row*width),
                               trees.begin()+((row+1)*width)};
    }

    auto &operator[](Pos pos)
    {
        return trees[ pos.row*width + pos.column];
    }



    size_t              width;
    size_t              height;
    std::vector<Tree>   trees;
};

int main()
try
{
    auto const lines = getDataLines(TestData{});
//    auto const lines = getDataLines();

    auto const size = lines.size();

    Forest forest{size,size};

    for(int row=0;row<size;row++)
    {
        for(int col=0;col<size;col++)
        {
            forest[row][col].height=lines[row][col]-'0';
        }
    }

    for(int i=0;i<size;i++)
    {
        forest[i]     [0].visibleFromOutside        =true;
        forest[i]     [size-1].visibleFromOutside   =true;
        forest[0]     [i].visibleFromOutside        =true;
        forest[size-1][i].visibleFromOutside        =true;
    }
    
    for(int row=1;row<size-1;row++)
    {
        for(int col=1;col<size-1;col++)
        {
            Pos pos{row,col};

            auto height = forest[pos].height;

            while(forest.up(pos))
            {
                if(forest[pos].height >= height)
                {
                    break;
                }
            }

            if(!forest


        }
    }




    auto visible = std::ranges::count_if(forest.trees,[](bool b){return b;}, &Tree::visibleFromOutside);

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
