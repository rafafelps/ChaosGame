#include <raylib.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#define RADIUS 350
#define DELAY 0
#define SIDES 6
#define MAX_POINTS 40000

const float ratio = 16.0 / 9.0;
const int height = 720;
const int width = height * ratio;

typedef struct {
    Vector2 pos;
    Color color;
} Point;

void update(Vector2* vertexList, double* lastTime, Point** pointsList, unsigned int* pointsCounter);
void render(Point** pointsList, unsigned int pointsCounter, RenderTexture2D* renderTexture);

int main() {
    InitWindow(width, height, "Chaos Game");
    SetTargetFPS(60);

    Vector2 center = {width * 0.5, height * 0.5};
    float cos30 = cosf(30 * (PI / 180.0));
    Vector2 vertexList[SIDES] = {{center.x - RADIUS, center.y},
                         {center.x - (RADIUS / 2), center.y - (RADIUS * cos30)},
                         {center.x + (RADIUS / 2), center.y - (RADIUS * cos30)},
                         {center.x + RADIUS, center.y},
                         {center.x + (RADIUS / 2), center.y + (RADIUS * cos30)},
                         {center.x - (RADIUS / 2), center.y + (RADIUS * cos30)}};

    Point* pointsList[MAX_POINTS];
    unsigned int pointsCounter = 0;
    pointsList[0] = (Point*) malloc(sizeof(Point));
    pointsList[0]->pos = vertexList[0];
    pointsList[0]->color = BLUE;
    pointsCounter++;

    RenderTexture2D renderTexture = LoadRenderTexture(width, height);
    BeginTextureMode(renderTexture);
    // Hexagon
    DrawPolyLines(center, SIDES, RADIUS, 90, RAYWHITE);
    EndTextureMode();

    double lastTime = GetTime();

    while(!WindowShouldClose()) {
        update(vertexList, &lastTime, pointsList, &pointsCounter);
        render(pointsList, pointsCounter, &renderTexture);
    }

    CloseWindow();

    return 0;
}

void update(Vector2* vertexList, double* lastTime, Point** pointsList, unsigned int* pointsCounter) {
    double currentTime = GetTime();
    if (currentTime - *lastTime >= DELAY) {
        if (*pointsCounter >= MAX_POINTS) { *lastTime = currentTime; return; }
        
        unsigned char currVertex = rand() % 6;

        Vector2 vec;
        vec.x = (vertexList[currVertex].x - pointsList[*pointsCounter - 1]->pos.x) * (2.0 / 3.0) + pointsList[*pointsCounter - 1]->pos.x;
        vec.y = (vertexList[currVertex].y - pointsList[*pointsCounter - 1]->pos.y) * (2.0 / 3.0) + pointsList[*pointsCounter - 1]->pos.y;

        pointsList[*pointsCounter] = (Point*) malloc(sizeof(Point));
        pointsList[*pointsCounter]->pos = vec;
        pointsList[*pointsCounter]->color = BLUE;

        *pointsCounter += 1;

        *lastTime = currentTime;
    }
}

void render(Point** pointsList, unsigned int pointsCounter, RenderTexture2D* renderTexture) {
    if (pointsCounter < MAX_POINTS) {
        BeginTextureMode(*renderTexture);
        DrawCircleV(pointsList[pointsCounter - 1]->pos, 1, pointsList[pointsCounter - 1]->color);
        EndTextureMode();
    }

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexture(renderTexture->texture, 0, 0, WHITE);

    // Counter
    std::string s = std::to_string(pointsCounter);
    DrawText(s.c_str(), 0, height - 20, 20, RAYWHITE);

    EndDrawing();
}
