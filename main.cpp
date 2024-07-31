#include <iostream>
#include <raylib.h>

using namespace std;

//color the canvas
Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

//global variables for player score and cpu score
int player_score = 0;
int cpu_score = 0;

//update scores
//cpu : wins a point if ball collides on player's wall
//player : wins a point if the ball collides on cpu's wall


class Ball
{
    public :
    //x and y coordinates of the ball
    float x, y;
    //speed of the ball in x and y coordinates
    int speed_x, speed_y;
    //radius of ball
    int radius;

    void Draw()
    {
        DrawCircle(x, y, radius, Yellow);
    }

    // to make the ball move
    void update()
    {
        //the below 2 lines of code  : increase x and y coordinates of the ball object
        //by 7 pixels each time the game loop is run.
        x = x + speed_x;
        y = y + speed_y;

        //code for ball to bounce back and not leave canvas
        if(y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y = speed_y * (-1);
        }
        if(x + radius >= GetScreenWidth()) //cpu wins
        {
            cpu_score++;
            ResetBall();
        }
        if(x - radius <= 0) //player wins
        {
            player_score++;
            ResetBall();
        }
    }

    //call this Reset ball func when a player or cpu scores a point
    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x = speed_x * speed_choices[GetRandomValue(0, 1)];
        speed_y = speed_y * speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle
{
    protected : 
    //func to limit movement of paddle
    void LimitMovement()
    {
        if(y <= 0 )
        {
            y = 0;
        }
        if(y >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
    public :
    //position of paddle or rectangle in the canvas
    float x, y;
    //dimensions of paddle or rectangle ie width, height
    float width, height;
    //speed : only reqd vertical speed, as paddle moves only up and down 
    int speed;

    void Draw()
    {
        // DrawRectangle(x, y, width, height, WHITE);
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void update()
    {
        //we want to move up the paddle when user presses up key on his keyboard
        //using ray lib : isKeyDown func
        if(IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }

        //make sure paddle dont move outside of canvas
        LimitMovement();

    }
};

class CpuPaddle : public Paddle
{
    public : 
    //override update method of Paddle class
    void update(int ball_y)
    {
        //very basic but effective artificial intelligence algorithm
        //we have to check if y position of ball
        //if(y position of ball > y position of centre of paddle)
        //          move the paddle up;
        //else
        //          move the paddle down;

        if(y + height/2 > ball_y)
        {
            y = y - speed;
        }
        if(y + height/2 <= ball_y)
        {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main () {

    cout<<"Starting the game"<<endl;

    const int screen_width = 1280;
    const int screen_height = 800;

    //SetTargetFPS : define how fast the game should run
    //it takes an integer as output which is the no of FPS that we want
    //if we dont define fps, computer will try to run the game as fast as possible
    //so the game speed will depend on the speed of the computer
    //60 means : we want to run while loop of game and update display 60 times per sec
    SetTargetFPS(60);

    //initialise ball object
    ball.radius = 20; //here 20 is in pixels
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 14;
    ball.speed_y = 14;

    //initialise player object of class paddle
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = (screen_height / 2) - (player.height / 2);
    player.speed = 12;

    //initialise cpu object of class CpuPaddle
    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height / 2;
    cpu.speed = 12;


    //Initialise Game Window
    InitWindow(screen_width, screen_height, "My Pong Game");

    //start the game loop with while loop
    //WindowShouldClose()  : checks if escape is pressed or if close icon of window is pressed
    while(WindowShouldClose() == false)
    {
        //BeginDrawing : this func creates a blank canvas, so we
        //can draw our game objects on it
        BeginDrawing();

        //updating
        ball.update();
        player.update();
        cpu.update(ball.y);

        //checking for collisions
        if(CheckCollisionCircleRec(Vector2 {ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            //reverse speed of the ball
            ball.speed_x = ball.speed_x * (-1);
        }

        //do same for cpu paddle as well
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            //reverse speed of the ball
            ball.speed_x = ball.speed_x * (-1);
        }

        //to remove backtraces of ball moving
        ClearBackground(Dark_Green);

        //player half
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height,  Green);
        DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);

        //draw line : to divide 2 sides : begin index(x,y), end indices(x,y), color
        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);

        //Drawing : a ball at centre of canvas
        // DrawCircle(screen_width/2, screen_height/2, 20, WHITE);

        //call Draw method of ball object
        ball.Draw();

        //Drawing : a Rectangle ie paddle 1: width, height
        //note : a rectangle is drawn from its top left point
        // DrawRectangle(10, screen_height/2 - 60, 25, 120, WHITE);
        cpu.Draw();

        //why 35 not 10 : bcz rectangle is drawn from its top left corner
        //and for right paddle : top left coordinate : 1200-10-25
        // DrawRectangle(screen_width-35, screen_height/2 - 60, 25, 120, WHITE);
        player.Draw();

        //display scores
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3*screen_width/4 - 20, 20, 80, WHITE);

        //EndDrawing : func ends the canvas drawing
        EndDrawing();
    }

    //Close Game Window
    CloseWindow();
    return 0;
}