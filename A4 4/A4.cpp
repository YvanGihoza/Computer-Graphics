//---------------------------------------
// Program: A4.cpp
// Purpose: Toolbox        
// Author:  Yvan Gihoza
// Date:    March 2019
//---------------------------------------
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <vector>

#define IN 0
#define LEFT 1
#define RIGHT 2
#define TOP 8
#define BOTTOM 4
typedef int Code;
float xmin, ymin, xmax, ymax;

using namespace std;

//---------------------------------------
// Init function for OpenGL
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

//---------------------------------------
// Display callback for OpenGL
//---------------------------------------
void display()
{
   glClear(GL_COLOR_BUFFER_BIT);
   glEnd();
   glFlush();
}
//GEOMETRIC OPERATIONS
//---------------------------------------
//Translation
//---------------------------------------
void translation(float x, float y, float z, float tx, float ty, float tz)
{
   float px, py, pz;
   float point[] = {x, y, z};
   px = x + tx;
   py = y + ty;
   pz = z + tz;
   //store the points
   point[0] = px;
   point[1] = py;
   point[2] = pz;
}
//---------------------------------------
//Rotation
//---------------------------------------
//Rotation on X axis function
void rotateX(float theta, float x, float y, float z)
{
   float xR, yR, zR;
   float point[] = {x, y, z};
   xR = x;
   yR = y * cos(theta) - z * sin(theta);
   zR = y * sin (theta) + z * cos(theta);
   //store the points
   point[0] = xR;
   point[1] = yR;
   point[2] = zR;
}
//Rotation on y axis function
void rotateY(float theta, float x, float y, float z)
{
   float xR, yR, zR;
   float point[] = {x, y, z};
   xR = x * cos(theta) + z * sin(theta);
   yR = y;
   zR = -x * sin(theta) + z * cos(theta);
   //store the points
   point[0] = xR;
   point[1] = yR;
   point[2] = zR;
}
//Rotation on z axis function
void rotateZ(float theta, float x, float y, float z)
{
   float xR, yR, zR;
   float point[] = {x, y, z};
   xR = x * cos(theta) - y * sin(theta);
   yR = x * sin(theta) + y * cos(theta);
   zR = z;
   //store the points
   point[0] = xR;
   point[1] = yR;
   point[2] = zR;
}
//---------------------------------------
//Scaling
//---------------------------------------
void scale(float x, float y, float z, float sx, float sy, float sz)
{
   float xScale, yScale, zScale;
   float point[] = {x, y, z};
   xScale = x * sx;
   yScale = y * sy;
   zScale = z * sz;
   //store the points
   point[0] = xScale;
   point[1] = yScale;
   point[2] = zScale;
}
//---------------------------------------
//Projection
//---------------------------------------
void projection(float d, float x, float y, float z)
{
   float xp, yp, zp;
   float point[] = {x, y, z};
   xp = x / (z/d);
   yp = y / (z/d);
   zp = d;
   //store the points
   point[0] = xp;
   point[1] = yp;
   point[2] = zp;
}
//CLIPPING OPERATIONS
//---------------------------------------
//Cohen-Sutherland code
//---------------------------------------
Code code(float x, float y)
{
   Code cd;
   cd = IN;
   if(x < xmin)
      cd |= LEFT;
   else if(x > xmax)
      cd |= RIGHT;
   if(y < ymin)
      cd |= BOTTOM;
   else if(y > ymax)
      cd |= TOP;
   return cd;
}

bool acceptLine(int x1, int y1, int x2, int y2)
{
   int c1 = code(x1,y1);
   int c2 = code(x2,y2);
   if (c1 == 0 && c2 == 0)
      return true;
}

bool rejectLine(int x1, int y1, int x2, int y2)
{
   int c1 = code(x1,y1);
   int c2 = code(x2,y2);
   if (c1 != 0 && c2 != 0)
      return true;
}

bool clipLine(int x1, int y1, int x2, int y2)
{
   int c1 = code(x1,y1);
   int c2 = code(x2,y2);
   if (c1 != 0 && c2 == 0)
      return true;
   else if(c1 == 0 && c2 != 0)
      return true;
}
//SHADING OPERATIONS
//---------------------------------------
//NORMALIZE 
//---------------------------------------
void normalize(float Ax, float Ay, float Az)
{
   float Bx, By, Bz;
   float point[] = {Ax, Ay, Az};
   float length= sqrt((Ax*Ax)+(Ay*Ay)+(Az*Az));
   Bx = Ax/length;
   By = Ay/length;
   Bz = Az/length;
   //store the points
   point[0] = Bx;
   point[1] = By;
   point[2] = Bz;
}
//---------------------------------------
//CROSS PRODUCT 
//---------------------------------------
void crossProd(float Ax, float Ay, float Az, float Bx, float By, float Bz)
{
   float Cx,Cy, Cz;
   float point[] = {Cx, Cy, Cz};
   Cx = Ay * Bz - Az * By;
   Cy = Az * Bx - Ax * Bz;
   Cz = Ax * By - Ay * Bx;
   //store the points
   point[0] = Cx;
   point[1] = Cy;
   point[2] = Cz;
}
//---------------------------------------
//DOT PRODUCT 
//---------------------------------------
float dotProd(float Ax, float Ay, float Az, float Bx, float By, float Bz)
{
   float point[] = {};
   float result = Ax * Bx + Ay * By + Az * Bz;
   point[0] = result;
   return result;
}
//---------------------------------------
//DIFFUSE TERM
//---------------------------------------
void diffuse(float Lx, float Ly, float Lz, float Nx, float Ny, float Nz)
{
   dotProd(Lx,Ly,Lz,Nx,Ny,Nz);
}
//---------------------------------------
//IDEAL REFLECTION
//---------------------------------------
void idealReflect(float Lx, float Ly, float Lz, float Nx, float Ny, float Nz)
{
   float Rx, Ry, Rz;
   float point[] = {Rx, Ry, Rz};
   Rx = 2 * (Lx * Nx) * (Nx - Lx);
   Ry = 2 * (Ly * Ny) * (Ny - Ly);
   Rz = 2 * (Lz * Nz) * (Nz - Lz);
   //store the points
   point[0] = Rx;
   point[1] = Ry;
   point[2] = Rz;  
}
//---------------------------------------
//SPECULAR TERM
//---------------------------------------
float spec(float p, float Vx, float Vy, float Vz, float Rx, float Ry, float Rz)
{
   float specTerm = dotProd(Vx,Vy,Vz,Rx,Ry,Rz);
   return pow(specTerm,p);
}
//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   /*(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("A4");
   glutDisplayFunc(display);
   init();
   glutMainLoop();*/
   return 0;
}
