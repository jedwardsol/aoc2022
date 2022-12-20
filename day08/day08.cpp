#include "include/codeAnalysis.h"
#include "include/print.h"
#include "include/thrower.h"
#include "include/getdata.h"
#include "include/posVector-RC.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <span>

struct Tree
{
    int  height;
    bool visibleFromOutside;
    int  score;
};


using Forest = Grid<Tree>;


bool visibleFromLeft(Forest  &forest, Pos pos)
{
    auto const height = forest[pos].height;

    for(int col=0;col < pos.col;col++)
    {
        if(forest[pos.row][col].height >= height)
        {
            return false;
        }
    }
    
    return true;
}

int visibleToLeft(Forest  &forest, Pos pos) // how many between here and the edge or a tree of >= height
{
    int  count{};
    auto const height = forest[pos].height;

    for(int col=pos.col-1;col >= 0;col--)
    {
        count++;
        if(forest[pos.row][col].height >= height)
        {
            break;
        }
    }
    
    return count;
}




bool visibleFromRight(Forest  &forest, Pos pos)
{
    auto const height = forest[pos].height;

    for(int col=forest.width-1;col > pos.col;col--)
    {
        if(forest[pos.row][col].height >= height)
        {
            return false;
        }
    }

    return true;
}

int visibleToRight(Forest  &forest, Pos pos) // how many between here and the edge or a tree of >= height
{
    int  count{};
    auto const height = forest[pos].height;

    for(int col=pos.col+1;col < forest.width;col++)
    {
        count++;
        if(forest[pos.row][col].height >= height)
        {
            break;
        }
    }
    
    return count;
}




bool visibleFromTop(Forest  &forest, Pos pos)
{
    auto const height = forest[pos].height;

    for(int row=0;row < pos.row;row++)
    {
        if(forest[row][pos.col].height >= height)
        {
            return false;
        }
    }

    return true;
}

int visibleToTop(Forest  &forest, Pos pos) // how many between here and the edge or a tree of >= height
{
    int  count{};
    auto const height = forest[pos].height;

    for(int row=pos.row-1;row >= 0;row--)
    {
        count++;
        if(forest[row][pos.col].height >= height)
        {
            break;
        }
    }
    
    return count;
}



bool visibleFromBottom(Forest  &forest, Pos pos)
{
    auto const height = forest[pos].height;

    for(int row=forest.height-1;row > pos.row;row--)
    {
        if(forest[row][pos.col].height >= height)
        {
            return false;
        }
    }

    return true;
}

int visibleToBottom(Forest  &forest, Pos pos) // how many between here and the edge or a tree of >= height
{
    int  count{};
    auto const height = forest[pos].height;

    for(int row=pos.row+1;row < forest.height;row++)
    {
        count++;
        if(forest[row][pos.col].height >= height)
        {
            break;
        }
    }
    
    return count;
}



int main()
try
{
//    auto const lines = getDataLines(TestData{});
    auto const lines = getDataLines();

    int const size = static_cast<int>(lines.size());

    Forest forest{size,size};

    for(int row=0;row<size;row++)
    {
        for(int col=0;col<size;col++)
        {
            forest[row][col].height=lines[row][col]-'0';
        }
    }

    
    for(int row=0;row<size;row++)
    {
        for(int col=0;col<size;col++)
        {
            Pos  const pos{row,col};

            forest[pos].visibleFromOutside =    visibleFromLeft(forest,pos)
                                             || visibleFromRight(forest,pos)
                                             || visibleFromTop(forest,pos)
                                             || visibleFromBottom(forest,pos);


            forest[pos].score =   visibleToLeft(forest,pos)
                                * visibleToRight(forest,pos)
                                * visibleToTop(forest,pos)
                                * visibleToBottom(forest,pos);
        }
    }

    auto visible = std::ranges::count_if(forest.data,[](bool b){return b;}, &Tree::visibleFromOutside);

    print("part 1 : {}\n",visible);

    auto maxScore = std::ranges::max(forest.data,{}, &Tree::score);

    print("part 2 : {}\n",maxScore.score);

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
