/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include<windows.h>
#include<bits/stdc++.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#define pi (2*acos(0.0))
using namespace std;
double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
float angleto_cylinder=0.0;
float partial_angletoRotate_cylinder=0.0;
float angletoRotate_cylinder_z=0.0;
int x_ax_cylinder=0;
int y_ax_cylinder=1;
int z_ax_cylinder=0;
float anglerotate_as=0;
int x_as=0;
int y_as=0;
int z_as=0;
struct point
{
	double x,y,z;
};

point position,u,r,l;
void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}


void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    glColor3f(178,190,181);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
    //glRotatef(90,0,1,0);
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    if(j%2==0)
			    {glColor3f(0.0, 0.0, 0.0);
			    }
			    else {
                   glColor3f(255.0,255.0,255.0);
			    }

				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);

				glRotatef(50,0,1,0);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}

	}
}
void drawupperHalfsphere(double radius,int slices,int stacks)
{
    //glRotatef(90,0,1,0);
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    if(j%2==0)
			    {glColor3f(0.0, 0.0, 0.0);
			    }
			    else {
                   glColor3f(255.0,255.0,255.0);
			    }

				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);


			}glEnd();
		}

	}
}
void drawLowerhalfsphere(double radius,int slices,int stacks)
{
   // glRotatef(anglerotate_as,0,y_as,0);
   // glTranslatef(0,0,50);
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));

		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    if(j%2==0)
			    {glColor3f(0.0, 0.0, 0.0);
			    }
			    else {
                   glColor3f(255.0,255.0,255.0);
			    }

                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}
