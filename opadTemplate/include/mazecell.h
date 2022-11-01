#ifndef MAZECELL_H
#define MAZECELL_H

class mazeCell
{
private:

public:
    int val;
    bool explored;
    int prevx;
    int prevy;

    mazeCell(int px, int py)
    {
        if (((px % 2) != 0) && ((py % 2) != 0)) this->val = 0;
        else this->val = 1;

        this->explored = false;
        this->prevx = -1;
        this->prevy = -1;
    }
};


#endif
