
//
// oppd 02 - 02/11/2022
// navigating a 3D maze
// friol 2k22
//

#include "include/oppd.h"
#include "include/mazecell.h"


void buildMaze(std::vector<std::vector<mazeCell>>* m, int dimx, int dimy);
void renderMaze2D(int dx, int dy, std::vector<std::vector<mazeCell>>* m);
bool canGo(std::vector<std::vector<mazeCell>>* m, int dirx, int diry, int px, int py);

enum day02GameMode
{
    mazemode = 0,
    mapmode = 1
};

void renderMaze3D(int camXpos, int camZpos, int camdir, std::vector<std::vector<mazeCell>>* m, const int fbx, const int fby, int wposx,int wposy)
{
    const int renderingDepth = 10;

    int scx = camXpos;
    int scz = camZpos;

    int zdepth = 6;
    int startHeight = 0;
    int endHeight = fby;
    int rxpos = fbx - 1;
    int lxpos = 0;

    int incrementArray[4][2] =
    {
        {0,-1},
        {1,0},
        {0,1},
        {-1,0}
    };

    wchar_t framebuffer[30][80];

    // floor&sky
    for (int y = 0;y < fby;y++)
    {
        for (int x = 0;x < fbx;x++)
        {
            //consoleGotoxy(x+wposx, y+wposy);
            if (y < fby / 2) framebuffer[y][x] = L' ';// std::wcout << " ";
            else framebuffer[y][x] = L'░'; // std::wcout << L"░";
        }
    }

    // left/right walls
    for (int c = 0;c < renderingDepth;c++)
    {
        for (int col = 0;col < zdepth;col++)
        {
            if ((*m)[scz + incrementArray[camdir][0]][scx - incrementArray[camdir][1]].val == 1)
            {
                for (int l = startHeight;l < endHeight;l++)
                {
                    //consoleGotoxy(rxpos+wposx, l+wposy);
                    //std::wcout << L"▒";
                    framebuffer[l][rxpos] = L'▒';
                }
            }

            if ((*m)[scz - incrementArray[camdir][0]][scx + incrementArray[camdir][1]].val == 1)
            {
                for (int l = startHeight;l < endHeight;l++)
                {
                    //consoleGotoxy(lxpos+wposx, l+wposy);
                    //std::wcout << L"▒";
                    framebuffer[l][lxpos] = L'▒';
                }
            }

            if ((col % 3) == 0)
            {
                startHeight++;
                endHeight--;
            }
            rxpos--;
            lxpos++;
        }
        
        // horz walls - right
        if ((*m)[scz + incrementArray[camdir][0]][scx - incrementArray[camdir][1]].val != 1)
        {
            for (int col = 0;col < zdepth;col++)
            {
                for (int l = startHeight;l < endHeight;l++)
                {
                    //consoleGotoxy(rxpos + 1 + col+wposx, l+wposy);
                    //std::wcout << L"█";
                    framebuffer[l][rxpos + 1 + col] = L'█';
                }
            }
        }

        // horz walls - left
        if ((*m)[scz - incrementArray[camdir][0]][scx + incrementArray[camdir][1]].val != 1)
        {
            for (int col = 0;col < zdepth;col++)
            {
                for (int l = startHeight;l < endHeight;l++)
                {
                    //consoleGotoxy(lxpos - 1 - col+wposx, l+wposy);
                    //std::wcout << L"█";
                    framebuffer[l][lxpos - 1 - col] = L'█';
                }
            }
        }

        // front wall
        if ((*m)[scz+ incrementArray[camdir][1]][scx+ incrementArray[camdir][0]].val == 1)
        {
            for (int l = startHeight;l < endHeight;l++)
            {
                for (int col = lxpos;col < rxpos + 1;col++)
                {
                    //consoleGotoxy(col+wposx, l+wposy);
                    //std::wcout << L"█";
                    framebuffer[l][col] = L'█';
                }
            }

            for (int y = 0;y < fby;y++)
            {
                for (int x = 0;x < fbx;x++)
                {
                    consoleGotoxy(x + wposx, y + wposy);
                    std::wcout << framebuffer[y][x];
                }
            }

            return;
        }

        scx += incrementArray[camdir][0];
        scz += incrementArray[camdir][1];

        if ((scx < 0) || (scz < 0)) return;
        if ((scx > (*m)[0].size() - 1) || (scz > m->size() - 1)) return;

        if ((c % 2) == 0) zdepth--;
    }

    for (int y = 0;y < fby;y++)
    {
        for (int x = 0;x < fbx;x++)
        {
            consoleGotoxy(x+wposx, y+wposy);
            std::wcout << framebuffer[y][x];
        }
    }

    consoleGotoxy(0+wposx,fby+1+wposy);
}

