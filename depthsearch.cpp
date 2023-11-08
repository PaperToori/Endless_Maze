#include "/opt/homebrew/Cellar/raylib/4.5.0/include/raylib.h"
#include "define.cpp"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void Draw(short map[][MAPSIZE], short points);

// Initialise variabel information
void Init(short map[][MAPSIZE], Locloc *player, short *points)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Sisyphus' Maze");
    SetWindowPosition(40, 40);
    // SetTargetFPS(100);

    // Initial Values
    (*player).x = 1;
    (*player).y = 1;
    (*points) = 0;

    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {
            map[i][j] = 0;
        }
    }

    map[(*player).x][(*player).y] = 1;
}

void CheckEmptyCells(short map[][MAPSIZE], Locloc position, Collisions *walls)
{
    // Reset
    (*walls).up = false;
    (*walls).down = false;
    (*walls).left = false;
    (*walls).right = false;
    (*walls).deadend = false;

    // Up
    if (1 > (position.y - 2))
    {
        (*walls).up = true;
        // printf("Top wall hit \t\t%i, %i\n", position.x, position.y);
    }
    else if (2 == map[position.x][position.y - 2])
    {
        (*walls).up = true;
        // printf("Space above unavailable\t%i, %i\n", position.x, position.y);
    }

    // Down
    if (MAPSIZE - 2 < position.y + 2)
    {
        (*walls).down = true;
        // printf("Bottom wall hit\t\t%i, %i\n", position.x, position.y);
    }
    if (2 == map[position.x][position.y + 2])
    {
        (*walls).down = true;
        // printf("Space below unavailable\t%i, %i\n", position.x, position.y);
    }

    // Left
    if (1 > position.x - 2)
    {
        (*walls).left = true;
        // printf("Left wall hit\t\t%i, %i\n", position.x, position.y);
    }
    if (2 == map[position.x - 2][position.y])
    {
        (*walls).left = true;
        // printf("Left space unavailable\t%i, %i\n", position.x, position.y);
    }

    // Right
    if (MAPSIZE - 2 < position.x + 2)
    {
        (*walls).right = true;
        // printf("Right wall hit\t\t%i, %i\n", position.x, position.y);
    }
    if (2 == map[position.x + 2][position.y])
    {
        (*walls).right = true;
        // printf("Right space unavailable\t%i, %i\n", position.x, position.y);
    }

    // if every path is blocked
    if ((*walls).up &&
        (*walls).down &&
        (*walls).left &&
        (*walls).right)
    {
        (*walls).deadend = true;
        // printf("Deadend\t\t%i, %i\n", position.x, position.y);
    }

    return;
}

void StepBack(Locloc *positionHistory, int *stepsTaken, Locloc *position,
              Locloc *ends, int *totalEnds, bool *end)
{

    // If it is at the end of a possible path, then it will remember the finishing point.
    if (*end)
    {
        *end = false;
        ends[*totalEnds].x = (*position).x;
        ends[*totalEnds].y = (*position).y;
        (*totalEnds)++;
    }

    // takes a step back in the log, and updates position to match
    (*stepsTaken) -= 1;
    (*position).x = positionHistory[(*stepsTaken) - 1].x;
    (*position).y = positionHistory[(*stepsTaken) - 1].y;
}

// Performs the depth-search algorithm updates map
void GenerateMaze(short map[][MAPSIZE], Locloc player, Locloc *goal, short *points)
{

    // Reset map
    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {
            map[i][j] = 0;
        }
    }

    srand(time(NULL));

    int randomNumber = 0;

    // Edges, and the count of how many
    Locloc ends[ALL_CELLS_VISITED];
    int totalEnds = 0;
    bool end = true;

    // Position history, and visited cells
    Locloc positionHistory[ALL_CELLS_VISITED];
    int stepsTaken = 0;
    int visitedCells = 0;
    bool moved = false;

    // Current position and check unvisited cells
    Collisions walls;
    Locloc position;
    position.x = player.x;
    position.y = player.y;

    // Set player position to be the first visited cells
    map[position.x][position.y] = 2;
    positionHistory[stepsTaken].x = player.x;
    positionHistory[stepsTaken].y = player.y;
    stepsTaken++;
    visitedCells++;

    while (ALL_CELLS_VISITED > visitedCells)
    {
        /* Values 0 - 3 are equivalent to:
            0 = up
            1 = right
            2 = down
            3 = left
            (like a clock) */
        randomNumber = rand() % 4;
        moved = false;

        CheckEmptyCells(map, position, &walls);

        if (walls.deadend)
        {
            StepBack(positionHistory, &stepsTaken, &position, ends, &totalEnds, &end);
            continue;
        }

        switch (randomNumber)
        {
        case 0:
        {
            if (walls.up)
            {
                randomNumber = 1;
                break;
            }

            position.y -= 2;
            map[position.x][position.y + 1] = 2;
            map[position.x][position.y] = 2;

            end = true;
            moved = true;

            break;
        }
        case 1:
        {
            if (walls.right)
            {
                randomNumber = 2;
                break;
            }

            position.x += 2;
            map[position.x - 1][position.y] = 2;
            map[position.x][position.y] = 2;

            end = true;
            moved = true;

            break;
        }
        case 2:
        {
            if (walls.down)
            {
                randomNumber = 3;
                break;
            }

            position.y += 2;
            map[position.x][position.y - 1] = 2;
            map[position.x][position.y] = 2;

            end = true;
            moved = true;

            break;
        }
        case 3:
        {
            if (walls.left)
            {
                randomNumber = 0;
                break;
            }

            position.x -= 2;
            map[position.x + 1][position.y] = 2;
            map[position.x][position.y] = 2;

            end = true;
            moved = true;

            break;
        }
        }

        if (moved)
        {
            positionHistory[stepsTaken].x = position.x;
            positionHistory[stepsTaken].y = position.y;
            stepsTaken++;
            visitedCells++;
        }

        Draw(map, *points);
    }
    map[player.x][player.y] = 1;
    randomNumber = rand() % totalEnds;
    map[ends[randomNumber].x][ends[randomNumber].y] = -1;
    (*goal).x = ends[randomNumber].x;
    (*goal).y = ends[randomNumber].y;
}

