#include "models.h"

/****************************************************************************************************************/
/**                                             PLAYER                                                         **/
/****************************************************************************************************************/

Player :: Player(){
    position_x = 0.0;
    position_y = 0.0;
    position_z = 3.0;
    angle = 0.0;
    alive = TRUE;
    life = 100;
    width = 0.01;
}

void Player :: init(){
    position_x = 0.0;
    position_y = 0.0;
    position_z = 3.0;
    angle = 0.0;
    alive = TRUE;
    life = 100;
}

void Player :: draw(){
    if( keyboard.keypressed[ KEY_LEFT ] ){
        if( position_x > -6.0 )
            position_x -= 0.33;

        if( angle < 60.0 )
            angle += 20.0;
    }
    else if( keyboard.keypressed[ KEY_RIGHT ] ){
        if( position_x < 6.0 )
            position_x += 0.33;

        if( angle > -60.0 )
            angle -= 20.0;
    }
    else{
        if( angle < 0.0 )
            angle += 10.0;
        else if( angle > 0.0 )
            angle -= 10.0;
    }

    glTranslatef( position_x, position_y, position_z );
    glRotatef( angle, 0.0, 0.0, 1.0 );

    if( alive )
        draw_vectors_array( player_vertex_vec, player_material_vec, -1 );
}

bool Player :: checkCollision( float x, float z, int kierunek ){
    if( ( x >= position_x-width ) && ( x <= position_x+width ) && ( z <= position_z ) && ( kierunek == DOL ) ){
        return TRUE;
    }
    else{
        return FALSE;
    }
}



/****************************************************************************************************************/
/**                                             ALIEN                                                          **/
/****************************************************************************************************************/

int kierunek = LEWO;
bool move_down = FALSE;
int parzyste = 0;
int number_alien_alive[ ALIEN_AMOUNT_Y ];

Alien :: Alien( float pos_x, float pos_z ){
    alive = TRUE;
    position_x = pos_x;
    position_z = pos_z;
    width = 0.3;
    state = 0;
}

void Alien :: move( int kierunek ){
    switch( kierunek ){
        case LEWO:
            position_x -= 0.1;
        break;

        case PRAWO:
            position_x += 0.1;
        break;
    }

    if( move_down ){
        position_z += 0.1;
    }
}

