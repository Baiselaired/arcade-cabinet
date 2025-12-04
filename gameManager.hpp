#include <stdlib.h>
#include <string.h>


#include "RGBmatrixPanel.h"
#include "Fonts/FreeSans9pt7b.h"


#include "simVec.hpp"        // Used for entities



/**************************************************************************************************
**                                           Constants                                           **
**************************************************************************************************/
#define CANVAS_WIDTH   64
#define CANVAS_HEIGHT  32
#define PLAYER_FENCE   12
#define ENEMY_FENCE    56

const int notes[] = {262, 294, 330, 349, 392, 440, 494, 523};


/**************************************************************************************************
**                                             Pins                                              **
**************************************************************************************************/
// HUB75e -- the matrix
#define R1     25
#define G1     26
#define BL1    24
// Ground      //
#define R2     29
#define G2     28
#define BL2    27
#define PIN_A  A0
#define PIN_B  A1
#define PIN_C  A2
#define PIN_D  A3
#define PIN_E  30
#define CLK    12     // USE THIS ON ARDUINO MEGA
#define LAT    11
#define OE     10
// Ground      //


// Other hardware elements
#define joyX   A4       // Joystick Vertical
#define joyY   A5       // Joystick Horizontal
#define button 31       // Fire button
#define button2 32
#define buzz   33


/**************************************************************************************************
**                                           Utility                                             **
**************************************************************************************************/
struct vec2 {
    uint8_t x, y;

    void operator+=(const vec2& rhs) {
        x += rhs.x;
        y += rhs.y;
    }

    void operator+=(const int& rhs) {
        x += rhs;
    }

    vec2() = default;
    vec2(int xin, int yin){
        x = xin;
        y = yin;
    }
};



/**************************************************************************************************
**                                           Entities                                            **
**************************************************************************************************/
class Entity {
    public:
        vec2 pos;
        vec2 size;
        int color;
        bool living = true;

        bool shuttle;
        bool friendly;

        virtual void update() = 0;

        virtual ~Entity() = default;
};


struct Bullet : public Entity {
    vec2 pos;
    vec2 size = {1, 1};
    int vel;
    int color = 0xfd40;
    bool living = true;
    bool friendly;


    Bullet(const vec2 & origin, const bool fromPlayer) {
        pos = origin;
        friendly = fromPlayer;

        if (fromPlayer){
            vel = 1;
        } else {
            vel = -1;
        }
    }

    void update() {
        pos += vel;
    }

    bool checkCollision(Entity * target){
        if (pos.x >= target->pos.x && pos.x <= (target->pos.x + target->size.x) && pos.y >= target->pos.y && pos.y <= (target->pos.y + target->size.y)){
            living = false;
            target->living = false;
            return true;
        }

        return false;
    }
};


class Player : public Entity {
    public:
        vec2 pos;
        vec2 vel;
        vec2 size = {3, 3};

        bool shuttle = true;
        bool living = true;
        int color = 0x0fff;
        unsigned int score;

        Player() = default;
        Player(vec2 position, vec2 big) {
            pos = position;
            size = big;
        }

        Bullet * shoot() {
            return new Bullet({pos.x + 2, pos.y + 1}, true);
        }

        void update() {
            int xAxis = analogRead(joyX);
            int yAxis = analogRead(joyY);

            if (xAxis > 1000){
                vel.x = 1;
            } else if (xAxis < 50){
                vel.x = -1;
            } else {
                vel.x = 0;
            }

            if (yAxis > 1000){
                vel.y = 1;
            } else if (yAxis < 50){
                vel.y = -1;
            } else {
                vel.y = 0;
            }

            pos += vel;                     // Position update, movement method is unnecessary
            if (pos.x >= PLAYER_FENCE){     // These are just primitive bounds checking
                pos.x = PLAYER_FENCE - 1;       // Will be replaced with checks before even touching position
            }
            
            if (pos.x <= 0){
                pos.x = 1;
            }
            
            if (pos.y >= CANVAS_HEIGHT - 3){
                pos.y = CANVAS_HEIGHT - 4;
            }
            
            if (pos.y <= 0){
                pos.y = 1;
            }

            if (digitalRead(button2)){
                color = 0x07e0;
            } else {
                color = 0x0fff;
            }
        }
};


class Enemy : public Entity {
    private:
        double created;
        double upTime;
        uint8_t age;


        void reColor(){
            switch (age){
                case 1:
                    color = 0xcb3e;
                    break;

                case 2:
                    color = 0xf3cf;
                    break;
            }
        }


    public:
        bool shuttle = false;
        bool living = true;
        int color = 0x565e;
        vec2 pos;
        vec2 size = {3, 3};

        Enemy(vec2 position, double time) {
            pos = position;
            created = time;
        }

        void move(bool y) {             // Make them go up on spawn and back down after -- probably using a bool flag
            if (pos.y > 1 && pos.y < CANVAS_HEIGHT - 1){
                pos.y += y;
            }
        }