void drawHorn(double outerradius,double radius,int slices,int stacks)
{



     //glRotatef(anglerotate_as,0,y_as,0);
    //glRotatef(90,0,1,0);
  //  glTranslatef(0,0,25);
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi));
		double rc;
		rc=outerradius-r;
		//printf("radius %f",rc);

		for(j=0;j<=slices;j++)
		{
			points[i][j].x=rc*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=rc*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    if(j%2==0)
			    {glColor3f(0.0, 0.0, 0.0);
			    }
			    else {
                   glColor3f(255.0,255.0,255.0);
			    }

                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
			}glEnd();
		}
	}
}
void drawCylinder(double radius,int slices,int stacks,int shift)
{
     //glRotatef(anglerotate_as,0,y_as,0);
    //glRotatef(angletoRotate_cylinder,x_ax_cylinder,y_ax_cylinder,z_ax_cylinder);
    glTranslatef(0,0,shift);
	struct point points[200][200];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius;
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    if(j%2==0)
			    {glColor3f(0.0, 0.0, 0.0);
			    }
			    else {
                   glColor3f(255.0,255.0,255.0);
			    }
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}
void drawLine(double a)
{
    glColor3f(0,128,0);
    glBegin(GL_LINES);
    {
        glVertex3f(-a,a,0);
        glVertex3f(a,a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a,a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(a,-a,0);
        glVertex3f(a,-a,0);
        glVertex3f(a,a,0);
    }
    glEnd();
}

void rotateFull(){
    glPushMatrix();
    {
         glRotatef(-90,0,1,0);
         glRotatef(-70,1,0,0);
        glTranslatef(0,0,400);
        drawSquare(50);

    }
    glPopMatrix();
    //glRotatef(-90,0,1,0);
    glRotatef(-90,1,0,0);
    glRotatef(angleto_cylinder,x_ax_cylinder,y_ax_cylinder,z_ax_cylinder);
    drawLowerhalfsphere(50,50,50);
    glRotatef(partial_angletoRotate_cylinder,1,0,0);
    drawupperHalfsphere(50,50,50);

    glRotatef(anglerotate_as,1,0,0);
    glRotatef(angletoRotate_cylinder_z,0,0,1);
    glPushMatrix();
    {
        glTranslatef(0,0,65);
        drawLowerhalfsphere(20,50,50);

    }
      glPopMatrix();
    // drawLowerhalfsphere(25,50,50);
    glTranslatef(0,0,80);

    glPushMatrix();
    {
     drawCylinder(20,50,199,0);
     glTranslatef(0,0,40);
     //glTranslatef(0,0,30);
     drawCylinder(20,50,199,0);
     glTranslatef(0,0,40);
     drawCylinder(20,50,199,0);
     glTranslatef(0,0,40);
     drawCylinder(20,50,199,0);
     glTranslatef(0,0,20);
     drawCylinder(20,50,199,0);
     glTranslatef(0,0,20);
    drawHorn(40,20,90,90);


    }
    glPopMatrix();

}
/*void rotate_as(){
     glRotatef(anglerotate_as,0,y_as,0);
     drawHalfSphere(25,50,50);
     drawCylinder(25,50,99,25);
     drawCylinder(25,50,99,50);
     drawCylinder(25,50,99,50);
     drawHorn(50,25,90,90);
}*/
void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}
point CrossProduct(struct point vector1, struct point vector2){
    struct point result;
    result.x=vector1.y*vector2.z-vector1.z*vector2.y;
    result.y=vector1.z*vector2.x-vector1.x*vector2.z;
    result.z=vector1.x*vector2.y-vector1.y*vector2.x;
    //printf(" vector is %f %f %f",result.x,result.y,result.z);
    return result;
}
void Normalize(struct point vector1){
    /*printf(" vector %d",&vector1.x);
     printf(" vector %d",&vector1.y);
      printf(" vector %d",&vector1.z);*/
    double mag=sqrt(vector1.x*vector1.x+vector1.y*vector1.y+vector1.z*vector1.z);
    vector1.x=vector1.x/mag;
    vector1.y=vector1.y/mag;
    vector1.z=vector1.z/mag;

}
void rotate_left(){
    double rangle;
    rangle=(angle*1.0*pi)/180;
    point cross_mult_1,cross_mult_2;
    cross_mult_1=CrossProduct(u,l);
    //printf(" vector is %f %f %f",cross_mult_1.x,cross_mult_1.y,cross_mult_1.z);
    cross_mult_2=CrossProduct(u,r);
    //printf(" vector is %f %f %f",l.x,l.y,l.z);
    l.x=l.x*cos(rangle)+cross_mult_1.x*sin(rangle);
    l.y=l.y*cos(rangle)+cross_mult_1.y*sin(rangle);
    l.z=l.z*cos(rangle)+cross_mult_1.z*sin(rangle);
    r.x=r.x*cos(rangle)+cross_mult_2.x*sin(rangle);
    r.y=r.y*cos(rangle)+cross_mult_2.y*sin(rangle);
    r.z=r.z*cos(rangle)+cross_mult_2.z*sin(rangle);
    Normalize(l);
    //printf("After normalizing vector is %f %f %f",l.x,l.y,l.z);
    Normalize(r);



}
void rotate_right(){
    double rangle;
    rangle=(angle*1.0*pi)/180;
    point cross_mult_1,cross_mult_2;
    cross_mult_1=CrossProduct(u,l);
    cross_mult_2=CrossProduct(u,r);
    l.x=l.x*cos(rangle)-cross_mult_1.x*sin(rangle);
    l.y=l.y*cos(rangle)-cross_mult_1.y*sin(rangle);
    l.z=l.z*cos(rangle)-cross_mult_1.z*sin(rangle);
    r.x=r.x*cos(rangle)-cross_mult_2.x*sin(rangle);
    r.y=r.y*cos(rangle)-cross_mult_2.y*sin(rangle);
    r.z=r.z*cos(rangle)-cross_mult_2.z*sin(rangle);
    Normalize(l);
    Normalize(r);



}
void look_down(){
    double rangle;
    rangle=(angle*1.0*pi)/180;
    point cross_mult_1,cross_mult_2;
    cross_mult_1=CrossProduct(r,l);
    cross_mult_2=CrossProduct(r,u);
    l.x=l.x*cos(rangle)-cross_mult_1.x*sin(rangle);
    l.y=l.y*cos(rangle)-cross_mult_1.y*sin(rangle);
    l.z=l.z*cos(rangle)-cross_mult_1.z*sin(rangle);
    u.x=u.x*cos(rangle)-cross_mult_2.x*sin(rangle);
    u.y=u.y*cos(rangle)-cross_mult_2.y*sin(rangle);
    u.z=u.z*cos(rangle)-cross_mult_2.z*sin(rangle);
    Normalize(l);
    Normalize(u);

}
void look_up(){
    double rangle;
    rangle=(angle*1.0*pi)/180;
    point cross_mult_1,cross_mult_2;
    cross_mult_1=CrossProduct(r,l);
    cross_mult_2=CrossProduct(r,u);
    l.x=l.x*cos(rangle)+cross_mult_1.x*sin(rangle);
    l.y=l.y*cos(rangle)+cross_mult_1.y*sin(rangle);
    l.z=l.z*cos(rangle)+cross_mult_1.z*sin(rangle);
    u.x=u.x*cos(rangle)+cross_mult_2.x*sin(rangle);
    u.y=u.y*cos(rangle)+cross_mult_2.y*sin(rangle);
    u.z=u.z*cos(rangle)+cross_mult_2.z*sin(rangle);
    Normalize(l);
    Normalize(u);

}
void anti_clock(){
    double rangle;
    rangle=(angle*1.0*pi)/180;
    point cross_mult_1,cross_mult_2;
    cross_mult_1=CrossProduct(l,u);
    cross_mult_2=CrossProduct(l,r);
    u.x=u.x*cos(rangle)+cross_mult_1.x*sin(rangle);
    u.y=u.y*cos(rangle)+cross_mult_1.y*sin(rangle);
    u.z=u.z*cos(rangle)+cross_mult_1.z*sin(rangle);
    r.x=r.x*cos(rangle)+cross_mult_2.x*sin(rangle);
    r.y=r.y*cos(rangle)+cross_mult_2.y*sin(rangle);
    r.z=r.z*cos(rangle)+cross_mult_2.z*sin(rangle);
    Normalize(l);
    Normalize(u);
}
void clock_R(){
    double rangle;
    rangle=(angle*1.0*pi)/180;
    point cross_mult_1,cross_mult_2;
    cross_mult_1=CrossProduct(l,u);
    cross_mult_2=CrossProduct(l,r);
    u.x=u.x*cos(rangle)-cross_mult_1.x*sin(rangle);
    u.y=u.y*cos(rangle)-cross_mult_1.y*sin(rangle);
    u.z=u.z*cos(rangle)-cross_mult_1.z*sin(rangle);
    r.x=r.x*cos(rangle)-cross_mult_2.x*sin(rangle);
    r.y=r.y*cos(rangle)-cross_mult_2.y*sin(rangle);
    r.z=r.z*cos(rangle)-cross_mult_2.z*sin(rangle);
    Normalize(l);
    Normalize(u);
}
void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			rotate_left();
			break;
        case '2':
			rotate_right();
			break;
        case '3':
			look_up();
			break;
        case '4':
			look_down();
			break;
        case '5':
			clock();
			break;
        case '6':
			anti_clock();
			break;
        case '9':
            position.z=position.z+u.z*3.0;
			break;
        case '0':
            position.z=position.z-u.z*3.0;
			break;
         case 'w':
             angleto_cylinder+=2.0;
             //x_ax_cylinder=1;
             //y_ax_cylinder=0;
             if(angleto_cylinder>45.0){
                angleto_cylinder=45.0;
             }
             break;
        case 'q':
             angleto_cylinder-=2.0;
             //x_ax_cylinder=1;
             //y_ax_cylinder=0;
             if(angleto_cylinder<-45.0){
                angleto_cylinder=-45.0;
             }
             break;
         case 'e':
             partial_angletoRotate_cylinder+=2.0;
             //x_ax_cylinder=0;
             //y_ax_cylinder=1;
             if( partial_angletoRotate_cylinder>45.0){
                 partial_angletoRotate_cylinder=45.0;
             }
             break;
         case 'r':
             partial_angletoRotate_cylinder-=2.0;
             x_ax_cylinder=0;
             y_ax_cylinder=1;
             if( partial_angletoRotate_cylinder<=-45.0){
                 partial_angletoRotate_cylinder=-45.0;
             }
             break;
         case 'a':
             anglerotate_as+=2.0;
             y_as=1;
             if(anglerotate_as>45.0){
                anglerotate_as=45.0;
             }
             break;
         case 's':
             anglerotate_as-=2.0;
             y_as=1;
             if(anglerotate_as<=-45.0){
                anglerotate_as=-45.0;
             }
             break;
          case 'd':
             angletoRotate_cylinder_z+=2.0;
             if(angletoRotate_cylinder_z>45.0){
                angletoRotate_cylinder_z=45.0;
             }
             break;
           case 'f':
               printf("HI");
             angletoRotate_cylinder_z-=2.0;
             if(angletoRotate_cylinder_z<-45.0){
                angletoRotate_cylinder_z=-45.0;
             }
             break;



		default:
			break;
	}
}

