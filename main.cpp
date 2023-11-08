#include "depthsearch.cpp"
#include "define.cpp"

/*--- Variables ---*/

short map[MAPSIZE][MAPSIZE];
Locloc player;
Locloc goal;
short points;

/*--- Main      ---*/
int main(void)
{
    Init(map, &player, &points);

    GenerateMaze(map, player, &goal, &points);

    while (!WindowShouldClose())
    {

        InputAndUpdate(map, &player, &goal, &points);

        Draw(map, points);
    }

    return 0;
}