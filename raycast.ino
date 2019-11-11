#include <Adafruit_GFX.h>        // Graphics library
#include <Adafruit_ST7735.h> // Hardware library
#include <SPI.h>

#define Pyxa_SCLK     13
#define Pyxa_MOSI     11
#define Pyxa_CS         10
#define Pyxa_RST        9
#define Pyxa_DC         8

Adafruit_ST7735 Pyxa = Adafruit_ST7735(Pyxa_CS, Pyxa_DC, Pyxa_RST);

#define ACTIVATED LOW

int buzzer = A1;    //    Buzzer

int button1 = A2; //    Left
int button2 = A3; //    Down
int button3 = A4; //    Right
int button4 = A5; //    Up

int button5 = 7;    //    Y
int button6 = 6;    //    B
int button7 = 5;    //    A
int button8 = 2;    //    X

int button1state = 0;
int button2state = 0;
int button3state = 0;
int button4state = 0;

int button5state = 0;
int button6state = 0;
int button7state = 0;
int button8state = 0;
const float pi = 3.1415927;

const int screenWidth = 128;
const int screenHeight = 160;

float playerX = 3.5;
float playerY = 3.5;
float playerA = 0.0;

float rayAngle;
float distance;
bool hitWall;
float eyeX;
float eyeY;
int testX;
int testY;
int color;

const int game_map[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,0,1,0,0,0,0,1},
    {1,0,0,0,0,1,0,1},
    {1,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,1,1,1,1},
    {1,1,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1},
};

const int mapWidth = sizeof(game_map[0]);
const int mapHeight = sizeof(game_map);

const float fov = pi / 4;
const int depth = 8;

int shade;

void setup() {
    Serial.begin(9600);

    pinMode(A1, OUTPUT); // Set buzzer pin as output

    // Set button pins as inputs

    pinMode(button1, INPUT_PULLUP);
    pinMode(button2, INPUT_PULLUP);
    pinMode(button3, INPUT_PULLUP);
    pinMode(button4, INPUT_PULLUP);

    pinMode(button5, INPUT_PULLUP);
    pinMode(button6, INPUT_PULLUP);
    pinMode(button7, INPUT_PULLUP);
    pinMode(button8, INPUT_PULLUP);

    Pyxa.initR(INITR_BLACKTAB);
    Pyxa.fillScreen(ST7735_BLACK);


}

void loop() {
    for (int x = 0; x < screenWidth; x++) {
        rayAngle = (playerA - fov / 2.0) + ((float)x / (float)screenWidth) * fov;
        distance = 0.0;
        hitWall = false;

        eyeX = cos(rayAngle);
        eyeY = sin(rayAngle);
        while (!hitWall) {
            distance += 0.1;

            testX = floor(playerX + eyeX * distance);
            testY = floor(playerY + eyeY * distance);
            if (testX < 0 | testX >= mapWidth | testY < 0 | testY >= mapHeight) {
                distance = depth;
                hitWall = true;
            }
            if (game_map[testY][testX]) {
                hitWall = true;
            }
        }

        int ceilingHeight = screenHeight / 2 - screenHeight / distance;
        int floorHeight = screenHeight - ceilingHeight;

        if (distance <= depth) {
            color = 0;
            shade = floor((255-color)/(depth/distance)+color);
        }
        else {
            shade = 255;
        }

        Pyxa.drawLine(x,0,x,ceilingHeight-1,Pyxa.color565(255,255,255));
        Pyxa.drawLine(x,ceilingHeight,x,floorHeight,Pyxa.color565(shade,shade,shade));
        Pyxa.drawLine(x,floorHeight+1,x,screenHeight,Pyxa.color565(255,255,255));

//        for (int y = 0; y < screenHeight; y++) {
//            if (y < ceilingHeight) {
//                Pyxa.drawPixel(x, y, Pyxa.color565(255, 255, 255));
//            }
//            else if (y > ceilingHeight and y <= floorHeight) {
//                Pyxa.drawPixel(x, y, Pyxa.color565(shade, shade, shade));
//            }
//            else {
//                Pyxa.drawPixel(x, y, Pyxa.color565(255, 255, 255));
//            }
//        }
    }
    boolean key_hit = false;
    while (!key_hit) {
        button1state = digitalRead(button1);
        button2state = digitalRead(button2);
        button3state = digitalRead(button3);
        button4state = digitalRead(button4);

        button5state = digitalRead(button5);
        button6state = digitalRead(button6);
        button7state = digitalRead(button7);
        button8state = digitalRead(button8);

        if (!button1state) {
            playerA -= 0.1;
            key_hit = true;
        }
        if (!button2state) {
            playerX -= cos(playerA)*0.1;
            playerY -= sin(playerA)*0.1;
            key_hit = true;
        }
        if (!button3state) {
            playerA += 0.1;
            key_hit = true;
        }
        if (!button4state) {
            playerX += cos(playerA)*0.1;
            playerY += sin(playerA)*0.1;
            key_hit = true;
        }
    }

}
