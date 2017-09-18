#include "obj_loader.h"
#include "utility.h"
#include "render.h"
#include "models.h"

using namespace std;

void timeShotAlien( int value );

int screen_width;
int screen_height;
unsigned int count_game = 0;
int status_gry = RUN;
bool created = FALSE;
bool first_shot = FALSE;

void initNewGame();

Camera             camera;
Timer              timer;
FPS_Counter        fps_counter;
Mouse              mouse;
Keyboard           keyboard;
Render             render;
Player             player;
Alien              *aliens[ ALIEN_AMOUNT_Y ][ ALIEN_AMOUNT_X ];
Earth              earth;
Universe           universe;
Text               text;

GLUquadric *quad = gluNewQuadric();

std :: vector < GLfloat  > alien_vertex_vec;
std :: vector < material > alien_material_vec;

std :: vector < GLfloat  > player_vertex_vec;
std :: vector < material > player_material_vec;

std :: vector < GLfloat  > city_vertex_vec;
std :: vector < material > city_material_vec;

std :: vector < GLfloat  > sphere_vertex_vec;
std :: vector < material > sphere_material_vec;

std :: vector < Shot > shot_vector;
std :: vector < Shot > :: iterator shot_iterator;

std :: vector < Explosion > explosion_vec;
std :: vector < Explosion > :: iterator expl_iterator;

GLuint univerese_texture;
GLuint earth_texture;
GLuint moon_texture;

GLuint player_win_texture;
GLuint alien_win_texture;
GLuint run_texture;


void display(){
    if( !created ){
        initNewGame();
        created = TRUE;
    }

    if( status_gry == NEW_GAME ){
            if( !first_shot ){
                glutTimerFunc( 1500, timeShotAlien, 0 );
                first_shot = TRUE;
            }

        timer.init();
        render.init();

        camera.rotate();
        camera.set();

        render.draw();

        enable2D();

        drawLifeBar( player.life );
        fps_counter.calculate();
        fps_counter.print();

        glutSwapBuffers();
        glutPostRedisplay();

        disable2D();
        timer.rest();
    }
    else if( status_gry != AWAITING ){

        enable2D();
        glClear( GL_COLOR_BUFFER_BIT );

        if( status_gry == RUN ){
            draw_img_screen( run_texture );
        }
        else if( status_gry == PLAYER_WIN ){
            draw_img_screen( player_win_texture );
        }
        else{
            draw_img_screen( alien_win_texture );
        }

        glutSwapBuffers();
        glutPostRedisplay();
        disable2D();

        status_gry = AWAITING;
    }
}

void reshape(int width, int height){
    screen_width = width;
    screen_height = height;

    render.reshape( width, height );
    display();
}

void mouseButton( int button, int state, int x, int y ){
    mouse.buttonAction( button, state, x, y );
}

void mouseMove( int x, int y ){
    mouse.moveAction( x, y );
}

void keyFunc( unsigned char key, int x, int y ){
    keyboard.keyAction( key, x, y );
}

void keyFuncUp( unsigned char key, int x, int y ){
    keyboard.keyActionUp( key, x, y );
}

void specialKeys(int key, int x, int y){
    keyboard.specialKeysAction( key, x, y, player );
    display();
}

void specialKeysUp(int key, int x, int y){
    keyboard.specialKeysActionUp( key, x, y );
}

void timeShotAlien( int value ){
    parzyste++;
    if( parzyste == 2 ){
        parzyste = 0;
    }

    shotAliens();

    glutTimerFunc( 1500, timeShotAlien, 0 );
}

void cleanVectors(){
    alien_vertex_vec.clear();
    alien_material_vec.clear();

    player_vertex_vec.clear();
    player_material_vec.clear();

    shot_vector.clear();
    explosion_vec.clear();

    sphere_vertex_vec.clear();
    sphere_material_vec.clear();
}

void initNewGame(){
    if( count_game == 0 ){
        createAliens();
    }
    else{
        initAliens();
    }

    shot_vector.clear();
    explosion_vec.clear();

    player.init();

    count_game++;
}

int main(int argc, char *argv[]){
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize ( SCREEN_WIDTH, SCREEN_HEIGHT         );
    glutCreateWindow   ( "ALIEN INVADERS 0.9"                );
    initLoader();

    glutDisplayFunc   ( display       );
    glutReshapeFunc   ( reshape       );
    glutMouseFunc     ( mouseButton   );
    glutMotionFunc    ( mouseMove     );
    glutKeyboardFunc  ( keyFunc       );
    glutKeyboardUpFunc( keyFuncUp     );
    glutSpecialFunc   ( specialKeys   );
    glutSpecialUpFunc ( specialKeysUp );

    player_vertex_vec.reserve( 205896 );
    player_material_vec.reserve( 7 );
    alien_vertex_vec.reserve( 7176 );
    alien_material_vec.reserve( 3 );
    sphere_vertex_vec.reserve( 950424 );
    sphere_material_vec.reserve( 1 );


    load_obj_to_vectors( PLAYER_PATH, player_vertex_vec, player_material_vec );
    load_obj_to_vectors( ALIEN_PATH, alien_vertex_vec, alien_material_vec );
    load_obj_to_vectors( CITY_PATH, city_vertex_vec, city_material_vec );
    load_obj_to_vectors( SPHERE_PATH, sphere_vertex_vec, sphere_material_vec );

    univerese_texture = load_texture( UNIVERSE_PATH );
    earth_texture = load_texture( EARTH_PATH );
    moon_texture = load_texture( MOON_PATH );

    player_win_texture = load_texture( PLAYER_WIN_TEX );
    alien_win_texture = load_texture( ALIEN_WIN_TEX );
    run_texture = load_texture( RUN_TEX );

    srand( time( NULL ) );

    atexit( clearLoader  );
    atexit( deleteAliens );
    atexit( cleanVectors );

    //FreeConsole();

    glutMainLoop();

    return 0;
}
