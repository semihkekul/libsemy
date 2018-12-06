
/* Copyright (c) Mark J. Kilgard, 1994. */

/**
 * (c) Copyright 1993, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */
/*
 *  scene.c
 *  This program demonstrates the use of the GL lighting model.
 *  Objects are drawn using a grey material characteristic. 
 *  A single light source illuminates the objects.
 */
#include <stdlib.h>
#include <GL/glut.h>
#include "tiny_obj_loader.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Vertex
{
    float vx;
    float vy;
    float vz;
};

struct Normal
{
    float nx;
    float ny;
    float nz;
};

struct Texture
{
    float tx;
    float ty;
};

struct Face
{
    Vertex V[3];
    Normal N[3];
    Texture T[3];
};

vector<Face> faces;


void loadObj()
{
    string inputfile  = "sphere.obj";
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
      
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
      
    if (!err.empty()) { // `err` may contain warning message.
      std::cerr << err << std::endl;
    }

    if (!ret) {
      exit(1);
    }

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
      // Loop over faces(polygon)
      size_t index_offset = 0;
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
        int fv = shapes[s].mesh.num_face_vertices[f];
        Face face;
        
        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          face.V[v].vx = attrib.vertices[3*idx.vertex_index+0];
          face.V[v].vy = attrib.vertices[3*idx.vertex_index+1];
          face.V[v].vz = attrib.vertices[3*idx.vertex_index+2];
          face.N[v].nx = attrib.normals[3*idx.normal_index+0];
          face.N[v].ny = attrib.normals[3*idx.normal_index+1];
          face.N[v].nz = attrib.normals[3*idx.normal_index+2];
          //face.T[v].tx = attrib.texcoords[2*idx.texcoord_index+0];
          //face.T[v].ty = attrib.texcoords[2*idx.texcoord_index+1];
        }
        index_offset += fv;

        // per-face material
        shapes[s].mesh.material_ids[f];

        faces.push_back(face);
      }
      
    }
}

void drawObject()
{
    for(unsigned int i = 0; i < faces.size(); i++)
    {
        glBegin(GL_TRIANGLES);
            glNormal3f(faces[i].N[0].nx, faces[i].N[0].ny, faces[i].N[0].nz);
            glVertex3f(faces[i].V[0].vx, faces[i].V[0].vy, faces[i].V[0].vz);

            glNormal3f(faces[i].N[1].nx, faces[i].N[1].ny, faces[i].N[1].nz);
            glVertex3f(faces[i].V[1].vx, faces[i].V[1].vy, faces[i].V[1].vz);

            glNormal3f(faces[i].N[2].nx, faces[i].N[2].ny, faces[i].N[2].nz);
            glVertex3f(faces[i].V[2].vx, faces[i].V[2].vy, faces[i].V[2].vz);
        glEnd();
    }
}
/*  Initialize material property and light source.
 */
void 
myinit(void)
{
    GLfloat light_ambient[] =
    {0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse[] =
    {1.0, 0.0, 0.0, 1.0};
    GLfloat light_specular[] =
    {1.0, 1.0, 1.0, 1.0};
/* light_position is NOT default value */
    GLfloat light_position[] =
    {1.0, 1.0, 1.0, 0.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHT0);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    loadObj();
}

void 
display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glPushMatrix();
    glScalef(1.3, 1.3, 1.3);
    glRotatef(20.0, 1.0, 0.0, 0.0);

    glPushMatrix();
    glTranslatef(-0.75, 0.5, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    glutSolidTorus(0.275, 0.85, 10, 15);
    glPopMatrix();

    /*glPushMatrix();
    glTranslatef(-0.75, -0.5, 0.0);
    glRotatef(270.0, 1.0, 0.0, 0.0);
    glutSolidTetrahedron();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.75, 0.0, -1.0);
    glutSolidIcosahedron();
    glPopMatrix();
 */
    drawObject();

    glPopMatrix();
    glFlush();
}

void 
myReshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-2.5, 2.5, -2.5 * (GLfloat) h / (GLfloat) w,
            2.5 * (GLfloat) h / (GLfloat) w, -10.0, 10.0);
    else
        glOrtho(-2.5 * (GLfloat) w / (GLfloat) h,
            2.5 * (GLfloat) w / (GLfloat) h, -2.5, 2.5, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

void 
polygon_mode(int value)
{
    switch (value) {
    case 1:
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case 2:
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_LIGHTING);
        glColor3f(1.0, 1.0, 1.0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    }
    glutPostRedisplay();
}

void 
main_menu(int value)
{
    if (value == 666)
        exit(0);
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int 
main(int argc, char **argv)
{
    int submenu;

    glutInit(&argc, argv);
    glutInitWindowPosition(500, 500);
    glutInitWindowSize(1200, 800);
    glutCreateWindow(argv[0]);
    myinit();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    submenu = glutCreateMenu(polygon_mode);
    glutAddMenuEntry("Filled", 1);
    glutAddMenuEntry("Outline", 2);
    glutCreateMenu(main_menu);
    glutAddMenuEntry("Quit", 666);
    glutAddSubMenu("Polygon mode", submenu);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 0;             /* ANSI C requires main to return int. */
}