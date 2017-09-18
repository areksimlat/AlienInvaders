#include "obj_loader.h"

std :: vector < ILuint > all_textures_image;
std :: vector < GLuint > all_textures;

std :: vector < wspolrzedne_wierzcholka > v;
std :: vector < wspolrzedne_tekstury    > vt;
std :: vector < wspolrzedne_normalnych  > vn;


void load_vertex( const char *linia ){
    switch( linia[1] ){
        case ' ':
        wspolrzedne_wierzcholka wsp_w;
        sscanf( linia+1, "%f %f %f", &wsp_w.x, &wsp_w.y, &wsp_w.z );
        v.push_back( wsp_w );
        break;

        case 't':
        wspolrzedne_tekstury wsp_t;
        sscanf( linia+2, "%f %f", &wsp_t.u, &wsp_t.v );
        vt.push_back( wsp_t );
        break;

        case 'n':
        wspolrzedne_normalnych wsp_n;
        sscanf( linia+2, "%f %f %f", &wsp_n.i, &wsp_n.j, &wsp_n.k );
        vn.push_back( wsp_n );
        break;
    }
}



void load_material( char *linia, std :: vector < material > &vec_mat ){
    char path[128];
    char read_line[512];
    material new_mat;
    FILE *mtl_file;

    new_mat.nazwa[0] = '\0';
    new_mat.map_Kd[0] = '\0';

    sscanf( linia+7, "%s", path );
    mtl_file = fopen( path, "r" );
    if( mtl_file == NULL ){
        printf("Nie mozna otworzyc pliku: %s \n", path );
        return;
    }

    while( !feof( mtl_file ) ){
        fgets( read_line, 512, mtl_file );

        switch( read_line[0] ){
            case 'n':
                if( strlen( new_mat.nazwa) > 0 ){
                    vec_mat.push_back( new_mat );
                }

                sscanf( read_line+7, "%s", new_mat.nazwa );
            break;

            case 'K':
                if( read_line[1] == 'a' ){
                    sscanf( read_line+3, "%f %f %f", &new_mat.Ka[0], &new_mat.Ka[1], &new_mat.Ka[2] );
                }
                else if( read_line[1] == 'd' ){
                    sscanf( read_line+3, "%f %f %f", &new_mat.Kd[0], &new_mat.Kd[1], &new_mat.Kd[2] );
                }
                else if( read_line[1] == 's' ){
                    sscanf( read_line+3, "%f %f %f", &new_mat.Ks[0], &new_mat.Ks[1], &new_mat.Ks[2] );
                }
            break;

            case 'd':
                sscanf( read_line+2, "%f", &new_mat.d );
            break;

            case 'i':
                sscanf( read_line+6, "%d", &new_mat.illum );
            break;

            case 'N':
                sscanf( read_line+3, "%f", &new_mat.Ns );
            break;

            case 'm':
                sscanf( read_line+7, "%s", new_mat.map_Kd );
            break;
        }
    }

    if( strlen( new_mat.nazwa) > 0  )
        vec_mat.push_back( new_mat );

    fclose( mtl_file );
}



GLuint load_texture( const char *read_line ){
    ILuint img_texture;
    GLuint texture;
    ILboolean load;
    ILenum error;

    ilGenImages( 1, &img_texture );
    ilBindImage( img_texture );
    load = ilLoadImage( read_line );
    if( load ){
        printf( "Wczytano teksture: %s \n", read_line );
        all_textures_image.push_back( img_texture );
    }
    else{
        error = ilGetError();
        printf( "Nie wczytano tekstury: %s \t %s \n", read_line, iluErrorString( error ) );
        ilDeleteImage( img_texture );
    }


    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexImage2D( GL_TEXTURE_2D,
                  0,
                  GL_RGBA,
                  ilGetInteger( IL_IMAGE_WIDTH ),
                  ilGetInteger( IL_IMAGE_HEIGHT ),
                  0,
                  ilGetInteger( IL_IMAGE_FORMAT ),
                  ilGetInteger( IL_IMAGE_TYPE ),
                  ilGetData()
                 );

    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    all_textures.push_back( texture );

return texture;
}