void InputAndUpdate(short map[][MAPSIZE], Locloc *player, Locloc *goal, short *points)
{
    if (IsKeyPressed(KEY_UP))
    {
        if (0 < (*player).y)
        {
            if (0 == map[(*player).x][(*player).y - 1])
            {
                printf("Hit wall\n");
            }
            else
            {
                map[(*player).x][(*player).y] = 2;
                (*player).y--;
                map[(*player).x][(*player).y] = 1;
            }
        }
        else
            printf("Too low!  %i\n", (*player).y);
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        if (MAPSIZE - 1 > (*player).y)
        {
            if (0 == map[(*player).x][(*player).y + 1])
            {
                // printf("Hit wall\n");
            }
            else
            {
                map[(*player).x][(*player).y] = 2;
                (*player).y++;
                map[(*player).x][(*player).y] = 1;
            }
        }
        else
            printf("Too high! %i\n", (*player).y);
    }
    if (IsKeyPressed(KEY_LEFT))
    {
        if (0 < (*player).x)
        {
            if (0 == map[(*player).x - 1][(*player).y])
            {
                printf("Hit wall\n");
            }
            else
            {
                map[(*player).x][(*player).y] = 2;
                (*player).x--;
                map[(*player).x][(*player).y] = 1;
            }
        }
        else
            printf("Left wall\n");
    }
    if (IsKeyPressed(KEY_RIGHT))
    {
        if (MAPSIZE - 1 > (*player).x)
        {
            if (0 == map[(*player).x + 1][(*player).y])
            {
                printf("Hit wall\n");
            }
            else
            {
                map[(*player).x][(*player).y] = 2;
                (*player).x++;
                map[(*player).x][(*player).y] = 1;
            }
        }
        else
            printf("Right wall\n");
    }
    if ((*player).x == (*goal).x && (*player).y == (*goal).y)
    {
        printf("You win!\n");
        (*points)++;
        GenerateMaze(map, *player, goal, points);
    }
}

// Draws the map and player position
void Draw(short map[][MAPSIZE], short points)
{
    BeginDrawing();
    ClearBackground(WHITE);

    for (int i = 0; i < MAPSIZE; i++)
    {
        for (int j = 0; j < MAPSIZE; j++)
        {

            if (0 == map[i][j])
                DrawRectangle(i * CUBE_WIDTH,
                              j * CUBE_WIDTH,
                              CUBE_WIDTH,
                              CUBE_WIDTH,
                              DARKGRAY);
            else if (1 == map[i][j])
            {
                DrawRectangle(i * CUBE_WIDTH,
                              j * CUBE_WIDTH,
                              CUBE_WIDTH,
                              CUBE_WIDTH,
                              LIGHTGRAY);
                DrawRectangle(i * CUBE_WIDTH + 1,
                              j * CUBE_WIDTH + 1,
                              CUBE_WIDTH - 2,
                              CUBE_WIDTH - 2,
                              BLACK);
            }
            else if (-1 == map[i][j])
                DrawRectangle(i * CUBE_WIDTH,
                              j * CUBE_WIDTH,
                              CUBE_WIDTH,
                              CUBE_WIDTH,
                              RED);
            else
                DrawRectangle(i * CUBE_WIDTH,
                              j * CUBE_WIDTH,
                              CUBE_WIDTH,
                              CUBE_WIDTH,
                              LIGHTGRAY);
        }
    }

    DrawText(TextFormat("%02i", points),
             22,
             50,
             550,
             (Colour){100, 100, 100, 150});

    EndDrawing();
}