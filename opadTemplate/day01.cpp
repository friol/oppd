
//
// oppd 01 - 01/11/2022
// building a 2D maze with Prim's algorithm
// friol 2k22
//

#include "include/oppd.h"
#include "include/mazecell.h"

bool canGo(std::vector<std::vector<mazeCell>>* m, int dirx, int diry, int px, int py)
{
    if (dirx == 1)
    {
        if ((px + 1) < ((*m)[0].size() - 1))
        {
            if ((*m)[py][px + 1].val == 0) return true;
        }
    }
    if (dirx == -1)
    {
        if ((px - 1) > 0)
        {
            if ((*m)[py][px - 1].val == 0) return true;
        }
    }
    if (diry == 1)
    {
        if ((py + 1) < (m->size() - 1))
        {
            if ((*m)[py + 1][px].val == 0) return true;
        }
    }
    if (diry == -1)
    {
        if ((py - 1) > 0)
        {
            if ((*m)[py - 1][px].val == 0) return true;
        }
    }

    return false;
}

void renderMaze2D(int dx, int dy, std::vector<std::vector<mazeCell>>* m)
{
    for (int r = 0;r < dy;r++)
    {
        for (int c = 0;c < dx;c++)
        {
            if ((*m)[r][c].val == 1)
            {
                if ((r == 0) || (c == 0) || (r == m->size() - 1) || (c == ((*m)[0].size() - 1)))
                {
                    std::wcout << L"▒";
                }
                else
                {
                    std::wcout << L"█";
                }
            }
            else std::wcout << " ";
        }
        std::wcout << std::endl;
    }
}

void addCellWalls(std::vector<std::vector<mazeCell>>* m, std::vector<std::vector<int>>* wl, int px, int py)
{
    if (py > 0)
    {
        if (((*m)[py - 1][px].val) != 0)
        {
            std::vector<int> curWall{ px,py - 1 };
            wl->push_back(curWall);
        }
    }

    if ((py < (m->size() - 1)))
    {
        if (((*m)[py + 1][px].val) != 0)
        {
            std::vector<int> curWall{ px,py + 1 };
            wl->push_back(curWall);
        }
    }

    if (px > 0)
    {
        if (((*m)[py][px - 1].val) != 0)
        {
            std::vector<int> curWall{ px - 1,py };
            wl->push_back(curWall);
        }
    }

    if (px < ((*m)[0].size() - 1))
    {
        if (((*m)[py][px + 1].val) != 0)
        {
            std::vector<int> curWall{ px + 1,py };
            wl->push_back(curWall);
        }
    }
}

// finds rooms adjacent to a wall
void findAdjRooms(std::vector<int>* theWall, std::vector<std::vector<int>>* adjRooms, std::vector<std::vector<mazeCell>>* m)
{
    int wpx = (*theWall)[0];
    int wpy = (*theWall)[1];

    if (wpx > 0)
    {
        if (((*m)[wpy][wpx - 1].val) == 0)
        {
            std::vector<int> newRoom;
            newRoom.push_back(wpx - 1);
            newRoom.push_back(wpy);
            adjRooms->push_back(newRoom);
        }
    }
    if (wpy > 0)
    {
        if (((*m)[wpy - 1][wpx].val) == 0)
        {
            std::vector<int> newRoom;
            newRoom.push_back(wpx);
            newRoom.push_back(wpy - 1);
            adjRooms->push_back(newRoom);
        }
    }
    if (wpx < ((*m)[0].size() - 1))
    {
        if (((*m)[wpy][wpx + 1].val) == 0)
        {
            std::vector<int> newRoom;
            newRoom.push_back(wpx + 1);
            newRoom.push_back(wpy);
            adjRooms->push_back(newRoom);
        }
    }
    if (wpy < (m->size() - 1))
    {
        if (((*m)[wpy + 1][wpx].val) == 0)
        {
            std::vector<int> newRoom;
            newRoom.push_back(wpx);
            newRoom.push_back(wpy + 1);
            adjRooms->push_back(newRoom);
        }
    }
}

