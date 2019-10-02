
//---------------------------------------
// Purpose: Game to interactively create
// and display a virtual world by stacking
// blocks over each other
// Author:  Yvan Gihoza
// 010785599
// CSCE 4813 Assignment #2
// Date:    Feb 2019
//---------------------------------------

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

void cube(float midx, float midy, float midz, float size);
void Maincube(float midx, float midy, float midz, float size);

GLenum mode = GL_POLYGON;

int xRotate = 10;
int yRotate = 15;
int zRotate = 10;
int cubes = 0;
int origin = 0;

#define place 10000
#define SIZE 0.08

float xPos [place];
float yPos [place];
float zPos [place];

bool Rmode  = false;  
bool Fmode  = false;
//---------------------------------------
// Init function
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   glEnable(GL_DEPTH_TEST);
   glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------
// Keyboard function
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   //Rotation Mode
   if(key == 'r' || key =='R')
   {
      Rmode = true;
      Fmode = false;
      cout << "ROTATION MODE" << endl;
      cout << "   'x' - rotate x-axis -5 degrees\n";
      cout << "   'X' - rotate x-axis +5 degrees\n";
      cout << "   'y' - rotate y-axis -5 degrees\n";
      cout << "   'Y' - rotate y-axis +5 degrees\n";
      cout << "   'z' - rotate z-axis -5 degrees\n";
      cout << "   'Z' - rotate z-axis +5 degrees\n";
   }

   //Fly mode
   else if(key == 'f' || key == 'F')
   {
      Fmode = true;
      Rmode = false;
      cout << "FLY MODE" << endl;
      cout << "   'x' - move x-axis -0.1\n";
      cout << "   'X' - move x-axis +0.1\n";
      cout << "   'y' - move y-axis -0.1\n";
      cout << "   'Y' - move y-axis +0.1\n";
      cout << "   'z' - move z-axis -0.1\n";
      cout << "   'Z' - move z-axis +0.1\n";
      cout << "   '+' to create cubes\n";
      cout << "   '-' to remove cubes (ONLY CREATED CUBES CAN BE REMOVED!!)\n";
   }

   if(Rmode)
   {
      if(key == 'x')
         xRotate -= 5;
      else if(key == 'X')
            xRotate += 5;
      else if(key == 'y')
         yRotate -= 5;
      else if(key == 'Y')
         yRotate += 5;
      else if(key == 'z')
         zRotate -= 5;
      else if(key == 'Z')
         zRotate += 5;
   }
   
   else if(Fmode)
   {
      if(key == 'x')
      {
         if(xPos[origin] > -0.6)
            xPos[origin] -= 0.1;
      }
      else if(key == 'X')
      {
         if(xPos[origin] < 0.6)
            xPos[origin] += 0.1;
      }
      else if(key == 'y')
      {
         if(yPos[origin] > -0.6)
            yPos[origin] -= 0.1;
      }
      else if(key == 'Y')
      {
         if(yPos[origin] < 0.6) 
            yPos[origin] += 0.1;
      }
      else if(key == 'z')
      {
         if(zPos[origin] > -0.6)
            zPos[origin] -= 0.1;
      }
      else if(key == 'Z')
      {
         if(zPos[origin] < 0.6)
            zPos[origin] += 0.1;
      }
      //adding cubes
      if(key == '+')
      {  
         cubes++;   
         xPos[cubes] = xPos[origin];
         yPos[cubes] = yPos[origin];
         zPos[cubes] = zPos[origin];
         if(xPos[cubes] < 0.6 && yPos[cubes] < 0.6 && yPos[cubes] < 0.6)
            Maincube(xPos[cubes], yPos[cubes], zPos[cubes], SIZE);   
      }
      //removing cubes
      if(key == '-')
      {  
         for(int i = 1; i <= cubes; i++)
         {
            if(xPos[i] == xPos[origin] && yPos[i] == yPos[origin] && zPos[i] == zPos[origin] )
               for(int j = i; j <= cubes; j++)
               {
                  xPos[j]= xPos[j+1];
                  yPos[j]= yPos[j+1];
                  zPos[j]= yPos[j+1];
               }
            cubes--; 
         }     
      }
   }
  
   // Redraw objects
   glutPostRedisplay();
}
//
void Maincube(float midx, float midy, float midz, float size)
{
   glColor3f(0.0, 0.0, 1.0);
   glTranslatef(midx, midy, midz);
   glutWireCube(size);
}
//---------------------------------------
// Function to draw 3D cube
//---------------------------------------
void cube(float midx, float midy, float midz, float size)
{
   // Define 8 vertices
   float ax = midx - size / 2;
   float ay = midy - size / 2;
   float az = midz + size / 2;
   float bx = midx + size / 2;
   float by = midy - size / 2;
   float bz = midz + size / 2;
   float cx = midx + size / 2;
   float cy = midy + size / 2;
   float cz = midz + size / 2;
   float dx = midx - size / 2;
   float dy = midy + size / 2;
   float dz = midz + size / 2;
   float ex = midx - size / 2;
   float ey = midy - size / 2;
   float ez = midz - size / 2;
   float fx = midx + size / 2;
   float fy = midy - size / 2;
   float fz = midz - size / 2;
   float gx = midx + size / 2;
   float gy = midy + size / 2;
   float gz = midz - size / 2;
   float hx = midx - size / 2;
   float hy = midy + size / 2;
   float hz = midz - size / 2;

   // Draw 6 faces
   glBegin(GL_LINE_LOOP);
   glColor3f(1.0, 0.0, 0.0);
   glVertex3f(ax, ay, az);
   glVertex3f(bx, by, bz);
   glVertex3f(cx, cy, cz);
   glVertex3f(dx, dy, dz);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glColor3f(0.0, 1.0, 0.0);
   glVertex3f(ax, ay, az);
   glVertex3f(dx, dy, dz);
   glVertex3f(hx, hy, hz);
   glVertex3f(ex, ey, ez);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glColor3f(0.0, 0.0, 1.0);
   glVertex3f(ax, ay, az);
   glVertex3f(ex, ey, ez);
   glVertex3f(fx, fy, fz);
   glVertex3f(bx, by, bz);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glColor3f(0.0, 1.0, 1.0);
   glVertex3f(gx, gy, gz);
   glVertex3f(fx, fy, fz);
   glVertex3f(ex, ey, ez);
   glVertex3f(hx, hy, hz);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glColor3f(1.0, 0.0, 1.0);
   glVertex3f(gx, gy, gz);
   glVertex3f(cx, cy, cz);
   glVertex3f(bx, by, bz);
   glVertex3f(fx, fy, fz);
   glEnd();

   glBegin(GL_LINE_LOOP);
   glColor3f(1.0, 1.0, 0.0);
   glVertex3f(gx, gy, gz);
   glVertex3f(hx, hy, hz);
   glVertex3f(dx, dy, dz);
   glVertex3f(cx, cy, cz);
   glEnd();
}
//---------------------------------------
// Display 
//---------------------------------------
void display()
{
 
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(xRotate, 1.0, 0.0, 0.0);
   glRotatef(yRotate, 0.0, 1.0, 0.0);
   glRotatef(zRotate, 0.0, 0.0, 1.0);
   //Big cube (acting as walls)
   cube(0.0, 0.0, 0.0, 1.3);
   //small cube (acting as a player)
   cube(xPos[origin],yPos[origin],zPos[origin], SIZE);
   Maincube(0.0, 0.3, 0.0, SIZE);
   Maincube(0.0, 0.2, 0.0, SIZE);
   Maincube(0.0, 0.1, 0.0, SIZE);
   Maincube(0.3, 0.0, 0.0, SIZE);
   Maincube(0.15, -0.4, -0.4, SIZE);
   for(int i = 1; i <cubes; i++)
      Maincube(xPos[i], yPos[i], zPos[i], SIZE);
   glutSwapBuffers();  
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(500,500);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow("Block World");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init();
   cout << "Keyboard commands:\n";
   cout << "   'r' or 'R' for rotation mode\n";
   cout << "   'f' or 'F' for flying mode\n";
   glutMainLoop();
   return 0;
}
