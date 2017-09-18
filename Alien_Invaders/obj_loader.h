#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <windows.h>
#include <GL/glut.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>



typedef struct {
    GLfloat x, y, z;
}wspolrzedne_wierzcholka;

typedef struct {
    GLfloat u, v;
}wspolrzedne_tekstury;

typedef struct {
    GLfloat i, j, k;
}wspolrzedne_normalnych;

typedef struct {
    char nazwa[256];
    GLfloat Ka[3];
    GLfloat Kd[3];
    GLfloat Ks[3];
    GLfloat d;
    GLfloat Ns;
    int illum;
    char map_Kd[256];
    unsigned int tex_begin;
    unsigned int tex_end;
    GLuint texture_id;
}material;


void initLoader();
void clearLoader();
void load_obj_to_vectors( const char *path, std :: vector < GLfloat > &vec, std :: vector < material > &vec_mat );
GLuint load_texture( const char *read_line );
void draw_vectors_array( std :: vector < GLfloat > &vec, std :: vector < material > &vec_mat, int manual_texture );

#endif // OBJ_LOADER_H_INCLUDED