bool roomIsInThePath(std::vector<int> room, std::vector<std::vector<int>>* thePath)
{
    for (int e = 0;e < thePath->size();e++)
    {
        if (((*thePath)[e][0] == room[0]) && ((*thePath)[e][1] == room[1])) return true;
    }

    return false;
}

int countRoomsNotInThePath(std::vector<std::vector<int>>* adjRooms, std::vector<std::vector<int>>* thePath)
{
    int numOfRoomsInThePath = 0;

    if (roomIsInThePath((*adjRooms)[0], thePath)) numOfRoomsInThePath++;
    if (roomIsInThePath((*adjRooms)[1], thePath)) numOfRoomsInThePath++;

    return numOfRoomsInThePath;
}

void buildMaze(std::vector<std::vector<mazeCell>>* m, int dimx, int dimy)
{
    std::vector<std::vector<int>> thePath;
    std::vector<std::vector<int>> wallList;

    std::vector<int> inipos{ 1,1 };
    thePath.push_back(inipos);

    srand((unsigned int)time(NULL));
    
    addCellWalls(m, &wallList, 1, 1);

    while (wallList.size() != 0)
    {
        int numWalls = (int)wallList.size();
        int curWall = rand() % numWalls;

        std::vector<std::vector<int>> adjRooms;
        findAdjRooms(&wallList[curWall], &adjRooms, m);

        if ((adjRooms.size() == 2) && (countRoomsNotInThePath(&adjRooms, &thePath) == 1))
        {
            if (roomIsInThePath(adjRooms[0], &thePath))
            {
                thePath.push_back(adjRooms[1]);
                addCellWalls(m, &wallList, adjRooms[1][0], adjRooms[1][1]);
            }
            else
            {
                thePath.push_back(adjRooms[0]);
                addCellWalls(m, &wallList, adjRooms[0][0], adjRooms[0][1]);
            }

            (*m)[wallList[curWall][1]][wallList[curWall][0]].val = 0;
        }

        // remove current wall from the list
        auto vi = wallList.begin();
        for (int p = 0;p < curWall;p++)
        {
            vi++;
        }
        wallList.erase(vi);
    }
}

void bfs(std::vector<std::vector<mazeCell>>* maze,int startposx,int startposy,int endposx,int endposy, std::vector<std::vector<int>>* solution)
{
    int directions[4][2] = { {0,-1},{1,0},{0,1},{-1,0} };
    std::vector<std::vector<int>> queue;

    std::vector<int> root = { startposx,startposy };
    queue.push_back(root);
    (*maze)[startposy][startposx].explored = true;
    (*maze)[startposy][startposx].prevx = -1;
    (*maze)[startposy][startposx].prevy = -1;

    while (!queue.empty())
    {
        int posx=queue[queue.size() - 1][0];
        int posy=queue[queue.size() - 1][1];
        queue.pop_back();

        if ((posx == endposx) && (posy == endposy))
        {
            bool goout = false;
            int curposx = posx;
            int curposy = posy;
            while (true)
            {
                std::vector<int> pos = { curposx,curposy };
                solution->push_back(pos);
                //consoleGotoxy(curposx, curposy);
                //std::wcout << L"x";

                int anewx = (*maze)[curposy][curposx].prevx;
                int anewy = (*maze)[curposy][curposx].prevy;

                curposx = anewx;
                curposy = anewy;

                if ((curposx == startposx) && (curposy == startposy)) return;
            }
            return;
        }

        for (int dir = 0;dir < 4;dir++)
        {
            if (canGo(maze, directions[dir][0], directions[dir][1], posx, posy))
            {
                int newx = posx + directions[dir][0];
                int newy = posy + directions[dir][1];

                if ((*maze)[newy][newx].explored == false)
                {
                    (*maze)[newy][newx].explored = true;
                    (*maze)[newy][newx].prevx=posx;
                    (*maze)[newy][newx].prevy=posy;

                    std::vector<int> newnode= { newx,newy };
                    queue.push_back(newnode);
                }
            }
        }
    }
}

