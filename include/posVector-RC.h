#pragma once

struct Vector
{
    int dRow{};
    int dCol{};

    Vector  &operator+=(Vector const &rhs)
    {
        dRow+=rhs.dRow;
        dCol+=rhs.dCol;
        return *this;
    }

    Vector  &operator-=(Vector const &rhs)
    {
        dRow-=rhs.dRow;
        dCol-=rhs.dCol;
        return *this;
    }

    friend bool operator== (Vector const &lhs,Vector const &rhs)=default;
    friend bool operator!= (Vector const &lhs,Vector const &rhs)=default;
    friend auto operator<=>(Vector const &lhs,Vector const &rhs)=default;
};


struct Pos
{
    int row{};                           
    int col{};                           

    Pos  &operator+=(Vector const &rhs)
    {
        row+=rhs.dRow;
        col+=rhs.dCol;
        return *this;
    }

    Pos  &operator-=(Vector const &rhs)
    {
        row-=rhs.dRow;
        col-=rhs.dCol;
        return *this;
    }

    friend bool operator== (Pos const &lhs,Pos const &rhs)=default;
    friend bool operator!= (Pos const &lhs,Pos const &rhs)=default;
    friend auto operator<=>(Pos const &lhs,Pos const &rhs)=default;
};


Vector operator+(Vector  lhs,Vector const &rhs)
{
    return lhs+=rhs;
}

Vector operator-(Vector  lhs,Vector const &rhs)
{
    return lhs-=rhs;
}


Pos operator+(Pos  lhs,Vector const &rhs)
{
    return lhs+=rhs;
}

Pos operator-(Pos  lhs,Vector const &rhs)
{
    return lhs-=rhs;
}


Pos operator+(Vector const &lhs,Pos rhs)
{
    return rhs+=lhs;
}




Vector operator-(Pos const &lhs,Pos const &rhs)
{
    return {lhs.row-rhs.row,lhs.col-rhs.col};
}


