#ifndef GUARD
#define GUARD

#define SCREEN_HEIGHT 650
#define SCREEN_WIDTH 650
#define CUBE_WIDTH 10

// mapsize needs to be an uneven number
#define MAPSIZE 65
#define ALL_CELLS_VISITED (((MAPSIZE - 1) / 2) * ((MAPSIZE - 1) / 2))

#define BODY_RAD 5
#define raycount 64

#define Colour Color

struct Locloc
{
    int x;
    int y;
};

struct Collisions
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool deadend;
};

#endif