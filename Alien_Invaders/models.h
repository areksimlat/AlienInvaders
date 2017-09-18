#ifndef MODELS_H_INCLUDED
#define MODELS_H_INCLUDED

#include "obj_loader.h"
#include <math.h>
#include <time.h>

#define PLAYER_PATH     "models/player/player.obj"
#define ALIEN_PATH      "models/alien/alien.obj"
#define CITY_PATH       "models/city/city.obj"
#define SPHERE_PATH     "models/sphere/sphere.obj"
#define EARTH_PATH      "models/texture/earth.png"
#define UNIVERSE_PATH   "models/texture/universe.jpg"
#define MOON_PATH       "models/texture/moon.jpg"
#define RUN_TEX         "models/run.jpg"
#define PLAYER_WIN_TEX  "models/player_win.jpg"
#define ALIEN_WIN_TEX   "models/alien_win.jpg"
#define ALIEN_AMOUNT_X  6
#define ALIEN_AMOUNT_Y  3
#define EXPLODE_FRAG    100



class Player;
class Alien;
class Keyboard;
class Earth;
class Background;
class Shot;
class Explosion;


extern class Player         player;
extern class Alien          *aliens[ ALIEN_AMOUNT_Y ][ ALIEN_AMOUNT_X ];
extern class Keyboard       keyboard;
extern class Earth          earth;
extern class Universe       universe;
extern class Shot           shot;

extern std :: vector < GLfloat  > alien_vertex_vec;
extern std :: vector < material > alien_material_vec;
extern std :: vector < GLfloat  > player_vertex_vec;
extern std :: vector < material > player_material_vec;
extern std :: vector < GLfloat  > sphere_vertex_vec;
extern std :: vector < material > sphere_material_vec;
extern std :: vector < GLfloat  > city_vertex_vec;
extern std :: vector < material > city_material_vec;

extern GLuint univerese_texture;
extern GLuint earth_texture;
extern GLuint moon_texture;

extern std :: vector < Shot > shot_vector;
extern std :: vector < Shot > :: iterator shot_iterator;

extern std :: vector < Explosion > explosion_vec;
extern std :: vector < Explosion > :: iterator expl_iterator;

extern GLUquadric *quad;
extern int parzyste;
extern int number_alien_alive[ ALIEN_AMOUNT_Y ];
extern int status_gry;
extern bool created;


void createAliens();
void initAliens();
void drawAliens();
void deleteAliens();
void shotAliens();
void checkAliveAliens();
void checkAlienPosition();

void moveShots();
void checkCollision();

void drawExplosions();


enum{
    KEY_LEFT,
    KEY_RIGHT,
    KEY_PLUS,
    KEY_MINUS
};

enum{
    LEWO,
    PRAWO,
    DOL,
    GORA
};

enum{
    PLAYER_DEAD,
    PLAYER_WIN,
    NEW_GAME,
    AWAITING,
    RUN
};

typedef struct{
    float position_x;
    float position_y;
    float position_z;
    float a;
    float r;
    float angle;
}Fragment;

extern Fragment explode[ EXPLODE_FRAG ];

class Player{
    float   position_y;
    float   angle;
    float   width;

    public:

    float   position_x;
    float   position_z;
    bool    alive;
    int     life;

                Player();
        void    init();
        void    draw();
        bool    checkCollision( float x, float z, int kierunek );
};

class Alien{
    public:
    bool            alive;
    float           position_x;
    float           position_z;
    float           width;
    int             state;

                Alien( float pos_x, float pos_z );
           void move( int kierunek );
           bool checkCollision( float x, float z, int kierunek );
};

class Earth{
    float   position_x;
    float   position_y;
    float   position_z;
    float   rotate_angle;

    public:
                Earth();
        void    draw();

};

class Shot{
    public:

    bool    alive;
    float   position_x;
    float   position_z;
    int     kierunek;
    float   delta_x;

             Shot( float pos_x, float pos_z, int kier, float d_x );
        void draw();
        void move();
};

class Universe{
    float   rotate_angle;

    public:
                Universe();
        void    draw();
};

class Keyboard{
    public:
        bool      keypressed[ 4 ];

        void      keyAction( unsigned char key, int x, int y );
        void      keyActionUp( unsigned char key, int x, int y );
        void      specialKeysAction( int key, int x, int y, Player &player );
        void      specialKeysActionUp( int key, int x, int y );
};


class Explosion{
    public:

    Fragment explode[ EXPLODE_FRAG ];
    bool alive;
    int color;
    float scale;

        Explosion( float x, float y, float z, int col );
        void update();
        void draw();
};

#endif // MODELS_H_INCLUDED
