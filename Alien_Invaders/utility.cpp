#include "utility.h"

/****************************************************************************************************************/
/**                                             KAMERA                                                         **/
/****************************************************************************************************************/

Camera :: Camera(){
    angle_x  = 90.0;
    angle_y  = 50.0;
    distance = 10.0;
    convert_to_radian = M_PI / 180.0;
}



void Camera :: rotate(){
    if( keyboard.keypressed[ KEY_PLUS ] && distance > 7.0 ){
        distance -= 0.5;
    }
    else if( keyboard.keypressed[ KEY_MINUS ] ){
        distance += 0.5;
    }

    eye_x = distance * cos( angle_x * convert_to_radian );
    eye_y = distance * cos( angle_y * convert_to_radian );
    eye_z = distance * sin( angle_x * convert_to_radian );
}



void Camera :: set(){
    gluLookAt( eye_x, eye_y, eye_z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );
}



void Camera :: set_angle( Mouse *mouse ){
    angle_x = angle_x + mouse->angle_x;
    angle_y = angle_y + mouse->angle_y;
}

/****************************************************************************************************************/
/**                                             TIMER                                                          **/
/****************************************************************************************************************/

void Timer :: init(){
    QueryPerformanceFrequency( reinterpret_cast<LARGE_INTEGER*> ( &freq_time  )  );
    QueryPerformanceCounter(   reinterpret_cast<LARGE_INTEGER*> ( &start_time )  );
}



void Timer :: rest(){
    QueryPerformanceCounter( reinterpret_cast<LARGE_INTEGER*> ( &stop_time ) );

    ms_generate_frame_time = ( ( static_cast<long double> (stop_time.QuadPart) - start_time.QuadPart) / freq_time.QuadPart ) * 1000.0d;
    ms_sleep_time = FRAME_PER_SECOND - ms_generate_frame_time;

    if( ms_sleep_time > 0 )
        Sleep( ms_sleep_time );
}



/****************************************************************************************************************/
/**                                             FPS_COUNTER                                                    **/
/****************************************************************************************************************/

FPS_Counter :: FPS_Counter(){
    frame_count   = 0;
    current_time  = 0;
    previous_time = 0;
    fps = 0;
}



void FPS_Counter :: calculate(){

    frame_count++;
    current_time = glutGet( GLUT_ELAPSED_TIME );

    difference_time = current_time - previous_time;

    if( difference_time > 1000 ){  /** 1000 ms = 1 sek **/
        fps = frame_count / ( difference_time / 1000.0f );
        previous_time = current_time;
        frame_count = 0;
        //sprintf( fps_text, "FPS: %.2f", fps );
    }
}



void FPS_Counter :: print(){
    printf( "FPS: %.2f on %.2f \n", fps, FRAME_PER_SECOND );
    //sprintf( fps_text, "FPS: %.2f", fps );
    //text.draw( 10, screen_height - 20, fps_text );
   // printf( "FPS: %s \n", fps_text );

}



/****************************************************************************************************************/
/**                                             MYSZ                                                           **/
/****************************************************************************************************************/

Mouse :: Mouse(){
    sensitive = 0.2;
}



void Mouse :: buttonAction( int button, int state, int x, int y ){
    if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ){
        previous_x = x;
        previous_y = y;
    }
}



void Mouse :: moveAction( int x, int y ){
    angle_x = ( x - previous_x ) * sensitive;
    angle_y = ( previous_y - y ) * sensitive;

    previous_x = x;
    previous_y = y;

    camera.set_angle( this );
}



/****************************************************************************************************************/
/**                                             VSYNC                                                          **/
/****************************************************************************************************************/

void setVSYNC(){
		typedef BOOL (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(int interval);
		PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		wglSwapIntervalEXT = ( BOOL (WINAPI *)(int)) wglGetProcAddress( "wglSwapIntervalEXT" );
		int game_vsync_active = 0;

		if (wglSwapIntervalEXT != NULL ){
			game_vsync_active = 1;

			if ( game_vsync_active )
				wglSwapIntervalEXT( 1 );
			else
				wglSwapIntervalEXT( 0 );
		}
		else{
			printf("Przepraszam, SwapIntervalExt() nie jest dostepny. \n");
			return;
		}

		if ( game_vsync_active )
			printf("Synchronizacja pionowa : ON  \n");
		else
			printf("Synchronizacja pionowa : OFF \n");
}

/****************************************************************************************************************/
/**                                             TEXT                                                           **/
/****************************************************************************************************************/

Text :: Text(){
    font = (int)GLUT_BITMAP_TIMES_ROMAN_24;
}

void Text :: draw( float x, float y, const char *text ){
    const char *letter;

    glRasterPos2f( x, y );

    glColor3f( 1.0, 0.0, 0.0 );

    for( letter = text; *letter != '\0'; letter++ ){
        glutBitmapCharacter( (void*)font, *letter );
    }
}

void enable2D(){
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();

    glLoadIdentity();
    gluOrtho2D( 0, screen_width, 0, screen_height );

    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
    glLoadIdentity();

    glDisable( GL_LIGHTING );
}

void disable2D(){
    glPopMatrix();
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}


void drawQuads( float x, float y, float z, bool fill ){
    glDisable( GL_TEXTURE_2D );

    if( fill )
        glPolygonMode( GL_FRONT, GL_FILL );
    else
        glPolygonMode( GL_FRONT, GL_LINE );

    glBegin( GL_QUADS );
        glVertex3f( x + x, y + y, z );
        glVertex3f( x, y + y, z );
        glVertex3f( x, y, z );
        glVertex3f( x + x, y, z );
    glEnd();

    glEnable( GL_TEXTURE_2D );
    if( !fill )
        glPolygonMode( GL_FRONT, GL_FILL );
}

void drawLifeBar( float life ){

    glPushMatrix();
    glTranslatef( screen_width - 420, screen_height - 50, 0.0 );
    glColor3f( 1.0, 0.0, 0.0 );
    drawQuads( 200.0, 20.0, 0.0, FALSE );

    glPopMatrix();
    if( life > 0 ){
        glTranslatef( screen_width - ( life * 4 ) - 20, screen_height - 50, 0.0 );
        glColor3f( 0.5, 0.0, 0.0 );
        drawQuads( life * 2, 20.0, 0.0, TRUE );
    }

}



void draw_img_screen( GLuint tex_id ){
    glEnable( GL_TEXTURE_2D );

    glBindTexture( GL_TEXTURE_2D, tex_id );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE );

    glBegin( GL_QUADS );
        glTexCoord2f( 1.0, 1.0 ); glVertex3f( screen_width, screen_height, 0.0 );
        glTexCoord2f( 0.0, 1.0 ); glVertex3f(          0.0, screen_height, 0.0 );
        glTexCoord2f( 0.0, 0.0 ); glVertex3f(          0.0,           0.0, 0.0 );
        glTexCoord2f( 1.0, 0.0 ); glVertex3f( screen_width,           0.0, 0.0 );
    glEnd();
}
