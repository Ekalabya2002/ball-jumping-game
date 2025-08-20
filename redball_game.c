 /* jumping_ball_graphics.c
   Graphical ball jump + left/right movement using graphics.h (WinBGIm)
   Controls: SPACE = jump, Arrow keys = move, Q = quit
*/

#include <graphics.h>
#include <conio.h>
#include <stdlib.h>

int main(void) {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");   // "" usually works when libbgi is properly linked

    // Window size
    int maxx = getmaxx();
    int maxy = getmaxy();

    // Ball properties
    float ballX = maxx / 4.0f;
    float ballY = maxy - 60;     // start a bit above ground
    float radius = 20.0f;
    float velY = 0.0f;
    const float gravity = 0.8f;
    const float jumpImpulse = -12.0f;

    // Movement speed
    const float moveSpeed = 10.0f;

    // Ground line
    int groundY = maxy - 30;

    // Frame timing
    const int frameDelayMs = 20; // ~50 FPS

    // Main loop
    while (1) {
        // Input (non-blocking)
        if (kbhit()) {
            int ch = getch();

            // Handle arrow keys (extended codes)
            if (ch == 0 || ch == 224) {
                int arrow = getch();   // read actual arrow key
                if (arrow == 75) {     // Left arrow
                    ballX -= moveSpeed;
                    if (ballX - radius < 0) ballX = radius; // prevent going out of screen
                } else if (arrow == 77) { // Right arrow
                    ballX += moveSpeed;
                    if (ballX + radius > maxx) ballX = maxx - radius;
                }
            }
            else if (ch == ' ' ) {   // SPACE -> jump
                if (ballY + radius >= groundY - 1) {
                    velY = jumpImpulse;
                }
            }
            else if (ch == 'q' || ch == 'Q') {
                break;
            }
        }

        // Physics
        velY += gravity;
        ballY += velY;

        // Collision with ground
        if (ballY + radius > groundY) {
            ballY = groundY - radius;
            velY = 0;
        }

        // Prevent ball going above screen
        if (ballY - radius < 0) {
            ballY = radius;
            velY = 0;
        }

        // Drawing
        cleardevice();

        // Draw ground
        setcolor(WHITE);
        line(0, groundY, maxx, groundY);
        for (int x = 0; x < maxx; x += 40) {
            line(x, groundY, x+20, groundY-6);
        }

        // Draw ball (filled)
        setfillstyle(SOLID_FILL, RED);
        setcolor(BLACK);
        fillellipse((int)ballX, (int)ballY, (int)radius, (int)radius);
        ellipse((int)ballX, (int)ballY, 0, 360, (int)radius, (int)radius);

        // Shadow
        int shadowW = (int)(radius * 1.6f);
        int shadowH = (int)(radius * 0.5f);
        setfillstyle(SOLID_FILL, DARKGRAY);
        fillellipse((int)ballX + 6, groundY + shadowH/2, shadowW, shadowH);

        // Instructions
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(8, 8, "SPACE = jump   Arrows = move   Q = quit");

        delay(frameDelayMs);
    }

    closegraph();
    return 0;
}