void day02()
{
    // maze generation
    int mazeDimX = 65;
    int mazeDimY = 23;

    std::vector<std::vector<mazeCell>> maze;
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

    //

    const int renderingAreaX = 80;
    const int renderingAreaY = 30;

    int camXpos = 1;
    int camZpos = 1;
    unsigned int camdir = 1; // 0=north, 1=right, 2=south, 3=left
    int goalPosx = mazeDimX - 2;
    int goalPosy = mazeDimY - 2;

    int windowPosx = 0;
    int windowPosy = 0;

    renderMaze3D(camXpos, camZpos, camdir, &maze, renderingAreaX, renderingAreaY, windowPosx, windowPosy);

    int incrementArray[4][2] =
    {
        {0,-1},
        {1,0},
        {0,1},
        {-1,0}
    };

    wchar_t arrows[4] = { L'^',L'>',L'v',L'<'};

    consoleGotoxy(0, renderingAreaY + 1);
    std::wcout << L"Press 'm' to switch to map" << std::endl;

    day02GameMode gameMode = mazemode;
    bool win = false;
    bool goOut = false;
    while (!goOut)
    {
        consoleGotoxy(0,renderingAreaY+1);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        int ch = _getch();
        if (ch == 27) goOut = true;

        if (gameMode == mazemode)
        {
            if (ch == 72) // up
            {
                if (canGo(&maze, incrementArray[camdir][0], incrementArray[camdir][1], camXpos, camZpos))
                {
                    camXpos += incrementArray[camdir][0];
                    camZpos += incrementArray[camdir][1];
                }

                renderMaze3D(camXpos, camZpos, camdir, &maze, renderingAreaX, renderingAreaY, windowPosx, windowPosy);
            }
            else if (ch == 77) // right
            {
                camdir += 1; camdir %= 4;
                renderMaze3D(camXpos, camZpos, camdir, &maze, renderingAreaX, renderingAreaY, windowPosx, windowPosy);
            }
            else if (ch == 75) // left
            {
                camdir -= 1; camdir %= 4;
                renderMaze3D(camXpos, camZpos, camdir, &maze, renderingAreaX, renderingAreaY, windowPosx, windowPosy);
            }
            else if (ch==109) // "m"
            {
                gameMode = mapmode;
                clearScreen();
                consoleGotoxy(0, 0);
                renderMaze2D(mazeDimX, mazeDimY, &maze);
                consoleGotoxy(camXpos, camZpos);
                std::wcout << arrows[camdir];
                consoleGotoxy(goalPosx, goalPosy);
                std::wcout << L"X";
                consoleGotoxy(0, mazeDimY + 1);
                std::wcout << L"Press 'm' to switch to 3D maze" << std::endl;
            }
        }
        else if (gameMode == mapmode)
        {
            if (ch == 109)
            {
                gameMode = mazemode;
                renderMaze3D(camXpos, camZpos, camdir, &maze, renderingAreaX, renderingAreaY, windowPosx, windowPosy);
                consoleGotoxy(0, renderingAreaY + 1);
                std::wcout << L"Press 'm' to switch to map" << std::endl;
            }
        }

        if ((camXpos == goalPosx) && (camZpos == goalPosy))
        {
            goOut = true;
            win = true;
        }
    }

    consoleGotoxy(0, renderingAreaY + 2);
    if (!win)
    {
        std::wcout << "You pressed ESC." << std::endl;
    }
    else
    {
        std::wcout << "*** Maze completed! ***" << std::endl;
    }
}
