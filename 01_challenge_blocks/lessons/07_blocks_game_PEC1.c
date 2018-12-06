/*******************************************************************************************
*
*   PEC1:           BLOCKS GAME
*   Alumno:         Pere Bertran Sole
*   Asignatura:     Programación Grafica
*
*   Compile example using:
*       gcc -o $(NAME_PART).exe $(FILE_NAME) -lraylib -lopengl32 -lgdi32 -Wall -std=c99
*
*   This example has been created using raylib 2.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2017-2018 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"


//----------------------------------------------------------------------------------
// Useful values definitions 
//----------------------------------------------------------------------------------
#define PLAYER_LIFES             5
#define PLAYER_LIFES_            3

#define BRICKS_LINES             5
#define BRICKS_PER_LINE         20

#define BRICKS_LINES_            3
#define BRICKS_PER_LINE_         2

#define BRICKS_POSITION_Y       50

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------


// LESSON 01: Window initialization and screens management
typedef enum GameScreen { LOGO, TITLE, ONE_PLAYER, TWO_PLAYERS, ENDING } GameScreen;

// Player structure
typedef struct Player {
    Vector2 position;
    Vector2 speed;
    Vector2 size;
    Rectangle bounds;
    int lifes;
} Player;

// Ball structure
typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    int radius;
    bool active;
} Ball;

// Bricks structure
typedef struct Brick {
    Vector2 position;
    Vector2 size;
    Rectangle bounds;
    int resistance;
    bool active;
} Brick;

// Bricks_ structure TWO_PLAYERS
typedef struct Brick_ {
    Vector2 position;
    Vector2 size;
    Rectangle bounds;
    bool active;
} Brick_;

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    //Fade Alpha
    float transAlpha = 0.0f;
    
    //title_pos
    int title_pos = 0;
    
    //score variables
    int score = 0;
    int score_ = 100;
    static int highScore;

    
    //two players variables
    bool two_players = false;
    bool player1_ = true;


    // LESSON 01: Window initialization and screens management
    InitWindow(screenWidth, screenHeight, "GP_PEC1 Pere Bertran : BLOCKS GAME");
    
    // NOTE: Load resources (textures, fonts, audio) after Window initialization
    
    // LESSON 05: Textures loading and drawing
    Texture2D texLogo = LoadTexture("resources/pec1_logo.png");
    Texture2D texBall = LoadTexture("resources/ball.png");
    Texture2D texPaddle = LoadTexture("resources/paddle.png");
    Texture2D texPaddle1 = LoadTexture("resources/paddle1.png");
    Texture2D texBrick = LoadTexture("resources/brick.png");
    
    // LESSON 06: Fonts loading and text drawing
    Font font = LoadFont("resources/SaucerBB.ttf");
    Font font1 = LoadFont("resources/setback.png");
    
    // LESSSON 07: Sounds and music loading and playing
    InitAudioDevice();              // Initialize audio system
    
    Sound fxStart = LoadSound("resources/start.wav");
    Sound fxBounce = LoadSound("resources/bounce.wav");
    Sound fxExplode = LoadSound("resources/explosion.wav");
    
    Music music = LoadMusicStream("resources/blockshock.mod");
    
    PlayMusicStream(music);         // Start music streaming

    // Game required variables
    GameScreen screen = LOGO;       // Current game screen state
    
    int framesCounter = 0;          // General pourpose frames counter
    int gameResult = -1;            // Game result: 0 - Loose, 1 - Win, -1 - Not defined
    bool gamePaused = false;        // Game paused state toggle
    
    // NOTE: Check defined structs on top
    Player player = { 0 };
    Player player1 = { 0 };
    Player player2 = { 0 };
    Ball ball = { 0 };
    Brick bricks[BRICKS_LINES][BRICKS_PER_LINE] = { 0 };
    Brick_ bricks_[BRICKS_LINES_][BRICKS_PER_LINE_] = { 0 };
    
    // Initialize player
    player.position = (Vector2){ screenWidth/2, screenHeight*7/8 };
    player.speed = (Vector2){ 8.0f, 0.0f };
    player.size = (Vector2){ 100, 24 };
    player.lifes = PLAYER_LIFES;
    
    // Initialize player1
    player1.position = (Vector2){ screenWidth*1/14, screenHeight/2 };
    player1.speed = (Vector2){ 8.0f, 8.0f };
    player1.size = (Vector2){ 24, 100 };
    player1.lifes = PLAYER_LIFES_;
    
    // Initialize player2
    player2.position = (Vector2){ screenWidth*10/11, screenHeight/2 };
    player2.speed = (Vector2){ 8.0f, 8.0f };
    player2.size = (Vector2){ 24, 100 };
    player2.lifes = PLAYER_LIFES_;
    
    // Initialize ball
    ball.radius = 10.0f;
    ball.active = false;
    ball.position = (Vector2){ player.position.x + player.size.x/2, player.position.y - ball.radius*2 };
    ball.speed = (Vector2){ 4.0f, 4.0f };

    // Initialize bricks
    for (int j = 0; j < BRICKS_LINES; j++)
    {
        for (int i = 0; i < BRICKS_PER_LINE; i++)
        {
            bricks[j][i].size = (Vector2){ screenWidth/BRICKS_PER_LINE, 20 };
            bricks[j][i].position = (Vector2){ i*bricks[j][i].size.x, j*bricks[j][i].size.y + BRICKS_POSITION_Y };
            bricks[j][i].bounds = (Rectangle){ bricks[j][i].position.x, bricks[j][i].position.y, bricks[j][i].size.x, bricks[j][i].size.y };
            bricks[j][i].active = true;
        }
    }

    
        // Initialize bricks_ TWO_PLAYERS
    int p=0;
    for (int j = 0; j < BRICKS_LINES_; j++)
    {
        for (int i = 0; i < BRICKS_PER_LINE_; i++)
        {
            if(i==0){
            bricks_[j][i].size = (Vector2){ 20, 149 };
            bricks_[j][i].position = (Vector2){ 10, p };
            bricks_[j][i].bounds = (Rectangle){ bricks_[j][i].position.x, bricks_[j][i].position.y, bricks_[j][i].size.x, bricks_[j][i].size.y };
            bricks_[j][i].active = true;
            }else{
            bricks_[j][i].size = (Vector2){ 20, 149 };
            bricks_[j][i].position = (Vector2){ 780, p };
            bricks_[j][i].bounds = (Rectangle){ bricks_[j][i].position.x, bricks_[j][i].position.y, bricks_[j][i].size.x, bricks_[j][i].size.y };
            bricks_[j][i].active = true;  
            }
        }
        p+=150;
    }

        
    SetTargetFPS(60);               // Set desired framerate (frames per second)
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(screen) 
        {
            case LOGO: 
            {
                // Update LOGO screen data here!
                
                framesCounter++;
                if (framesCounter <240){
                    transAlpha +=0.005f;
                }else{
                    transAlpha -=0.005f;
                }
            ;
                if (framesCounter >480) 
                {
                    screen = TITLE;    // Change to TITLE screen after 3 seconds
                    framesCounter = 0;
                }
                
            } break;
            case TITLE: 
            {
                // Update TITLE screen data here!
                
                if(title_pos<121) title_pos +=1;
                
                framesCounter++;
                

                // LESSON 03: Inputs management (keyboard, mouse)
                if (IsKeyPressed('Q'))
                {
                    screen = ONE_PLAYER;
                    PlaySound(fxStart);
                }
                if (IsKeyPressed('W'))
                {
                    screen = TWO_PLAYERS;
                    PlaySound(fxStart);
                }
            
            } break;
            case ONE_PLAYER:
            { 
                two_players = false;
                // Update GAMEPLAY screen data here!
                
                // LESSON 03: Inputs management (keyboard, mouse)
                if (IsKeyPressed('P')) gamePaused = !gamePaused;    // Pause button logic

                if (!gamePaused)
                {
                    // LESSON 03: Inputs management (keyboard, mouse)
                    
                    // Player movement logic
                    if (IsKeyDown(KEY_LEFT)) player.position.x -= player.speed.x;
                    if (IsKeyDown(KEY_RIGHT)) player.position.x += player.speed.x;
                    
                    if ((player.position.x) <= 0) player.position.x = 0;
                    if ((player.position.x + player.size.x) >= screenWidth) player.position.x = screenWidth - player.size.x;
                    
                    player.bounds = (Rectangle){ player.position.x, player.position.y, player.size.x, player.size.y };

                    if (ball.active)
                    {
                        // Ball movement logic
                        ball.position.x += ball.speed.x;
                        ball.position.y += ball.speed.y;
                        
                        // Collision logic: ball vs screen-limits
                        if (((ball.position.x + ball.radius) >= screenWidth) || ((ball.position.x - ball.radius) <= 0)) ball.speed.x *= -1;
                        if ((ball.position.y - ball.radius) <= 0) ball.speed.y *= -1;
                        
                        // LESSON 04: Collision detection and resolution
                        
                        // NOTE: For collisions we consider elements bounds parameters, 
                        // that's independent of elements drawing but they should match texture parameters
                        
                        // Collision logic: ball vs player
                        if (CheckCollisionCircleRec(ball.position, ball.radius, player.bounds))
                        {
                            ball.speed.y *= -1;
                            ball.speed.x = (ball.position.x - (player.position.x + player.size.x/2))/player.size.x*5.0f;
                            PlaySound(fxBounce);
                        }
                        
                        // Collision logic: ball vs bricks
                        for (int j = 0; j < BRICKS_LINES; j++)
                        {
                            for (int i = 0; i < BRICKS_PER_LINE; i++)
                            {
                                if (bricks[j][i].active && (CheckCollisionCircleRec(ball.position, ball.radius, bricks[j][i].bounds)))
                                {
                                    bricks[j][i].active = false;
                                    ball.speed.y *= -1;
                                    PlaySound(fxExplode);
                                    score ++;
                                    if(highScore < score) highScore = score;
                                    break;
                                }
                            }
                        }

                        // Game ending logic
                        if ((ball.position.y + ball.radius) >= screenHeight)
                        {
                            ball.position.x = player.position.x + player.size.x/2;
                            ball.position.y = player.position.y - ball.radius - 1.0f;
                            ball.speed = (Vector2){ 0, 0 };
                            ball.active = false;

                            player.lifes--;
                        }
                        
                        if (player.lifes < 0)
                        {
                            screen = ENDING;
                            player.lifes = 5;
                            framesCounter = 0;
                            gamePaused = 0;
                            score = 0;
                            gameResult = 0;
                        }
                    }
                    else
                    {
                        // Reset ball position
                        ball.position.x = player.position.x + player.size.x/2;
                        
                        // LESSON 03: Inputs management (keyboard, mouse)
                        if (IsKeyPressed(KEY_SPACE))
                        {
                            // Activate ball logic
                            ball.active = true;
                            ball.speed = (Vector2){ 0, -5.0f };
                        }
                    }
                    if(score == score_ ){
                        gameResult = 1;
                        score_+=100;
                        screen = ENDING;
                        
                    }
                }
                
            } break;
            case TWO_PLAYERS:
            {
                two_players = true;
                if (IsKeyPressed('P')) gamePaused = !gamePaused;    // Pause button logic

                if (!gamePaused)
                {
                    // LESSON 03: Inputs management (keyboard, mouse)
                    
                    // Player1 movement logic
                    if (IsKeyDown('A')) player1.position.y -= player1.speed.y;
                    if (IsKeyDown('Z')) player1.position.y += player1.speed.y;
                    
                    if ((player1.position.y) <= 0) player1.position.y = 0;
                    if ((player1.position.y + player1.size.y) >= screenHeight) player1.position.y = screenHeight - player1.size.y;
                    
                    player1.bounds = (Rectangle){ player1.position.x, player1.position.y, player1.size.x, player1.size.y };
                    
                    // Player2 movement logic
                    if (IsKeyDown('K')) player2.position.y -= player2.speed.y;
                    if (IsKeyDown('M')) player2.position.y += player2.speed.y;
                    
                    if ((player2.position.y) <= 0) player2.position.y = 0;
                    if ((player2.position.y + player2.size.y) >= screenHeight) player2.position.y = screenHeight - player2.size.y;
                    
                    player2.bounds = (Rectangle){ player2.position.x, player2.position.y, player2.size.x, player2.size.y };
                
                    if(ball.active){
                        //Ball moviment logic
                        ball.position.x += ball.speed.x;
                        ball.position.y += ball.speed.y;
                        
                        // Collision logic: ball vs screen-limits
                        if (((ball.position.y + ball.radius) >= screenHeight) || ((ball.position.y - ball.radius) <= 0)) ball.speed.y *= -1;
                        //if ((ball.position.y - ball.radius) <= 0) ball.speed.y *= -1;
                        
                        // Collision logic: ball vs players
                        if (CheckCollisionCircleRec(ball.position, ball.radius, player1.bounds))
                        {
                            ball.speed.x *= -1;
                            ball.speed.y = (ball.position.y - (player1.position.y + player1.size.y/2))/player1.size.y*5.0f;
                            PlaySound(fxBounce);
                        }
                        
                        if (CheckCollisionCircleRec(ball.position, ball.radius, player2.bounds))
                        {
                            ball.speed.x *= -1;
                            ball.speed.y = (ball.position.y - (player2.position.y + player2.size.y/2))/player2.size.y*5.0f;
                            PlaySound(fxBounce);
                        }
                        
                        // Collision logic: ball vs bricks
                        for (int j = 0; j < BRICKS_LINES_; j++)
                        {
                            for (int i = 0; i < BRICKS_PER_LINE_; i++)
                            {
                                if (bricks_[j][i].active && (CheckCollisionCircleRec(ball.position, ball.radius, bricks_[j][i].bounds)))
                                {
                                    bricks_[j][i].active = false;
                                    ball.speed.x *= -1;
                                    PlaySound(fxExplode);
                                    break;
                                }
                            }
                        }
                        
                        // Game ending logic
                        if ((ball.position.x + ball.radius) >= screenWidth || (ball.position.x + ball.radius) <= 0 )
                        {
                            if(ball.position.x + ball.radius>= screenWidth){
                                player2.lifes--;
                            }else{
                                player1.lifes--;
                            }
                            ball.position.x = screenWidth/2;
                            ball.position.y = screenHeight/2;
                            ball.speed = (Vector2){ 0, 0 };
                            ball.active = false;                            
                        }
                        
                        if (player1.lifes < 0 || player2.lifes < 0 )
                        {
                            screen = ENDING;
                            if(player2.lifes>0){
                                player1_=true;
                            }else{
                                player1_=false;
                            } 
                            player1.lifes = 3;
                            player2.lifes = 3;
                            framesCounter = 0;
                            gamePaused = 0;
                        }
                    }
                    else
                    {
                        // Reset ball position
                        ball.position.x = screenWidth/2;
                        ball.position.y = screenHeight/2;
                        // LESSON 03: Inputs management (keyboard, mouse)
                        if (IsKeyPressed(KEY_SPACE))
                        {
                            // Activate ball logic
                            ball.active = true;
                            ball.speed = (Vector2){ -5.0f, 2.0f };
                        }
                        }
                }
                break;
            }
            case ENDING: 
            {
                // Update END screen data here!
                
                framesCounter++;
                
                // LESSON 03: Inputs management (keyboard, mouse)
                if (IsKeyPressed(KEY_ENTER))
                {
                    // Replay / Exit game logic
                    if(two_players || score < 100){
                        screen = TITLE;
                        gameResult=-1;
                    }else{
                        screen = ONE_PLAYER;
                        
                    }
                }
                    // GameOver logic
 
 
                //restard brikcs
                
                for (int j = 0; j < BRICKS_LINES; j++)
                {
                    for (int i = 0; i < BRICKS_PER_LINE; i++)
                    {
                        bricks[j][i].size = (Vector2){ screenWidth/BRICKS_PER_LINE, 20 };
                        bricks[j][i].position = (Vector2){ i*bricks[j][i].size.x, j*bricks[j][i].size.y + BRICKS_POSITION_Y };
                        bricks[j][i].bounds = (Rectangle){ bricks[j][i].position.x, bricks[j][i].position.y, bricks[j][i].size.x, bricks[j][i].size.y };
                        bricks[j][i].active = true;
                    }
                }
                
                //restard ball
                ball.position.x = player.position.x + player.size.x/2;
                ball.position.y = player.position.y - ball.radius - 1.0f;
                ball.speed = (Vector2){ 0, 0 };
                ball.active = false;
                
                // Restard bricks_ TWO_PLAYERS
                int p=0;
                for (int j = 0; j < BRICKS_LINES_; j++)
                {
                    for (int i = 0; i < BRICKS_PER_LINE_; i++)
                    {
                        if(i==0){
                        bricks_[j][i].size = (Vector2){ 20, 149 };
                        bricks_[j][i].position = (Vector2){ 10, p };
                        bricks_[j][i].bounds = (Rectangle){ bricks_[j][i].position.x, bricks_[j][i].position.y, bricks_[j][i].size.x, bricks_[j][i].size.y };
                        bricks_[j][i].active = true;
                        }else{
                        bricks_[j][i].size = (Vector2){ 20, 149 };
                        bricks_[j][i].position = (Vector2){ 780, p };
                        bricks_[j][i].bounds = (Rectangle){ bricks_[j][i].position.x, bricks_[j][i].position.y, bricks_[j][i].size.x, bricks_[j][i].size.y };
                        bricks_[j][i].active = true;  
                        }
                    }
                    p+=150;
                }
                
            } break;
            default: break;
        }
        
        // LESSSON 07: Sounds and music loading and playing
        // NOTE: Music buffers must be refilled if consumed
        UpdateMusicStream(music);
        //----------------------------------------------------------------------------------
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
            ClearBackground(RAYWHITE);
            
            switch(screen) 
            {
                case LOGO: 
                {
                    // Draw LOGO screen here!
                    
                    // LESSON 05: Textures loading and drawing)
                    DrawTexture(texLogo, screenWidth/2 - texLogo.width/2, screenHeight/2 - texLogo.height/2, Fade(WHITE,transAlpha));
                    DrawText("PEC1 PROGRAMACIÓ GRÀFICA",100,350,30, BLUE);
                    DrawText("Pere Bertran Sole",100,400,20, BLUE);
                    
                } break;
                case TITLE: 
                {
                    // Draw TITLE screen here!
                    ClearBackground(SKYBLUE);
                    // LESSON 06: Fonts loading and text drawing
                    DrawTextEx(font, "BLOCKS", (Vector2){ 100, title_pos }, 120, 10, MAROON);   // Draw Title

                    if(framesCounter>120){
                        if ((framesCounter/30)%2 == 0) DrawText("SELECT GAME MODE:", GetScreenWidth()/2 - MeasureText("SELECT GAME MODE:", 20)/2, GetScreenHeight()/2 + 60, 20, DARKGRAY);
                        DrawText("PRESS 'Q' FOR ONE_PLAYER", GetScreenWidth()/2- MeasureText("PRESS 'Q' FOR ONE_PLAYER", 20)/2 ,GetScreenHeight()/2 + 80,20, DARKGRAY);
                        DrawText("PRESS 'W' FOR TWO_PLAYER", GetScreenWidth()/2- MeasureText("PRESS 'W' FOR TWO_PLAYER", 20)/2 ,GetScreenHeight()/2 + 100,20, DARKGRAY);
                    }
                } break;
                case ONE_PLAYER:
                { 
                    // Draw GAMEPLAY screen here!
                    DrawText(FormatText("SCORE: %i", score), 20, 20, 30, BLUE);
                    DrawText(FormatText("HIGHSCORE: %i", highScore), 520, 20, 30, RED);
                    //DrawText(("SCORE: %i", score),100,20,30, BLUE);
                    
                    #define LESSON05_TEXTURES         // Alternative: LESSON02_SHAPES
                    #if defined(LESSON02_SHAPES)
                        // LESSON 02: Draw basic shapes (circle, rectangle)
                        DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, BLACK);   // Draw player bar
                        DrawCircleV(ball.position, ball.radius, MAROON);    // Draw ball
                        
                        // Draw bricks                        {

                        for (int j = 0; j < BRICKS_LINES; j++)
                            for (int i = 0; i < BRICKS_PER_LINE; i++)
                            {
                                if (bricks[j][i].active)
                                {
                                    if ((i + j)%2 == 0) DrawRectangle(bricks[j][i].position.x, bricks[j][i].position.y, bricks[j][i].size.x, bricks[j][i].size.y, GRAY);
                                    else DrawRectangle(bricks[j][i].position.x, bricks[j][i].position.y, bricks[j][i].size.x, bricks[j][i].size.y, DARKGRAY);
                                }
                            }
                        }
                    #elif defined(LESSON05_TEXTURES)
                        // LESSON 05: Textures loading and drawing
                        DrawTextureEx(texPaddle, player.position, 0.0f, 1.0f, WHITE);   // Draw player
                        
                        DrawTexture(texBall, ball.position.x - ball.radius/2, ball.position.y - ball.radius/2, MAROON);    // Draw ball
                    
                        // Draw bricks
                        for (int j = 0; j < BRICKS_LINES; j++)
                        {
                            for (int i = 0; i < BRICKS_PER_LINE; i++)
                            {
                                if (bricks[j][i].active)
                                {
                                    // NOTE: Texture is not scaled, just using original size
                                    
                                    if ((i + j)%2 == 0) DrawTextureEx(texBrick, bricks[j][i].position, 0.0f, 1.0f, GRAY);
                                    else DrawTextureEx(texBrick, bricks[j][i].position, 0.0f, 1.0f, DARKGRAY);
                                }
                            }
                        }
                    #endif
                    
                    // Draw GUI: player lives
                    for (int i = 0; i < player.lifes; i++) DrawRectangle(20 + 40*i, screenHeight - 30, 35, 10, LIGHTGRAY);

                    // Draw pause message when required
                    if (gamePaused) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 + 60, 40, GRAY);
                } break;
                
                case TWO_PLAYERS:
                {
                        for (int j = 0; j < BRICKS_LINES_; j++)
                        {
                            for (int i = 0; i < BRICKS_PER_LINE_; i++)
                            {
                                if (bricks_[j][i].active)
                                {
                                    if ((i + j)%2 == 0) DrawRectangle(bricks_[j][i].position.x, bricks_[j][i].position.y, bricks_[j][i].size.x, bricks_[j][i].size.y, GRAY);
                                    else DrawRectangle(bricks_[j][i].position.x, bricks_[j][i].position.y, bricks_[j][i].size.x, bricks_[j][i].size.y, DARKGRAY);
                                }
                            }
                        }
                       
                    #define LESSON05_TEXTURES         // Alternative: LESSON02_SHAPES
                    #if defined(LESSON02_SHAPES)
                        //draw players
                        DrawRectangle(player2.position.x, player2.position.y, player2.size.x, player2.size.y, BLACK);   // Draw player bar
                        DrawRectangle(player1.position.x, player1.position.y, player1.size.x, player1.size.y, BLACK);   // Draw player bar
                        //Draw ball
                        DrawCircleV(ball.position, ball.radius, MAROON); 
                        

                    #elif defined(LESSON05_TEXTURES)
                        // LESSON 05: Textures loading and drawing
                        DrawTextureEx(texPaddle1, player1.position, 0.0f, 1.0f, GREEN);   // Draw player
                        DrawTextureEx(texPaddle1, player2.position, 0.0f, 1.0f, BLUE);   // Draw player
                        DrawTexture(texBall, ball.position.x - ball.radius/2, ball.position.y - ball.radius/2, MAROON);
                    #endif
                    
                                        
                    // Draw GUI: player lives
                    for (int i = 0; i < player1.lifes; i++) DrawRectangle(50 + 40*i, 30, 35, 10, GREEN);
                    for (int i = 0; i < player2.lifes; i++) DrawRectangle(650 + 40*i, 30, 35, 10, BLUE);
                    // Draw pause message when required
                    if (gamePaused) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 + 60, 40, GRAY);
                    
                }break;
                case ENDING: 
                {
                    // Draw END screen here!
                    
                    // LESSON 06: Fonts loading and text drawing
                    // Draw ending message
                    DrawTextEx(font, "GAME FINISHED", (Vector2){ 30, 100 }, 80, 6, MAROON);
                    
                    if(!two_players){
                        if(gameResult == 1){
                            DrawText("Let's Rock, baby!",screenWidth/2 - MeasureText("Let's Rock, baby!", 40)/2,200,50, RED);
                        }
                        else{
                            DrawText("You lost!",screenWidth/2 - MeasureText("You lost!", 40)/2,200,50, RED);
                    }
                    }else{
                        if(!player1_){
                            DrawTextEx(font1,"Player 1 WIN",(Vector2){230,200},30,10, GREEN);
                        }else{
                            DrawTextEx(font1,"Player 2 WIN",(Vector2){230,200},30,10, BLUE);
                        }
                    }

                    if ((framesCounter/30)%2 == 0) DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 + 80, 20, GRAY);
                    
                } break;
                default: break;
            }
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    
    // NOTE: Unload any loaded resources (texture, fonts, audio)
    
    // LESSON 05: Textures loading and drawing
    UnloadTexture(texBall);
    UnloadTexture(texPaddle);
    UnloadTexture(texBrick);
    
    // LESSON 06: Fonts loading and text drawing
    UnloadFont(font);
    
    // LESSSON 07: Sounds and music loading and playing
    UnloadSound(fxStart);
    UnloadSound(fxBounce);
    UnloadSound(fxExplode);
    
    UnloadMusicStream(music);   // Unload music streamming buffers
    
    CloseAudioDevice();         // Close audio device connection
    
    CloseWindow();              // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    
    return 0;
}