bool Alien :: checkCollision( float x, float z, int kierunek ){
    if( ( x >= position_x-width ) && ( x <= position_x+width ) && ( z <= position_z ) && ( kierunek == GORA ) ){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

void createAliens(){
    for( unsigned int i = 0; i < ALIEN_AMOUNT_Y; i++ ){
        for( unsigned int j = 0; j < ALIEN_AMOUNT_X; j++ ){
            aliens[i][j] = new Alien( (float)j * 2 - 5.0, -( (float)i + 2.5 ) * 2.0 );
        }
        number_alien_alive[ i ] = ALIEN_AMOUNT_X;
    }
}

void initAliens(){
    for( unsigned int i = 0; i < ALIEN_AMOUNT_Y; i++ ){
        for( unsigned int j = 0; j < ALIEN_AMOUNT_X; j++ ){
            aliens[i][j]->position_x = (float)j * 2 - 5.0;
            aliens[i][j]->position_z = -((float)i + 2.5 ) * 2.0 ;
            aliens[i][j]->alive = TRUE;
            aliens[i][j]->state = 0;
        }
        number_alien_alive[ i ] = ALIEN_AMOUNT_X;
    }
}


void deleteAliens(){
    for( unsigned int i = 0; i < ALIEN_AMOUNT_Y; i++ ){
        for( unsigned int j = 0; j < ALIEN_AMOUNT_X; j++ ){
            delete aliens[i][j];
        }
    }
}

void drawAliens(){

    if( aliens[0][0]->alive == FALSE ){
        aliens[0][0]->move( kierunek );
    }
    if( aliens[0][ALIEN_AMOUNT_X]->alive == FALSE ){
        aliens[0][ALIEN_AMOUNT_X]->move( kierunek );
    }

    if( ( aliens[0][0]->position_x < -6.0 ) && ( kierunek == LEWO ) ){
        kierunek = PRAWO;
        move_down = TRUE;
    }
    else if( ( aliens[0][ALIEN_AMOUNT_X]->position_x > -4.0 ) && ( kierunek == PRAWO ) ){
        kierunek = LEWO;
        move_down = TRUE;
    }

    for( int i = 0; i < ALIEN_AMOUNT_Y; i++ ){
        for( int j = 0; j < ALIEN_AMOUNT_X; j++ ){
            if( aliens[i][j]->alive ){
                glPopMatrix();
                glPushMatrix();

                aliens[i][j]->move( kierunek );

                glTranslatef( aliens[i][j]->position_x, 0.0, aliens[i][j]->position_z );

                draw_vectors_array( alien_vertex_vec, alien_material_vec, alien_material_vec[ aliens[i][j]->state ].texture_id );
            }
        }
    }

    move_down = FALSE;
}

void shotAliens(){
    int shot_count = 0;
    bool done = FALSE;
    float dx, dz, m;

    for( int i = 0; i < ALIEN_AMOUNT_Y; i++ ){
        for( int j = 0; j < ALIEN_AMOUNT_X; j++ ){
            if( aliens[i][j]->alive ){
                if( ( j % 2 == parzyste ) && ( shot_count < 3 ) ){

                    dx = aliens[i][j]->position_x - player.position_x;
                    dz = aliens[i][j]->position_z - player.position_z;
                    m = dx / dz;

                    shot_vector.push_back( Shot(aliens[i][j]->position_x, aliens[i][j]->position_z, DOL, m ));

                    shot_count++;

                    if( shot_count == 3 ){
                        done = TRUE;
                        break;
                    }
                }
            }
        }
        if( done )
            break;
    }
}

void checkAlienPosition(){
    for( int i = 0; i < ALIEN_AMOUNT_Y; i++ ){
        for( int j = 0; j < ALIEN_AMOUNT_X; j++ ){
            if( aliens[i][j]->position_z >= 2.0 ){
                player.alive = FALSE;
                status_gry = PLAYER_DEAD;
                break;
            }
        }
    }
}


/****************************************************************************************************************/
/**                                             SHOT                                                           **/
/****************************************************************************************************************/

Shot :: Shot( float pos_x, float pos_z, int kier, float d_x ){
    alive = TRUE;
    position_x = pos_x;
    position_z = pos_z;
    kierunek = kier;
    delta_x = d_x;
}

void Shot :: draw(){
    glDisable( GL_TEXTURE_2D );

    glColor3f( 1.0, 1.0, 0.0 );
    glTranslatef( position_x, 0.0, position_z );
    gluSphere( quad, 0.05, 8, 8 );
}

void Shot :: move(){
    switch( kierunek ){
        case GORA:
            position_z -= 0.5;

            if( position_z < -10.0 )
                alive = FALSE;
        break;

        case DOL:
            position_x += delta_x;
            position_z += 0.5;
        break;
    }
}

void moveShots(){
    shot_iterator = shot_vector.begin();

    while( shot_iterator != shot_vector.end() ){
        glPopMatrix();
        glPushMatrix();

        if( shot_iterator->alive ){
            shot_iterator->move();
            shot_iterator->draw();
            shot_iterator++;
        }
        else{
            shot_iterator = shot_vector.erase( shot_iterator );
        }
    };
}

void checkCollision(){
    bool hit = FALSE;
    shot_iterator = shot_vector.begin();

    while( shot_iterator != shot_vector.end() ){
        if( shot_iterator->kierunek == GORA ){
            for( unsigned int i = 0; i < ALIEN_AMOUNT_Y; i++ ){
                for( unsigned int j = 0; j < ALIEN_AMOUNT_X; j++ ){

                    if( aliens[i][j]->alive ){

                        if( aliens[i][j]->checkCollision( shot_iterator->position_x, shot_iterator->position_z, shot_iterator->kierunek ) ){
                            aliens[i][j]->state++;
                            shot_iterator = shot_vector.erase( shot_iterator );

                            if( aliens[i][j]->state > 2 ){
                                number_alien_alive[i]--;

                                aliens[i][j]->alive = FALSE;
                                explosion_vec.push_back( Explosion( aliens[i][j]->position_x, 0.0, aliens[i][j]->position_z, 0 ) );
                            }

                            hit = TRUE;
                            break;
                        }
                    }
                }
                if( hit ){
                    hit = FALSE;
                    break;
                }
            }
        }
        else if( shot_iterator->kierunek == DOL ){
            if( player.checkCollision( shot_iterator->position_x, shot_iterator->position_z, DOL ) ){
                player.life -= 20;

                if( ( player.life <= 0 ) && ( player.alive ) ){
                    player.alive = FALSE;
                    explosion_vec.push_back( Explosion( player.position_x, 0.0, player.position_z, 1 ) );
                }

                shot_iterator = shot_vector.erase( shot_iterator );
            }
        }

    if( shot_iterator != shot_vector.end() ) shot_iterator++;
    }
}

void checkAliveAliens(){
    int alive_aliens = 0;

    for( int i = 0; i < ALIEN_AMOUNT_Y; i++ ){
        alive_aliens += number_alien_alive[i];
    }

    if( alive_aliens == 0 ){
        Sleep(4);
        status_gry = PLAYER_WIN;
    }
}

/****************************************************************************************************************/
/**                                             KLAWIATURA                                                     **/
/****************************************************************************************************************/

void Keyboard :: keyAction( unsigned char key, int x, int y ){
    if( key == ' ' ){
        shot_vector.push_back( Shot( player.position_x, player.position_z, GORA, 0 ) );
    }
    else if( key == 13 ){
        if ( status_gry == AWAITING ){
            created = FALSE;
            status_gry = NEW_GAME;
            glutPostRedisplay();
        }
    }
    else if( key == '+' ){
        keypressed[ KEY_PLUS ] = TRUE;
    }
    else if( key == '-' ){
        keypressed[ KEY_MINUS ] = TRUE;
    }
}

void Keyboard :: keyActionUp( unsigned char key, int x, int y ){
    if( key == '+' ){
        keypressed[ KEY_PLUS ] = FALSE;
    }
    else if( key == '-' ){
        keypressed[ KEY_MINUS ] = FALSE;
    }
}

void Keyboard :: specialKeysAction( int key, int x, int y, Player &player ){
    if( key == GLUT_KEY_LEFT ){
        keypressed[ KEY_LEFT ] = TRUE;
    }
    else if( key == GLUT_KEY_RIGHT ){
        keypressed[ KEY_RIGHT ] = TRUE;
    }
}

void Keyboard :: specialKeysActionUp( int key, int x, int y ){
    if( key == GLUT_KEY_LEFT ){
        keypressed[ KEY_LEFT ] = FALSE;
    }
    else if( key == GLUT_KEY_RIGHT ){
        keypressed[ KEY_RIGHT ] = FALSE;
    }
}


/****************************************************************************************************************/
/**                                             EARTH                                                          **/
/****************************************************************************************************************/

Earth :: Earth(){
    position_x =  -10.0;
    position_y =  -7.0;
    position_z =  -60.0;
}



void Earth :: draw(){
    rotate_angle += 2.0;
    if( rotate_angle > 360.0 )
        rotate_angle = 0.0;

    glTranslatef( position_x, position_y, position_z );
    glRotatef( 40.0, 1.0, 0.0, 0.0 );

    glRotatef( -rotate_angle, 0.0, 0.0, 1.0 );
    draw_vectors_array( sphere_vertex_vec, sphere_material_vec, earth_texture );
}



/****************************************************************************************************************/
/**                                             UNIVERSE                                                       **/
/****************************************************************************************************************/

Universe :: Universe(){
    rotate_angle = 0.0;
}



void Universe :: draw(){
    rotate_angle += 0.05;

    if( rotate_angle > 360.0 )
        rotate_angle = 0.0;

    glScalef( 500.0, 500.0, 500.0 );
    glRotatef( -rotate_angle, 1.0, 0.0, 0.0 );

    draw_vectors_array( sphere_vertex_vec, sphere_material_vec, univerese_texture );
}


/****************************************************************************************************************/
/**                                             EXPLODE                                                        **/
/****************************************************************************************************************/

GLfloat expl_mat_ambient[4]  = { 0.2, 0.2, 0.2, 1.0 };
GLfloat expl_mat_specular[4] = { 0.8, 0.0, 0.0, 1.0 };

Explosion :: Explosion( float x, float y, float z, int col ){
    float r;
    float step =  ( 2 * M_PI ) / EXPLODE_FRAG;
    float a = 0;

    for( int i = 0; i < EXPLODE_FRAG; i++ ){
        r = ( rand() % 1000 + 1 ) / 1000.0;
        explode[i].position_x = x + r * cos( a );
        explode[i].position_y = y + r * sin( a );
        explode[i].position_z = z - ( ( rand() % 2000 - 1000 ) / 1000.0 );
        explode[i].a = a;
        explode[i].r = r;
        explode[i].angle = rand() % 360;
        a += step;
    }

    alive = TRUE;
    color = col;
    scale = 1.0;
}

void Explosion :: update(){
    for( int i = 0; i < EXPLODE_FRAG; i++ ){
        explode[i].r += 0.01;
        explode[i].position_x += explode[i].r * cos( explode[i].a );
        explode[i].position_y += explode[i].r * sin( explode[i].a );
        explode[i].position_z += 0.1;
    }

    if( ( explode[0].position_z > 2.0 ) && ( color == 0 ) ){
        alive = FALSE;
    }
    else if( explode[0].position_z > 5.0 ){
        alive = FALSE;
        if( !player.alive ){
            status_gry = PLAYER_DEAD;
        }
    }

    scale += 0.1;
}

void Explosion :: draw(){
    glDisable( GL_TEXTURE_2D );

    if( color == 0 ){
        glColor3f( 0.45, 0.20, 0.20 );
    }
    else{
        glColor3f( 0.0, 0.0, 1.0 );
    }

    glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, expl_mat_ambient );
    glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, expl_mat_specular );

    for( int i = 0; i < EXPLODE_FRAG; i++ ){
        glPopMatrix();
        glPushMatrix();

        glTranslatef( explode[i].position_x, explode[i].position_y, explode[i].position_z );
        glRotatef( explode[i].angle, 1.0, 1.0, 1.0 );
        glScaled( scale, scale, scale );

        glBegin( GL_TRIANGLES );
            glTexCoord3f(  0.1, 0.0, 0.0 ); glVertex3f(  0.1, 0.0, 0.0 );
            glTexCoord3f(  0.0, 0.1, 0.0 ); glVertex3f(  0.0, 0.1, 0.0 );
            glTexCoord3f( -0.1, 0.0, 0.0 ); glVertex3f( -0.1, 0.0, 0.0 );
        glEnd();
    }
}

void drawExplosions(){
    expl_iterator = explosion_vec.begin();

    while( expl_iterator != explosion_vec.end() ){
        if( expl_iterator->alive ){
            expl_iterator->draw();
            expl_iterator->update();
        }
        else{
            expl_iterator = explosion_vec.erase( expl_iterator );
        }

    if( expl_iterator != explosion_vec.end() )
        expl_iterator++;
    }
}
