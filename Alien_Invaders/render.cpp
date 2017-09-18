#include "render.h"

void Render :: init(){
    GLfloat light0_param[4] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light0_position[4] = { 0.0, 100.0, 0.0, 0.0 };
    GLfloat light1_param[4] = { 0.9, 0.9, 0.9, 1.0 };
    GLfloat light1_position[4] = { 0.0, -20.0, 0.0, 0.0 };

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // glPolygonMode( GL_FRONT, GL_LINE );

    glShadeModel( GL_SMOOTH );
    glHint     ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    glEnable   ( GL_CULL_FACE      );
    glEnable   ( GL_POLYGON_SMOOTH );
    glEnable   ( GL_DEPTH_TEST     );
    glEnable   ( GL_TEXTURE_2D     );
    glEnable   ( GL_NORMALIZE      );
    glEnable   ( GL_COLOR_MATERIAL );
    glEnable   ( GL_LIGHTING       );
    glEnable   ( GL_LIGHT0         );
    glEnable   ( GL_LIGHT1         );


    glFrontFace( GL_CCW );
    glLightfv( GL_LIGHT0, GL_POSITION, light0_position );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, light0_param );
    glLightfv( GL_LIGHT0, GL_SPECULAR, light0_param );
    glLightfv( GL_LIGHT1, GL_POSITION, light1_position );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, light1_param );
    glLightfv( GL_LIGHT1, GL_SPECULAR, light1_param );

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void Render :: reshape( int width, int height ){
    glViewport( 0, 0, (GLsizei)width, (GLsizei)height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    gluPerspective( 45.0f, (GLfloat)width/(GLfloat)height, 1.0, 6000.0 );
}


void Render :: draw(){

    glPushMatrix();
    universe.draw();

    glPopMatrix();
    glPushMatrix();
    earth.draw();

    glPopMatrix();
    glPushMatrix();
    player.draw();

    checkCollision();
    moveShots();
    drawAliens();
    checkAliveAliens();
    checkAlienPosition();

    drawExplosions();
    glPopMatrix();
    glPushMatrix();

    glPopMatrix();
    glPushMatrix();
    glTranslatef( 18.0, -50.0, -10.0 );
    glScalef( 0.2, 0.2, 0.2 );
    draw_vectors_array( city_vertex_vec, city_material_vec, -1 );

    glPopMatrix();
    glScalef( 40.0, 40.0, 40.0 );
    glTranslatef( 0.0, -7.8, -2.0 );
    draw_vectors_array( sphere_vertex_vec, sphere_material_vec, moon_texture );
}
