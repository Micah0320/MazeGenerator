//Name: Micah Arndt
//File Name: main.cpp

#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "Maze.h"
#include "Stack.h"

//ALGORITHM: build_maze
//INPUT: (r,c) - the starting point


//let UNVISITED be the container of unvisited cells:
//initially this should be all cells except for (r,c)

//let PATH be the container containing only (r,c)

//let VISITED be a container of visited cells:
//initially this should contain only (r,c)

//let PUNCHED be an empty container of "(r0,c0) -- (r1,c1)"
//that represents punched walls.
//WARNING: If "(r0,c0) -- (r1,c1)" is in this container.
//then "(r1,c1) -- (r0,c0)" is also in the container.

//while PATH is not empty:
//look at the last step stored in PATH -- call it x
//look at all the surrounding cells around x
//the available cells to go to are the
//cells to the north, south, east, west of x
//which are within the grid and not visited yet,
//i.e., in UNVISITED.
//if there is at least one available cell:
//randomly choose one available cell -- call it y
//store y in VISITED and add that to PATH
//remove y from UNVISITED
//store x--y (the punched wall) in PUNCHED; y--x
//should be there too
//else:
//there are are no available cells
//we have to go backwards -- remove x from PATH



// The maze is n-by-n and (r,c) is the starting point of the
// maze.
std::vector< PunchedWall > build_maze(int n,
int r, int c);
void print_maze(int n, const std::vector<PunchedWall> & v);

//Functions for Creating the Maze
void chooseDirection(int n, std::vector< Cell* > &,
                     std::vector< Cell* > &, std::vector< PunchedWall > &,
                     Stack< Cell* > &);
bool inVisited(Cell &, std::vector< Cell* > &);
bool inPunched(PunchedWall &, const std::vector< PunchedWall > &);
int find(Cell & c, std::vector< Cell * >collection);





int main()
{
    srand((unsigned int) time(NULL));
// Maze is in a grid of n-by-n cells. The starting point is (r, c).
    int n, r, c;
    std::cin >> n >> r >> c;
    if (n > 0)
    {
    std::vector< PunchedWall > punched_walls = build_maze(n, r,  c);
    print_maze(n, punched_walls);
    }

    else
    {
        std::cout << "Your size must be positive!" << std::endl;
    }
    return 0;
}

std::vector< PunchedWall > build_maze(int n,
int r, int c)
{
    std::vector< PunchedWall >punchedWall;
    std::vector< Cell* >UNVISITED;
    std::vector< Cell* >VISITED;
    std::vector< Cell * >CELLS;
    Stack< Cell* >PATH;
    //std::vector< Cell* > PUNCHED;

    int k = 0;
    for (int i = 0; i < n; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            if (i == r && j == c)
            {
                VISITED.push_back(new Cell(i, j));
                CELLS.push_back(VISITED[0]);
                PATH.push(VISITED[0]);
            }
            else
            {
                UNVISITED.push_back(new Cell(i, j));
                CELLS.push_back(UNVISITED[k]);
                ++k;
            }
        }
    }
    while(punchedWall.size() < (n * n) && UNVISITED.size() > 0)
    {
            chooseDirection(n, VISITED, UNVISITED, punchedWall, PATH);
            PATH.pop();
            //std::cout << punchedWall.size() << ' ' << std::endl;
            //std::cout << PATH.size() << ' ' << std::endl;
    }
    //std::cout << punchedWall.size() << ' ' <<n * n << std::endl;
    return punchedWall;
}

void print_maze(int n, const std::vector<PunchedWall> & v)
{
    /*for (int i = 0; i < v.size(); ++i)
    {
        std::cout << v[i].c0 << ' ' << v[i].c1 << std::endl;
        }*/
    for(int x = 0; x < n; ++x)
    {
        if(x == 0)
        {
            for (int y = 0; y < n; ++y)
            {
                    std::cout << "+-";
            }
            std::cout << "+\n";
        }
            for (int y = 0; y < n - 1; ++y)
            {
                Cell it(x, y);
                Cell it2(x, y + 1);
                PunchedWall p(it, it2);
                PunchedWall p2(it2, it);
                /*
                if (x == n - 1)
                {
                    std::cout << it << ' ' << it2 << std::endl;
                    std::cout << inPunched(p, v) << ' '<<inPunched(p, v) <<std::endl;
                    continue;
                    }*/
                if (!y)
                {
                    std::cout << '|';
                }
                if (inPunched(p, v) || inPunched(p2,v))
                {
                    std::cout << "  ";
                }
                else
                    std::cout << " |";
            }
            std::cout << " |" << std::endl;
            for (int y = 0; y < n; ++y)
            {
                Cell it(x, y);
                Cell it2(x + 1, y);
                PunchedWall p(it, it2);
                PunchedWall p2(it2, it);
                if (inPunched(p, v) ||inPunched(p2, v))
                    std::cout << "+ ";
                else
                    std::cout << "+-";
            }
            std::cout << "+\n";
    }
}