void load_face_to_vector( char *linia, std :: vector < GLfloat > &vec ){
    std :: vector < unsigned int> poz_spacja;
    int iv, ivt, ivn;
    int rodzaj;
    int ilosc = 0;

    for( unsigned int i = 1; i < strlen( linia ); i++ ){
        if( linia[i] == ' ' ){
            poz_spacja.push_back( i );
        }
        if( linia[i] == '/' ){
            ilosc++;
        }
    }

    rodzaj = ( ilosc / poz_spacja.size() );

    if( rodzaj == 0 ){
        for( unsigned int i = 0; i < poz_spacja.size(); i++ ){
            sscanf( linia+poz_spacja[i], "%d", &iv );
            vec.push_back( v[iv - 1].x );
            vec.push_back( v[iv - 1].y );
            vec.push_back( v[iv - 1].z );
        }
    }
    else if( rodzaj == 1 ){

        for( unsigned int i = 0; i < poz_spacja.size(); i++ ){
            sscanf( linia+poz_spacja[i], "%d/%d", &iv, &ivt );
            vec.push_back( vt[ivt - 1].u );
            vec.push_back( vt[ivt - 1].v );
            vec.push_back( v[iv - 1].x );
            vec.push_back( v[iv - 1].y );
            vec.push_back( v[iv - 1].z );
        }
    }
    else if( rodzaj == 2 ){

        for( unsigned int i = 0; i < poz_spacja.size(); i++ ){
            sscanf( linia+poz_spacja[i], "%d/%d/%d", &iv, &ivt, &ivn );
            vec.push_back( vt[ivt - 1].u );
            vec.push_back( vt[ivt - 1].v );
            vec.push_back( vn[ivn - 1].i );
            vec.push_back( vn[ivn - 1].j );
            vec.push_back( vn[ivn - 1].k );
            vec.push_back( v[iv - 1].x );
            vec.push_back( v[iv - 1].y );
            vec.push_back( v[iv - 1].z );
        }
    }

    poz_spacja.clear();
}



unsigned int use_material( char *read_line, std :: vector < GLfloat > &vec, std :: vector < material > &vec_mat  ){
    char mat_nazwa[128];
    unsigned int i;

    sscanf( read_line+7, "%s", mat_nazwa );

    for( i = 0; i < vec_mat.size(); i++ ){
        if( strcmp( mat_nazwa, vec_mat[i].nazwa ) == 0 ){
           vec_mat[i].tex_begin = vec.size();
           break;
        }
    }

    return i;
}



void load_obj_to_vectors( const char *path, std :: vector < GLfloat > &vec, std :: vector < material > &vec_mat ){

    FILE *obj_file = fopen( path, "r" );
    char read_line[512];
    unsigned int last_use_material;

    if( obj_file == FALSE ){
        printf("\n Nie mozna otworzyc pliku: %s \n", path );
        return;
    }
    else
        printf("\n Plik %s zostal otwarty \n", path );


    while( !feof( obj_file ) ){
        fgets( read_line, 512, obj_file );

        switch( read_line[0] ){
            case 'm':
                load_material( read_line, vec_mat );
            break;

            case 'v':
                load_vertex( read_line );
            break;

            case 'f':
                load_face_to_vector( read_line, vec );
                vec_mat[ last_use_material ].tex_end = vec.size();
            break;

            case 'u':
                last_use_material = use_material( read_line, vec, vec_mat );
            break;

        }
    }

    for( unsigned int i = 0; i < vec_mat.size(); i++ ){
        if( strlen( vec_mat[i].map_Kd  ) > 0 )
            vec_mat[i].texture_id = load_texture( vec_mat[i].map_Kd );
    }

    v.clear();
    vt.clear();
    vn.clear();
}



void draw_vectors_array( std :: vector<GLfloat> &vec, std :: vector < material > &vec_mat, int manual_texture ){

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

    glInterleavedArrays( GL_T2F_N3F_V3F, 0, &vec[0] );

    if( vec_mat[0].illum == 2 )
                glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, vec_mat[0].Ks );

        glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, vec_mat[0].Ns );

        if( vec_mat[0].d < 1.0 ){
                glEnable( GL_BLEND );
                glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            }

        if( ( strlen( vec_mat[0].map_Kd ) > 0 ) && ( manual_texture < 0 ) ){
            glBindTexture( GL_TEXTURE_2D, vec_mat[0].texture_id );
            glEnable( GL_TEXTURE_2D );
        }
        else{
            glBindTexture( GL_TEXTURE_2D, manual_texture );
            glEnable( GL_TEXTURE_2D );
        }
    glDrawArrays( GL_TRIANGLES, 0, vec.size() / 8 );
}



void initLoader(){
    ilInit();
    iluInit();
    ilutRenderer( ILUT_OPENGL );
}



void clearLoader(){
    ilDeleteImages( all_textures_image.size(), &all_textures_image[0] );
    glDeleteTextures( all_textures.size(), &all_textures[0] );
    all_textures_image.clear();
    all_textures.clear();
}

