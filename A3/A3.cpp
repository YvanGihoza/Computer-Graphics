//---------------------------------------
// Purpose: Display background using 
// Gouraud shading
// Author:  Yvan Gihoza
// 010785599
// CSCE 4813 Assignment #3
// Date:    March 2019
//---------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

using namespace std;

void init_normals();
void light1();
void light2();

#define SIZE 31
float Px[SIZE + 1][SIZE + 1];
float Py[SIZE + 1][SIZE + 1];
float Pz[SIZE + 1][SIZE + 1];
float Nx[SIZE + 1][SIZE + 1];
float Ny[SIZE + 1][SIZE + 1];
float Nz[SIZE + 1][SIZE + 1];
//Light location
//L1
float LX = 0.5;
float LY = 0.3;
float LZ = 0.8;
//L2
float LX2 = 0.34;
float LY2 = 0.25;
float LZ2 = 0;
//Light color
//L1
float LR[SIZE + 1][SIZE + 1];
float LG[SIZE + 1][SIZE + 1];
float LB[SIZE + 1][SIZE + 1];
//L2
float LR2[SIZE + 1][SIZE + 1];
float LG2[SIZE + 1][SIZE + 1];
float LB2[SIZE + 1][SIZE + 1];
//L1 colors
float r = 0.1, g = 0.4, b = 0.9;
//L1 colors
float r2 = 0.09, g2 = 0.25, b2 = 0.55;
//vector
float Vx[SIZE + 1][SIZE + 1];
float Vy[SIZE + 1][SIZE + 1];
float Vz[SIZE + 1][SIZE + 1];
//Light source
bool L1  = false;  
bool L2  = false;
//Rotation
int xRotate = 10;
int yRotate = 15;
int zRotate = 10;
bool Rmode  = false;

//---------------------------------------
// Init function
//---------------------------------------
void init()
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
   //normal
   init_normals();
   light1();
   light2();
}
//---------------------------------------
// Calculate random value between [-R..R]
//---------------------------------------
float myrand(float R)
{
   return (2 * R * rand()) / RAND_MAX - R;
}

//---------------------------------------
// Recursive function to split surface
//---------------------------------------
void split(int xlow, int xhigh, int ylow, int yhigh, float radius)
{
   // Check terminating condition
   if ((xhigh > xlow+1) || (yhigh > ylow+1))
   {
      // Calculate length of diagonal
      int xmid = (xhigh + xlow) / 2;
      int ymid = (yhigh + ylow) / 2;
      float dx = Px[xhigh][yhigh] - Px[xlow][ylow];
      float dy = Py[xhigh][yhigh] - Py[xlow][ylow];
      float dz = Pz[xhigh][yhigh] - Pz[xlow][ylow];
      float length = sqrt(dx * dx + dy * dy + dz * dz) / radius;

      // Generate five midpoints with random displacements
      Px[xlow][ymid] = (Px[xlow][ylow] + Px[xlow][yhigh]) / 2 + myrand(length);
      Py[xlow][ymid] = (Py[xlow][ylow] + Py[xlow][yhigh]) / 2 + myrand(length);
      Pz[xlow][ymid] = (Pz[xlow][ylow] + Pz[xlow][yhigh]) / 2 + myrand(length);

      Px[xhigh][ymid] = (Px[xhigh][ylow] + Px[xhigh][yhigh]) / 2 + myrand(length);
      Py[xhigh][ymid] = (Py[xhigh][ylow] + Py[xhigh][yhigh]) / 2 + myrand(length);
      Pz[xhigh][ymid] = (Pz[xhigh][ylow] + Pz[xhigh][yhigh]) / 2 + myrand(length);

      Px[xmid][ylow] = (Px[xlow][ylow] + Px[xhigh][ylow]) / 2 + myrand(length);
      Py[xmid][ylow] = (Py[xlow][ylow] + Py[xhigh][ylow]) / 2 + myrand(length);
      Pz[xmid][ylow] = (Pz[xlow][ylow] + Pz[xhigh][ylow]) / 2 + myrand(length);

      Px[xmid][yhigh] = (Px[xlow][yhigh] + Px[xhigh][yhigh]) / 2 + myrand(length);
      Py[xmid][yhigh] = (Py[xlow][yhigh] + Py[xhigh][yhigh]) / 2 + myrand(length);
      Pz[xmid][yhigh] = (Pz[xlow][yhigh] + Pz[xhigh][yhigh]) / 2 + myrand(length);

      Px[xmid][ymid] = (Px[xlow][ylow] + Px[xhigh][yhigh]) / 2 + myrand(length);
      Py[xmid][ymid] = (Py[xlow][ylow] + Py[xhigh][yhigh]) / 2 + myrand(length);
      Pz[xmid][ymid] = (Pz[xlow][ylow] + Pz[xhigh][yhigh]) / 2 + myrand(length);

      // Perform recursive calls
      split(xlow, xmid, ylow, ymid, radius);
      split(xmid, xhigh, ylow, ymid, radius);
      split(xlow, xmid, ymid, yhigh, radius);
      split(xmid, xhigh, ymid, yhigh, radius);
   }
}
//---------------------------------------
//Drawing The Surface Function
//---------------------------------------
void init_surface()
{
   Px[0][0] = -0.5;
   Py[0][0] = -0.5;
   Pz[0][0] = 0.0;
   Px[0][SIZE] = -0.5;
   Py[0][SIZE] = 0.5;
   Pz[0][SIZE] = 0.0;
   Px[SIZE][0] = 0.5;
   Py[SIZE][0] = -0.5;
   Pz[SIZE][0] = 0.0;
   Px[SIZE][SIZE] = 0.5;
   Py[SIZE][SIZE] = 0.5;
   Pz[SIZE][SIZE] = 0.0;
   split(0, SIZE, 0, SIZE, 20);

   // Add randoms waves to surface
   for (int wave = 1; wave <= 10; wave++)
   {
      int rand_i = rand() % SIZE / wave;
      int rand_j = rand() % SIZE / wave;
      float length = sqrt(rand_i * rand_i + rand_j * rand_j);
      if (length >= 10)
      for (int i = 0; i <= SIZE; i++)
      for (int j = 0; j <= SIZE; j++)
      {
         float angle = (rand_i * i + rand_j * j) / (length * length);
         Pz[i][j] += 0.01 * sin(angle * 2 * M_PI);
      }
   }   
}

