//---------------------------------------
// Program: A5.cpp
// Purpose: Video game "MazeWorld"       
// Author:  Yvan Gihoza
// Date:    March 2019
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>
#include <string>
#include "libim/im_color.h"

using namespace std;

//Variable of the text file
//ifstream inFile ("maze.txt");
string str;

// Global variables 
#define ROW 20
#define COL 20
#define place 100
#define SIZE 0.08
int rows,cols, startRow, startCol;
unsigned char *texture_brick, *texture_rock , *texture_wood , *texture_grass, *texture_player;
int move = 0.5;
int xdim;
int ydim;
int Xorigin = 0;
int Yorigin = 18;
int Zorigin = 0;
int xRotate = 5;
int yRotate = 5;
int zRotate = 0;

float xPos [place];
float yPos [place];
float zPos [place];
unsigned char maze[ROW][COL];

//---------------------------------------
// Function to draw 3D block
//---------------------------------------
void block(float xmin, float ymin, float zmin,
           float xmax, float ymax, float zmax)
{
   // Define 8 vertices
   float ax = xmin, ay = ymin, az = zmax;
   float bx = xmax, by = ymin, bz = zmax;
   float cx = xmax, cy = ymax, cz = zmax;
   float dx = xmin, dy = ymax, dz = zmax;

   float ex = xmin, ey = ymin, ez = zmin;
   float fx = xmax, fy = ymin, fz = zmin;
   float gx = xmax, gy = ymax, gz = zmin;
   float hx = xmin, hy = ymax, hz = zmin;

   // Draw 6 faces
   glBegin(GL_POLYGON);  // Max texture coord 1.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(bx, by, bz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(cx, cy, cz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(dx, dy, dz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 1.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(hx, hy, hz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 1.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(hx, hy, hz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(gx, gy, gz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 1.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(bx, by, bz);
   glTexCoord2f(1.0, 0.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(1.0, 1.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(0.0, 1.0);
   glVertex3f(cx, cy, cz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 3.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(ax, ay, az);
   glTexCoord2f(2.0, 0.0);
   glVertex3f(ex, ey, ez);
   glTexCoord2f(2.0, 2.0);
   glVertex3f(fx, fy, fz);
   glTexCoord2f(0.0, 2.0);
   glVertex3f(bx, by, bz);
   glEnd();

   glBegin(GL_POLYGON);  // Max texture coord 3.0
   glTexCoord2f(0.0, 0.0);
   glVertex3f(gx, gy, gz);
   glTexCoord2f(3.0, 0.0);
   glVertex3f(cx, cy, cz);
   glTexCoord2f(3.0, 3.0);
   glVertex3f(dx, dy, dz);
   glTexCoord2f(0.0, 3.0);
   glVertex3f(hx, hy, hz);
   glEnd();
}

//---------------------------------------
// Initialize texture image
//---------------------------------------
void init_texture(char *name, unsigned char *&texture, int &xdim, int &ydim)
{
   // Read jpg image
   im_color image;
   image.ReadJpg(name);
   //printf("Reading image %s\n", name);
   xdim = 1; while (xdim < image.R.Xdim) xdim*=2; xdim /=2;
   ydim = 1; while (ydim < image.R.Ydim) ydim*=2; ydim /=2;
   image.Interpolate(xdim, ydim);
   //printf("Interpolating to %d by %d\n", xdim, ydim);

   // Copy image into texture array
   texture = (unsigned char *)malloc((unsigned int)(xdim*ydim*3));
   int index = 0;
   for (int y = 0; y < ydim; y++)
      for (int x = 0; x < xdim; x++)
      {
         texture[index++] = (unsigned char)(image.R.Data2D[y][x]);
         texture[index++] = (unsigned char)(image.G.Data2D[y][x]);
         texture[index++] = (unsigned char)(image.B.Data2D[y][x]);
      }
}

//---------------------------------------
// Init function
//---------------------------------------
void init()
{
   // Init view
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
   glEnable(GL_DEPTH_TEST);
   glMatrixMode(GL_MODELVIEW);
   init_texture((char *)"textures/brick.jpg", texture_brick, xdim, ydim);
   init_texture((char *)"textures/wood.jpg", texture_wood, xdim, ydim);
   init_texture((char *)"textures/rock.jpg", texture_rock, xdim, ydim);
   init_texture((char *)"textures/grass.jpg", texture_grass, xdim, ydim);
   init_texture((char *)"textures/lava.jpg", texture_player, xdim, ydim);
   glEnable(GL_TEXTURE_2D);
   
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
//---------------------------------------
// Display 
//---------------------------------------
void display()
{
   // Incrementally rotate objects
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(xRotate, 1.0, 0.0, 0.0);
   glRotatef(yRotate, 0.0, 1.0, 0.0);
   glRotatef(zRotate, 0.0, 0.0, 1.0);
   
   //Variable of the text file
   ifstream inFile ("maze.txt");
   //getting the rows and columns
   inFile >> rows >> cols >> startRow >> startCol;
   getline(inFile, str);
   //store the file in the array
   for(int r = 0; r < rows; r++)
   {
      for(int c = 0; c < cols; c++)
      {
         //maze[rows][cols] = str[c];
         inFile >> noskipws >> maze[r][c];
      }
      getline(inFile, str);
   }
   for(int i=0; i < rows; i++)
      xPos[i] = (i*0.1f);
   for(int j=0; j < cols; j++)
      yPos[j] = (j*0.1f);
   //Drawing the maze
   for(int i = 0; i < rows; i++)
   {
      for(int j = 0; j < cols; j++)
      {
         //brick
         if(maze[i][j] == 'b')
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_brick);
         //wood
         else if(maze[i][j] == 'w')
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_wood);
         //brick
         else if(maze[i][j] == 'r')
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_rock);
         //grass
         else if(maze[i][j] == ' ')
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_grass);
         block(xPos[i], yPos[j], 0.0f, xPos[i]+0.1f, yPos[j]+0.1f, 0.1f);
      }
   }
   //player
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_player );
   block(xPos[Xorigin], yPos[Yorigin], 0.0f, xPos[Xorigin]+0.1f, yPos[Yorigin]+0.1f, 0.1f);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_player );
   block(Xorigin, Yorigin, Zorigin+0.0f, Xorigin+0.1f, Yorigin+0.1f, Zorigin+0.1f);
   
   glFlush();
}
//---------------------------------------
// Keyboard function
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   //Moving right
   if(key == 'd')
   {
         Xorigin += 1;   
   }
   //Moving left
   else if(key == 'a')
   {
      Xorigin -= 1;
   }
   //Moving up
   else if(key == 'w')
   {
      Yorigin += 1;
   }
   //Moving down
   else if(key == 's')
   {
      Yorigin -= 1;
   }
   // Redraw objects
   glutPostRedisplay();
}
//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(500,500);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("MazeWorld");
   glutKeyboardFunc(keyboard);
   init();
   glutDisplayFunc(display);
   cout << "Keyboard commands:\n";
   cout << "   'd' To move right\n";
   cout << "   'a' To move left\n";
   cout << "   'w' To move up\n";
   cout << "   's' To move down\n";
   glutMainLoop();
   return 0;
}