        Bullet * shoot() {
            if (age < 4){
                age++;
                reColor();
            }

            return new Bullet({pos.x, pos.y}, false);
        }

        void update() {
            upTime = millis() - created;

            if (upTime >= 5000) {
                shoot();
                created = millis();
            }
        }
};



/**************************************************************************************************
**                                           Game Man                                            **
**************************************************************************************************/
class GameManager {
    private:
        unsigned long startTime = 0;
        unsigned long currTime = 0;
        unsigned long lastTime = 0;
        unsigned long spawnTime = 0;
        unsigned long lastSpawnTime = 0;
        // bool boost       // I really wanna

        RGBmatrixPanel * display;

        Player * shuttle;
        simVec<Bullet *> rounds;
        simVec<Enemy *> enemies;


    public:
        GameManager(RGBmatrixPanel * screen) : display(screen) {}

        ~GameManager(){
            if (display != nullptr){
                delete display;
                display = nullptr;
            }

            delete shuttle;
        }

        void onStart(){
            startTime = millis();

            shuttle = new Player({4, 15}, {3, 3});
        }

        void zoomScope(){               // Take over program loop from main
            while (shuttle->living){
                spawning();
                advance();

                if (millis() - startTime >= 20000){
                    shuttle->living = false;
                    drawGameOver();
                }
            }
        }

        void spawning(){
            if (spawnTime - lastSpawnTime >= 10000){
                spawnTime = currTime;
                enemies.append(new Enemy({58, 15}, currTime));
            }
        }


        /**************************************************************************************************
        **                                       Render-specifics                                        **
        **************************************************************************************************/
        void flood(){
            display->fillScreen(0x0000);       // 565 color format, rgb in 4 hexdec values
        }

        void draw(const Player & target){
            display->drawRect(target.pos.x, target.pos.y, target.size.x, target.size.y, target.color);
        }

        void draw(const Enemy & target){
            display->drawRect(target.pos.x, target.pos.y, target.size.x, target.size.y, target.color);
        }
        
        void draw(const Bullet & target){
            display->drawRect(target.pos.x, target.pos.y, target.size.x, target.size.y, target.color);
        }

        void displayText(int x, int y, char *str, int color, int pixels_size, const GFXfont * f = &FreeSans9pt7b){
            display->setTextSize(pixels_size);// size 1 == 8 pixels high
            display->setTextWrap(false);
            display->setFont(f);      //set font
            display->setCursor(x, y);
            display->setTextColor(color);
            display->println(str);
        }

        void drawStandby(){
            flood();
            displayText(4, 14, "Insert", 0x56bc, 1);     // Might want repeated calls per character
            displayText(4, 30, "Token", 0xed46, 1);
        }

        void drawGameOver(){    // Don't allow handling until maybe 1 second after death occurs
            flood();
            displayText(4, 14, "Game", 0xe000, 1);
            displayText(4, 30, "Over!", 0xe000, 1);
        }

        void advance(){                                        // Updating
            currTime = millis();

            if(currTime - lastTime >= 200){
                lastTime = currTime;
                shuttle->update();
                
                if (!digitalRead(button)){
                    tone(buzz, notes[1]);
                    rounds.append(shuttle->shoot());
                    Serial.println("New bullet");                 // Logging
                    noTone(buzz);
                }

                for (int i = 0; i < enemies.size(); i++){
                    if (enemies[i]->living){
                        enemies[i]->update();
                    } else {
                        enemies.erase(enemies[i]);
                        Serial.println("\nAnother down\n");             // Logging
                        shuttle->score += 100;
                        i--;
                    }
                }

                for (int i = 0; i < rounds.size(); i++){                // Finish updating, start collision checking
                    if (rounds[i]->living){
                        rounds[i]->update();

                        if (!rounds[i]->friendly && rounds[i]->pos.x <= PLAYER_FENCE){        // Position bounding, reduce collision checks
                            rounds[i]->checkCollision(shuttle);
                        }

                        if (rounds[i]->friendly && rounds[i]->pos.x >= ENEMY_FENCE){
                            if (rounds[i]->pos.x >= CANVAS_WIDTH){
                                rounds[i]->living = false;
                                i--;
                                continue;
                            }
                            
                            for (int j = 0; j < enemies.size(); j++){
                                if (rounds[i]->checkCollision(enemies[j])) break;
                            }
                        }
                        
                    } else {
                        rounds.erase(rounds[i--]);
                    }
                }

                flood();
                draw(*shuttle);                                              // Drawing
                for (int i = 0; i < enemies.size(); i++){draw(*enemies[i]);}
                for (int i = 0; i < rounds.size(); i++){draw(*rounds[i]);}
                // display->updateDisplay();
                Serial.print("Number of current entities: ");
                Serial.print(1 + enemies.size() + rounds.size());
                Serial.println();
                Serial.print("Number of bullets: ");
                Serial.println(rounds.size());

            } else {
                ;
            }
        }
};