void chooseDirection(int n, std::vector< Cell* > & Visited,
                     std::vector< Cell* > & Unvisited,
                     std::vector< PunchedWall > & Punched,
                     Stack< Cell* > & path)
{
    int x = path.top()->c();
    int y = path.top()->r();

    bool northBlock = 0;
    bool southBlock = 0;
    bool eastBlock = 0;
    bool westBlock = 0;

    Cell temp(x, y);

    while(1)
    {
        int direction = rand() % 4;
        //std::cout << direction <<std::endl;
        switch(direction)
        {
            //North
            case 0:
                temp.r() = path.top()->FindNRow();
                temp.c() = path.top()->FindNCol();
                //std::cout << temp << std::endl;
                if (temp.r() > -1 && inVisited(temp, Unvisited))
                {
                    Punched.push_back(PunchedWall(*(path.top()), temp));
                    path.push(new Cell(temp));
                    Visited.push_back(path.top());
                    Unvisited.erase(Unvisited.begin() +
                                    find(temp, Unvisited));
                }
                else
                    northBlock = 1;
                break;
                //South
            case 1:
                temp.r() = path.top()->FindSRow();
                temp.c() = path.top()->FindSCol();
                //std::cout << temp << std::endl;
                if (temp.r() < n && inVisited(temp, Unvisited))
                {
                    Punched.push_back(PunchedWall(*(path.top()), temp));
                    path.push(new Cell(temp));
                    Visited.push_back(path.top());
                    Unvisited.erase(Unvisited.begin() +
                                    find(temp, Unvisited));
                }
                else
                    southBlock = 1;
                break;
                //East
            case 2:
                temp.r() = path.top()->FindERow();
                temp.c() = path.top()->FindECol();
                
                // std::cout << temp << std::endl;
                if (temp.c() < n && inVisited(temp, Unvisited))
                {
                    Punched.push_back(PunchedWall(*(path.top()), temp));
                    path.push(new Cell(temp));
                    Visited.push_back(path.top());
                    Unvisited.erase(Unvisited.begin() +
                                    find(temp, Unvisited));
                }
                else
                    eastBlock = 1;
                break;
                //West
            case 3:
                temp.r() = path.top()->FindWRow();
                temp.c() = path.top()->FindWCol();
                //std::cout << temp << std::endl;
                if (temp.c() > -1 && inVisited(temp, Unvisited))
                {
                    Punched.push_back(PunchedWall(*(path.top()), temp));
                    path.push(new Cell(temp));
                    Visited.push_back(path.top());
                    Unvisited.erase(Unvisited.begin() +
                                    find(temp, Unvisited));
                }
                else
                    westBlock = 1;
                break;
        }
        //std::cout << Punched.size() << std::endl;
        //If everything is blocked, end this function
        if (northBlock && southBlock
            && eastBlock && westBlock)
        {
            //std::cout << "everything is blocked" <<std::endl;
            return;
        }
        
    }
    
    
}

bool inVisited(Cell & c, std::vector< Cell* > &visited)
{
    for (int i = 0; i < visited.size(); ++i)
    {
        if(c.r() == visited[i]->r() &&
           c.c() == visited[i]->c())
        {
            return 1;
        }
    }
    return 0;
}

bool inPunched(PunchedWall &p, const std::vector< PunchedWall> &con)
{
    for (int i = 0; i < con.size(); ++i)
    {
        if (p.c0.r() == con[i].c0.r() &&
            p.c0.c() == con[i].c0.c() &&
            p.c1.r() == con[i].c1.r() &&
            p.c1.c() == con[i].c1.c())
            return 1;
    }
    return 0;
}
int find(Cell & c, std::vector< Cell * >collection)
{
    for (int i = 0; i < collection.size(); ++i)
    {
        if(c.r() == collection[i]->r() &&
           c.c() == collection[i]->c())
        {
            return i;
        }
    }
}
