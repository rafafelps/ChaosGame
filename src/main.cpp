#include <raylib.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

#define RADIUS 350
#define DELAY 0.0
#define SIDES 5
#define MAX_POINTS 40000
#define RULE (1.0 / 1.618033988749)

const float ratio = 16.0 / 9.0;
const int height = 720;
const int width = height * ratio;

void update(Vector2* vertexList, double* lastTime, Vector2* pointsList, unsigned int* pointsCounter);
void render(Vector2* pointsList, unsigned int pointsCounter, RenderTexture2D* renderTexture);

int main() {
    InitWindow(width, height, "Chaos Game");
    SetTargetFPS(60);

    Vector2 center = {width * 0.5, height * 0.5};
    Vector2 vertexList[SIDES];
    for (int i = 0; i < SIDES; i++) {
        float theta = PI / 2.0 - i * (2 * PI / SIDES);
        vertexList[i].x = center.x + RADIUS * cosf(theta);
        vertexList[i].y = center.y - RADIUS * sinf(theta);
    }

    Vector2 pointsList[MAX_POINTS];
    unsigned int pointsCounter = 0;
    pointsList[0] = vertexList[0];
    pointsCounter++;

    RenderTexture2D renderTexture = LoadRenderTexture(width, height);
    BeginTextureMode(renderTexture);
    // Hexagon
    DrawPolyLines(center, SIDES, RADIUS, 180, RAYWHITE);
    EndTextureMode();

    double lastTime = GetTime();

    while(!WindowShouldClose()) {
        update(vertexList, &lastTime, pointsList, &pointsCounter);
        render(pointsList, pointsCounter, &renderTexture);
    }

    CloseWindow();

    return 0;
}

void update(Vector2* vertexList, double* lastTime, Vector2* pointsList, unsigned int* pointsCounter) {
    double currentTime = GetTime();
    if (currentTime - *lastTime >= DELAY) {
        if (*pointsCounter >= MAX_POINTS) { *lastTime = currentTime; return; }
        
        unsigned char currVertex = rand() % SIDES;

        Vector2 vec;
        vec.x = (vertexList[currVertex].x - pointsList[*pointsCounter - 1].x) * RULE + pointsList[*pointsCounter - 1].x;
        vec.y = (vertexList[currVertex].y - pointsList[*pointsCounter - 1].y) * RULE + pointsList[*pointsCounter - 1].y;

        pointsList[*pointsCounter] = vec;

        *pointsCounter += 1;

        *lastTime = currentTime;
    }
}

void render(Vector2* pointsList, unsigned int pointsCounter, RenderTexture2D* renderTexture) {
    if (pointsCounter < MAX_POINTS) {
        BeginTextureMode(*renderTexture);
        DrawCircleV(pointsList[pointsCounter - 1], 1, BLUE);
        EndTextureMode();
    }

    BeginDrawing();
    ClearBackground(BLACK);

    // NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
    DrawTextureRec(renderTexture->texture, (Rectangle) { 0, 0, (float)renderTexture->texture.width, (float)-renderTexture->texture.height }, (Vector2) { 0, 0 }, WHITE);

    // Counter
    std::string s = std::to_string(pointsCounter);
    DrawText(s.c_str(), 0, height - 20, 20, RAYWHITE);

    EndDrawing();
}