void day01usage(char** argv)
{
    std::wcout << "Usage:" << std::endl << argv[0] << " [easy | normal | hard]" << std::endl;
}

void day01(int argc,char** argv)
{
    std::vector<std::vector<mazeCell>> maze;

    setUnicodeConsole();

    if (argc > 2)
    {
        day01usage(argv);
        return;
    }

    int mazeDimX = 65;
    int mazeDimY = 23;

    if (argc == 1)
    {
        // no arguments, difficulty is "normal"
    }
    else
    {
        if (!strcmp(argv[1], "easy"))
        {
            mazeDimX = 13;
            mazeDimY = 13;
        }
        else if (!strcmp(argv[1], "normal"))
        {

        }
        else if (!strcmp(argv[1], "hard"))
        {
            mazeDimX = 111;
            mazeDimY = 25;
        }
        else
        {
            day01usage(argv);
            return;
        }
    }

    for (int r = 0;r < mazeDimY;r++)
    {
        std::vector<mazeCell> curRow;
        for (int c = 0;c < mazeDimX;c++)
        {
            mazeCell mc(c, r);
            curRow.push_back(mc);
        }
        maze.push_back(curRow);
    }

    buildMaze(&maze, mazeDimX, mazeDimY);

    renderMaze2D(mazeDimX, mazeDimY, &maze);

    int manposx = 1;
    int manposy = 1;
    consoleGotoxy(manposx, manposy);
    std::wcout << L"☺";

    int exitposx = mazeDimX-2;
    int exitposy = mazeDimY-2;
    consoleGotoxy(exitposx,exitposy);
    std::wcout << L"X";

    std::vector<std::vector<int>> solution;
    bfs(&maze, manposx, manposy, exitposx, exitposy,&solution);
    int optimalNumberOfMoves = (int)solution.size();

    // play
    int numberOfMoves = 0;
    bool goOut = false;
    bool win = false;
    while ((!goOut)&&(!win))
    {
        consoleGotoxy(0,mazeDimY+1);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        int ch = _getch();
        if (ch == 27) goOut = true;
        if (ch == 72) // up
        {
            if (canGo(&maze, 0, -1, manposx, manposy))
            {
                consoleGotoxy(manposx, manposy);
                std::wcout << L" ";
                manposy -= 1;
                consoleGotoxy(manposx, manposy);
                std::wcout << L"☺";
            }
            numberOfMoves++;
        }
        else if (ch == 80) // down
        {
            if (canGo(&maze, 0, 1, manposx, manposy))
            {
                consoleGotoxy(manposx, manposy);
                std::wcout << L" ";
                manposy += 1;
                consoleGotoxy(manposx, manposy);
                std::wcout << L"☺";
            }
            numberOfMoves++;
        }
        else if (ch == 75) // left
        {
            if (canGo(&maze, -1, 0, manposx, manposy))
            {
                consoleGotoxy(manposx, manposy);
                std::wcout << L" ";
                manposx -= 1;
                consoleGotoxy(manposx, manposy);
                std::wcout << L"☺";
            }
            numberOfMoves++;
        }
        else if (ch == 77) // right
        {
            if (canGo(&maze, 1, 0, manposx, manposy))
            {
                consoleGotoxy(manposx, manposy);
                std::wcout << L" ";
                manposx += 1;
                consoleGotoxy(manposx, manposy);
                std::wcout << L"☺";
            }
            numberOfMoves++;
        }

        if ((manposx == exitposx) && (manposy == exitposy))
        {
            win = true;
        }
    }

    consoleGotoxy(0, mazeDimY + 1);
    if (!win) std::wcout << "You pressed ESC without finishing." << std::endl;
    else std::wcout << "You reached the exit!" << std::endl;
    std::wcout << "Number of moves: " << numberOfMoves << std::endl;
    std::wcout << "Perfect score: " << optimalNumberOfMoves << std::endl;
    if (numberOfMoves == optimalNumberOfMoves) std::wcout << "You matched the perfect score." << std::endl;
}
