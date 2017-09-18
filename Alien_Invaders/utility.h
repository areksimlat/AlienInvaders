#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "models.h"

const double       FRAME_PER_SECOND    = 33.33;   //(1000/30) frame per second
const unsigned int SCREEN_HEIGHT       = 600;
const unsigned int SCREEN_WIDTH        = 800;



class Camera;
class Timer;
class FPS_Counter;
class Mouse;
class Text;


extern class Camera         camera;
extern class Timer          timer;
extern class FPS_Counter    fps_counter;
extern class Mouse          mouse;
extern class Text           text;
extern int screen_width;
extern int screen_height;

extern GLuint player_win_texture;
extern GLuint alien_win_texture;


void setVSYNC();
void drawQuads( float x, float y, float z, bool fill );
void drawLifeBar( float life );
void draw_img_screen( GLuint tex_id );

void enable2D();
void disable2D();



class Camera{
    GLfloat       eye_x;
    GLfloat       eye_y;
    GLfloat       eye_z;
    GLfloat       angle_x;
    GLfloat       angle_y;
    GLfloat       distance;
    GLfloat       convert_to_radian;

    public:
                  Camera();
        void      set_angle( Mouse *mouse );
        void      rotate();
        void      set();
};

class Timer{
    LARGE_INTEGER start_time;
    LARGE_INTEGER stop_time;
    LARGE_INTEGER freq_time;
    long double   ms_generate_frame_time;
    long double   ms_sleep_time;

    public:
        void      init();
        void      rest();
};

class FPS_Counter{
    int           frame_count;
    int           current_time;
    int           previous_time;
    int           difference_time;
    char          fps_text[6];
    float         fps;

    public:
                  FPS_Counter();
        void      calculate();
        void      print();
};

class Mouse{
    public:

    int           previous_x;
    int           previous_y;
    float         angle_x;
    float         angle_y;
    float         sensitive;

                  Mouse();
        void      calculate();
        void      buttonAction( int button, int state, int x, int y );
        void      moveAction( int x, int y );
};

class Text{
    int font;

    void drawBackground();
    public:
             Text();
        void draw( float x, float y, const char *text );
};


#endif // UTILITY_H_INCLUDED