void specialKeyListener(int key, int x,int y){
    float amount= 3.0f;
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			position.x=position.x-l.x*amount;
			position.y=position.y-l.y*amount;
			break;
		case GLUT_KEY_UP:		// up arrow key
			position.x=position.x+l.x*amount;
			position.y=position.y+l.y*amount;
			break;

		case GLUT_KEY_RIGHT:
			position.x=position.x+r.x*amount;
			position.y=position.y+r.y*amount;
			break;
		case GLUT_KEY_LEFT:
			position.x=position.x-r.x*amount;
			position.y=position.y-r.y*amount;
			break;

		case GLUT_KEY_PAGE_UP:
		    position.z=position.z+u.z*3.0;
			break;
		case GLUT_KEY_PAGE_DOWN:
		     position.z=position.z-u.z*3.0;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

void loadData(){


    ifstream scene("/Offline1/scene.txt");

    if (!scene.is_open())
    {
        cout << "Error while opening" << endl;
        exit(1);
    }
   /* string lines;
    int count_line = 0;
     while (getline(scene, lines))
    {
        stringstream s(lines);
        string tokens;
        while (s >> tokens)
        {
            separated_strings[count_line].push_back(tokens);
        }
        count_line++;
    }
    cout<<"total line"<<count_line<<"..."<<endl;
      for (int i = 0; i < 2; i++){
             if (i == 0){
                recursion_level=stod(separated_strings[i][0].c_str());
             }
             if(i==1){
                width_image=stod(separated_strings[i][0].c_str());
                height_image=stod(separated_strings[i][0].c_str());
             }

      }
      cout<<recursion_level<<"..."<<endl;
      cout<<width_image<<"..."<<height_image<<".."<<endl;*/
}

void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(	position.x, position.y, position.z,
                position.x+l.x, position.y+l.y,  position.z+l.z,
                u.x, u.y, u.z);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawAxes();
	drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);
    //draw_cylinder(25, 100);
    rotateFull();
    //rotate_as();
    /*drawSphere(30,50,50);
    drawHalfSphere(25,50,50);
    drawCylinder(25,50,99,25);
     drawCylinder(25,50,99,50);
    drawHorn(50,25,90,90);*/

    //drawCircle(30,24);

    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


void animate(){
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	loadData();
	drawgrid=0;
	drawaxes=1;

	position.x=100;
	position.y=100;
	position.z=0;
    u.x=0;
    u.y=0;
    u.z=1;
    r.x=-1/sqrt(2);
    r.y=1/sqrt(2);
    r.z=0;
    l.x=-1/sqrt(2);
    l.y=-1/sqrt(2);
    l.z=0;
	printf("init  e vector is %f %f %f",l.x,l.y,l.z);
	angle = 3.0f;
	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring
    //glutSpecialFunc(specialKeyListener);
	glutKeyboardFunc(keyboardListener);
	// printf("Main e vector is %d %d %d",l.x,l.y,l.z);
	glutSpecialFunc(specialKeyListener);
	// printf("Main e vector is %d %d %d",&l.x,&l.y,&l.z);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
