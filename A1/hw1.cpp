
//---------------------------------------
// Purpose: To generate images of fireworks
// Author:  Yvan Gihoza
// 010785599
// CSCE 4813 Assignment #1
// Date:    January 2019
//---------------------------------------

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void idle();


float x = -0.5;
float y = -0.5;
float z = -0.5;

float xR = 0.5;
float yR = 0.5;
float zR = 0.5;

float xG = 0.55;
float yG = -0.50;
float zG = -0.50;

float xM = -0.5;
float yM = 0.5;
float zM = -0.5;

float xM2 = 0.15;
float yM2 = 0.15;
float zM2 = 0.5;

//Random values function
float myrand(float R)
{
   return (2 * R * rand()) / RAND_MAX - R;
}
//---------------------------------------
// Init function
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}
//---------------------------------------
// Display 
//---------------------------------------
void display()
{
   //to generate random number of colors
   srand(time(NULL));
   int color1 = rand()% 255 + 1;
   int color2 = rand()% 255 + 1;
   int color3 = rand()% 255 + 1;

   glClear(GL_COLOR_BUFFER_BIT);
   glBegin(GL_LINE_STRIP); 

   //BLUE fireworks
   for(int i = 0; i < 50; i++)
   {
      glColor3ub(color1, color2, 1.0);
      glVertex3f(x, y, z);
      glVertex3f(x+myrand(x), y+myrand(y), z+myrand(z));
   }

   //RED fireworks
   for(int i = 0; i < 50; i++)
   {
      glColor3ub(1.0, color2, color3);
      glVertex3f(xR, yR, zR);
      glVertex3f(xR+myrand(xR), yR+myrand(yR), zR+myrand(zR));
   }

   //GREEN fireworks
   for(int i = 0; i < 55; i++)
   {
      glColor3ub(color1, 1.0, color3);
      glVertex3f(xG, yG, zG);
      glVertex3f(xG+myrand(xG), yG+myrand(yG), zG+myrand(zG));
   }

   //RANDOM fireworks
   for(int i = 0; i < 100; i++)
   {
      glColor3ub(color1, color2, color3);
      glVertex3f(xM, yM, zM);
      glVertex3f(xM+myrand(xM), yM+myrand(yM), zM+myrand(zM));
   }

   //RANDOM fireworks
   for(int i = 0; i < 100; i++)
   {
      glColor3ub(color1, color2, color3);
      glVertex3f(xM2, yM2, zM2);
      glVertex3f(xM2+myrand(xM2), yM2+myrand(yM2), zM2+myrand(zM2));
   }
   glEnd(); 
   glFlush();  
}

//---------------------------------------
// Idle callback for OpenGL
//---------------------------------------
void idle()
{
   if (system("sleep 1") == -1)
      printf("Error: could not execute system command\n");
   glutPostRedisplay();
}

//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(1000, 1000);
   glutInitWindowPosition(0,0);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("Fireworks");
   glutDisplayFunc(display);
   glutIdleFunc(idle);
   init();
   glutMainLoop();
   return 0;
}