//---------------------------------------
// Display callback for OpenGL
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
   // Draw the surface
   int i, j;

   for (i = 0; i < SIZE; i++)
      for (j = 0; j < SIZE; j++)
      {
	 glBegin(GL_POLYGON);
	 glColor3f(LR[i][j] + LR2[i][j], LG[i][j] + LG2[i][j], LB[i][j] + LB2[i][j]);
	 glVertex3f(Px[i][j], Py[i][j], Pz[i][j]);
	 glColor3f(LR[i][j] + LR2[i][j], LG[i][j] + LG2[i][j], LB[i][j] + LB2[i][j]);
	 glVertex3f(Px[i + 1][j], Py[i + 1][j], Pz[i + 1][j]);
	 glColor3f(LR[i][j] + LR2[i][j], LG[i][j] + LG2[i][j], LB[i][j] + LB2[i][j]);
	 glVertex3f(Px[i + 1][j + 1], Py[i + 1][j + 1], Pz[i + 1][j + 1]);
	 glColor3f(LR[i][j] + LR2[i][j], LG[i][j] + LG2[i][j], LB[i][j] + LB2[i][j]);
	 glVertex3f(Px[i][j + 1], Py[i][j + 1], Pz[i][j + 1]);
	 glEnd();
      }
   glFlush();
}
//---------------------------------------
// Initialize surface normals
//---------------------------------------
void init_normals()
{
   // Initialize surface normals
   for (int i=0; i<=SIZE; i++)
   for (int j=0; j<=SIZE; j++)
   {
      // Get tangents S and T
      float Sx = (i<SIZE) ? Px[i+1][j] - Px[i][j] : Px[i][j] - Px[i-1][j];
      float Sy = (i<SIZE) ? Py[i+1][j] - Py[i][j] : Py[i][j] - Py[i-1][j];
      float Sz = (i<SIZE) ? Pz[i+1][j] - Pz[i][j] : Pz[i][j] - Pz[i-1][j];
      float Tx = (j<SIZE) ? Px[i][j+1] - Px[i][j] : Px[i][j] - Px[i][j-1];
      float Ty = (j<SIZE) ? Py[i][j+1] - Py[i][j] : Py[i][j] - Py[i][j-1];
      float Tz = (j<SIZE) ? Pz[i][j+1] - Pz[i][j] : Pz[i][j] - Pz[i][j-1];
      
      // Calculate N = S cross T
      float Slength = sqrt(Sx*Sx + Sy*Sy + Sz*Sz);
      float Tlength = sqrt(Tx*Tx + Ty*Ty + Tz*Tz);
      if ((Slength * Tlength) > 0)
      {
         Nx[i][j] = (Sy*Tz - Sz*Ty) / (Slength * Tlength);
         Ny[i][j] = (Sz*Tx - Sx*Tz) / (Slength * Tlength);
         Nz[i][j] = (Sx*Ty - Sy*Tx) / (Slength * Tlength);
      }
   }
}
//---------------------------------------
// Keyboard function
//---------------------------------------
void keyboard(unsigned char key, int x, int y)
{
   //L1
   if(key == '1')
   {
      L1 = true;
      L2 = false;
      Rmode = false;
      cout << "L1" << endl;
      cout << "   'x' or 'X' - to change the location of the light on the x axis\n";
      cout << "   'y' or 'Y' - to change the location of the light on the y axis\n";
      cout << "   'z' or 'Z' - to change the location of the light on the z axis\n";
      cout << "   'r' or 'R' - to modify the RED color\n";
      cout << "   'g' or 'G' - to modify the GREEN color\n";
      cout << "   'b' or 'B' - to modify the BLUE color\n";
      cout << "   't' or 'T' - to go to ROTATION mode\n";
   }
   //L2
   else if(key == '2')
   {
      L1 = false;
      L2 = true;
      Rmode = false;
      cout << "L2" << endl;
      cout << "   'x' or 'X' - to change the location of the light on the x axis\n";
      cout << "   'y' or 'Y' - to change the location of the light on the y axis\n";
      cout << "   'z' or 'Z' - to change the location of the light on the z axis\n";
      cout << "   'r' or 'R' - to modify the RED color\n";
      cout << "   'g' or 'G' - to modify the GREEN color\n";
      cout << "   'b' or 'B' - to modify the BLUE color\n";
      cout << "   't' or 'T' - to go to ROTATION mode\n";
   }
   //Rotation mode
   else if(key == 't' || key =='T')
   {
      Rmode = true;
      L1 = false;
      L2 = false;
      cout << "Turning MODE" << endl;
      cout << "   'x' - rotate x-axis -5 degrees\n";
      cout << "   'X' - rotate x-axis +5 degrees\n";
      cout << "   'y' - rotate y-axis -5 degrees\n";
      cout << "   'Y' - rotate y-axis +5 degrees\n";
      cout << "   'z' - rotate z-axis -5 degrees\n";
      cout << "   'Z' - rotate z-axis +5 degrees\n";
      cout << "   '1' or '2' to change to mode 1 and 2\n";
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
   //L1
   if(L1)
   {
      //light position
      if(key == 'x')
         LX -= 0.5;
      else if(key == 'X')
         LX += 0.5;
      else if(key == 'y')
         LY -= 0.5;
      else if(key == 'Y')
         LY += 0.5;
      else if(key == 'z')
         LZ -= 0.5;
      else if(key == 'Z')
         LZ += 0.5;
      //color
      else if(key == 'r')
         r -= 0.08;
      else if(key == 'R')
         r += 0.08;
      else if(key == 'g')
         g -= 0.08;
      else if(key == 'G')
         g += 0.08;
      else if(key == 'b')
         b -= 0.08;
      else if(key == 'B')
         b += 0.08;
      light1();
   }
   //L2 
   if(L2)
   {
      //light position
      if(key == 'x')
         LX2 -= 0.2;
      else if(key == 'X')
         LX2 += 0.2;
      else if(key == 'y')
         LY2 -= 0.2;
      else if(key == 'Y')
         LY2 += 0.2;
      else if(key == 'z')
         LZ2 -= 0.2;
      else if(key == 'Z')
         LZ2 += 0.2;
      //color
      else if(key == 'r')
         r2 -= 0.05;
      else if(key == 'R')
         r2 += 0.05;
      else if(key == 'g')
         g2 -= 0.05;
      else if(key == 'G')
         g2 += 0.05;
      else if(key == 'b')
         b2 -= 0.05;
      else if(key == 'B')
         b2 += 0.05;
      light2();
   }
   // Redraw objects
   glutPostRedisplay();
}
//---------------------------------------
//Diffuse L1
//---------------------------------------
void light1()
{
   float D, product;
   for (int i=0; i<=SIZE; i++)
   for (int j=0; j<=SIZE; j++)
   {
      //finding distance between 2 points
      Vx[i][j] = Px[i][j] - LX;
      Vy[i][j] = Py[i][j] - LY;
      Vz[i][j] = Pz[i][j] - LZ;
      //dot product
      D = sqrt((Vx[i][j] * Vx[i][j]) + (Vy[i][j] * Vy[i][j]) + (Vz[i][j] * Vz[i][j]));
      product = ((Vx[i][j] * Nx[i][j]) + (Vy[i][j] * Ny[i][j]) + (Vz[i][j] * Nz[i][j]))/(1+ 1*D+ 1*(D*D));
      LR[i][j] = product + r; 
      LG[i][j] = product + g;
      LB[i][j] = product + b;
   }
}
//---------------------------------------
//Diffuse L2
//---------------------------------------
void light2()
{
   float D, product;
   for (int i=0; i<=SIZE; i++)
   for (int j=0; j<=SIZE; j++)
   {
      //finding distance between 2 points
      Vx[i][j] = Px[i][j] - LX2;
      Vy[i][j] = Py[i][j] - LY2;
      Vz[i][j] = Pz[i][j] - LZ2;
      //dot product
      D = sqrt((Vx[i][j] * Vx[i][j]) + (Vy[i][j] * Vy[i][j]) + (Vz[i][j] * Vz[i][j]));
      product = ((Vx[i][j] * Nx[i][j]) + (Vy[i][j] * Ny[i][j]) + (Vz[i][j] * Nz[i][j]))/(1+ 1*D+ 1*(D*D));
      LR2[i][j] = product + r2; 
      LG2[i][j] = product + g2;
      LB2[i][j] = product + b2;
   }
}
//---------------------------------------
// Main program
//---------------------------------------
int main(int argc, char *argv[])
{
   glutInit(&argc, argv);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(250, 250);
   glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   glutCreateWindow("A3");
   glutDisplayFunc(display);
   glutKeyboardFunc(keyboard);
   init_surface();
   init();
   cout << "Keyboard commands:\n";
   cout << "   't' or 'T' for Turning mode\n";
   cout << "   '1' or '2' Light 1 and 2\n";
   glutMainLoop();
   return 0;
}

