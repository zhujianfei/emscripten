/*
THIS WORK, INCLUDING THE SOURCE CODE, DOCUMENTATION
AND RELATED MEDIA AND DATA, IS PLACED INTO THE PUBLIC DOMAIN.

THE ORIGINAL AUTHOR IS KYLE FOLEY.

THIS SOFTWARE IS PROVIDED AS-IS WITHOUT WARRANTY
OF ANY KIND, NOT EVEN THE IMPLIED WARRANTY OF
MERCHANTABILITY. THE AUTHOR OF THIS SOFTWARE,
ASSUMES _NO_ RESPONSIBILITY FOR ANY CONSEQUENCE
RESULTING FROM THE USE, MODIFICATION, OR
REDISTRIBUTION OF THIS SOFTWARE.
*/

#include "GL/glew.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    SDL_Surface *screen;
    if ( SDL_Init(SDL_INIT_VIDEO) != 0 ) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    screen = SDL_SetVideoMode( 1024, 768, 24, SDL_OPENGL );
    if ( !screen ) {
        printf("Unable to set video mode: %s\n", SDL_GetError());
        return 1;
    }
    
    glClearColor( 0, 0, 0, 0 );
    glClear( GL_COLOR_BUFFER_BIT );

    // Load the OpenGL texture

    GLuint texture;
    SDL_Surface *surface;
    
    if ( (surface = IMG_Load("screenshot.png")) ) { 
        if ( (surface->w & (surface->w - 1)) != 0 ) {
            printf("warning: width is not a power of 2\n");
        }
        if ( (surface->h & (surface->h - 1)) != 0 ) {
            printf("warning: height is not a power of 2\n");
        }
    
        glGenTextures( 1, &texture );
        glBindTexture( GL_TEXTURE_2D, texture );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, 
                      GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels );
    } 
    else {
        printf("SDL could not load image.bmp: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }    
    
    if ( surface ) { 
        SDL_FreeSurface( surface );
    }

    // Create a second texture

    GLuint texture2;
    glGenTextures( 1, &texture2 );
    glBindTexture( GL_TEXTURE_2D, texture2 );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    GLubyte texture2Data[] = { 0xff,    0,    0, 0xff,
                                  0, 0xff,    0, 0xaa,
                                  0,    0, 0xff, 0x55,
                               0x80, 0x90, 0x70,    0 };
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0,
                  GL_RGBA, GL_UNSIGNED_BYTE, texture2Data );
    
    // BEGIN

    glewInit();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.6435469817188064, 0.6435469817188064 ,-0.48266022190470925, 0.48266022190470925 ,0.5400000214576721, 2048);
    glMatrixMode(GL_MODELVIEW);
    GLfloat matrixData[] = { -1, 0, 0, 0,
                              0, 0,-1, 0,
                              0, 1, 0, 0,
                              0, 0, 0, 1 };
    glLoadMatrixf(matrixData);
    glRotated(0, 0, 1, 0);
    glRotated(0,-1, 0, 0);
    glRotated(0, 0, 0,-1);
    glTranslated(-512,-512,-527);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glClear(GL_DEPTH_BUFFER_BIT);

    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glActiveTexture(GL_TEXTURE0);

    glEnableClientState(GL_VERTEX_ARRAY);

    GLuint arrayBuffer, elementBuffer;
    glGenBuffers(1, &arrayBuffer);
    glGenBuffers(1, &elementBuffer);

    GLubyte arrayData[] = {
/*
[0, 0,   0, 67] ==>  128 float
[0, 0, 128, 67] ==>  256 float
[0, 0,   0, 68] ==>  512 float
[0, 0, 128, 68] ==> 1024 float

[vertex x        ] [vertex y         ] [vertex z         ] [nr]                [texture u        ] [texture v        ] [lm u   ] [lm v   ] [color r,g,b,a    ] */
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128, //  0
  0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128, //  1
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,  67,   0,   0,   0,   0, 128, 128, 128, 128, //  2
  0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,   0, 128, 128, 128, 128, //  3
  0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128, //  4
  0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0, 128,  67,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128, //  5
  0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0, 128,  67,   0,   0,   0,  67,   0,   0,   0,   0, 128, 128, 128, 128, //  6
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,  67,   0,   0,   0,   0, 128, 128, 128, 128, //  7
  0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,   0, 128, 128, 128, 128, //  8
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,  67,   0,   0,   0,   0, 128, 128, 128, 128, //  9
  0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0, 128,  67,   0,   0,   0,   0, 128, 128, 128, 128, // 10
  0,   0,   0,   0,   0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128,  67,   0,   0,   0,   0, 128, 128, 128, 128, // 11
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0,   0,  67,   0,   0,   0,   0, 128, 128, 128, 128, // 12
  0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0, 128,  67,   0,   0,   0,  67,   0,   0,   0,   0, 128, 128, 128, 128, // 13
  0,   0, 128,  68,   0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0, 128,  67,   0,   0, 128,  67,   0,   0,   0,   0, 128, 128, 128, 128, // 14
  0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  67,   0,   0, 128,  67,   0,   0,   0,   0, 128, 128, 128, 128, // 15

  0,   0,   0,  68,   0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,   0,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,   0,   0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,   0,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0, 128,  68,   0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0, 128,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,   0,   0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,   0,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,   0,   0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0, 128,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0,   0,  68,   0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0, 128,  68,   0,   0,   0,  68,   0,   0, 128,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128,
  0,   0, 128,  68,   0,   0, 128,  68,   0,   0,   0,  68,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 128, 128, 128, 128
    };
    assert(sizeof(arrayData) == 1408);
    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrayData), arrayData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLushort elementData[] = { 1, 2, 0, 2, 3, 0, 5, 6, 4, 6, 7, 4, 9, 10, 8, 10, 11, 8, 13, 14, 12, 14, 15, 12 };
    assert(sizeof(elementData) == 48);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementData), elementData, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    // sauer vertex data is apparently 0-12: V3F, 12: N1B, 16-24: T2F, 24-28: T2S, 28-32: C4B
    glVertexPointer(3, GL_FLOAT, 32, (void*)0); // all these apply to the ARRAY_BUFFER that is bound
    glTexCoordPointer(2, GL_FLOAT, 32, (void*)16);
    glClientActiveTexture(GL_TEXTURE1);
    glTexCoordPointer(2, GL_SHORT, 32, (void*)24);
    glClientActiveTexture(GL_TEXTURE0); // likely not needed, it is a cleanup
    glNormalPointer(GL_BYTE, 32, (void*)12);
    glColorPointer(4, GL_UNSIGNED_BYTE, 32, (void*)28);

    glBindTexture(GL_TEXTURE_2D, texture); // diffuse?
    glActiveTexture(GL_TEXTURE0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2); // lightmap?
    glActiveTexture(GL_TEXTURE0);

    GLint ok;

    const char *vertexShader = "uniform vec4 texgenscroll;\n"
                               "void main(void)\n"
                               "{\n"
                               "    gl_Position = ftransform();\n"
                               "    gl_TexCoord[0].xy = gl_MultiTexCoord0.xy + texgenscroll.xy;\n"
                               "    gl_TexCoord[1].xy = gl_MultiTexCoord1.xy * 3.051851e-05;\n"
                               "}\n";
    const char *fragmentShader = "uniform vec4 colorparams;\n"
                                 "uniform sampler2D diffusemap, lightmap;\n"
                                 "void main(void)\n"
                                 "{\n"
                                 "    vec4 diffuse = texture2D(diffusemap, gl_TexCoord[0].xy);\n"
                                 "    vec4 lm = texture2D(lightmap, gl_TexCoord[1].xy);\n"
                                 "    diffuse *= colorparams;\n"
                                 "    gl_FragColor = diffuse * lm;\n"
                                 "}\n";

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertexShader, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &ok);
    assert(ok);

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragmentShader, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &ok);
    assert(ok);

    GLuint program = glCreateProgram();

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    assert(ok);

    glUseProgram(program);

    GLint lightmapLocation = glGetUniformLocation(program, "lightmap");
    assert(lightmapLocation >= 0);
    glUniform1i(lightmapLocation, 1); // sampler2D? Is it the texture unit?

    GLint diffusemapLocation = glGetUniformLocation(program, "diffusemap");
    assert(diffusemapLocation >= 0);
    glUniform1i(diffusemapLocation, 0);

    GLint texgenscrollLocation = glGetUniformLocation(program, "texgenscroll");
    assert(texgenscrollLocation >= 0);

    GLint colorparamsLocation = glGetUniformLocation(program, "colorparams");
    assert(colorparamsLocation >= 0);

    GLfloat texgenscrollData[] = { 0, 0, 0, 0 };
    glUniform4fv(texgenscrollLocation, 1, texgenscrollData);

    GLfloat colorparamsData[] = { 2, 2, 2, 1 };
    glUniform4fv(colorparamsLocation, 1, colorparamsData);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)12);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*) 0);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)24);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, (void*)36);

    // END

    SDL_GL_SwapBuffers();
    
#if !EMSCRIPTEN
    SDL_Delay(3000);
#endif

    SDL_Quit();
    
    return 0;
}
