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
#include<iostream>
#include <stdlib.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include <cstdlib>
#include <ctime>
#define pi (2*acos(0.0))

double cameraHeight;
double cameraAngle;
int countp=0;
int drawgrid;
int drawaxes;
double angle;
int insidecircle[5];
int collision_bubbles[5];
int time_bubble[5];
float in=1.00;

struct point
{
    double x,y,z;
};

struct position
{
    double x,y;

};
struct position pos[5];
struct position temp;
struct position  sp[5];
float dx[5];
float dy[5];
double amount_x;
double amount_y;
clock_t time_req;

void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
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
void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0)
    glBegin(GL_QUADS);
    {

        glVertex3f( a, a,2);
        glVertex3f( a,-a,2);
        glVertex3f(-a,-a,2);
        glVertex3f(-a, a,2);
        //glColor3f(34.0,139.0,34.0);
    }
    glEnd();
}


void drawCircle(double radius,int segments,int r)
{
    int i;
    struct point points[100];
   // glColor3f(0,0,0);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0; i<segments; i++)
    {
        if(r==0){
        glColor3f(255.0,0,0);
        }
        else{
            glColor3f(0.9,0.6,0.5);
        }

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
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0; i<segments; i++)
    {
        //create shading effect
        if(i<segments/2)
            shade=2*(double)i/(double)segments;
        else
            shade=2*(1.0-(double)i/(double)segments);
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
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }
}


void drawSS()
{

    drawLine(140);
    drawCircle(80,60,0);
    glTranslatef(-130,-130,0);
    glPushMatrix();
    {
        glTranslatef(pos[0].x,pos[0].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(pos[1].x,pos[1].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(pos[2].x,pos[2].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(pos[3].x,pos[3].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();
    glPushMatrix();
    {
        glTranslatef(pos[4].x,pos[4].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();

}

void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case 'p':
        countp++;
        for(int i=0;i<5;i++){
        if(countp%2==0){
            sp[i].x=dx[i];
            sp[i].y=dy[i];
        }
        else{
            dx[i]=sp[i].x;
            dy[i]=sp[i].y;
            sp[i].x=0;
            sp[i].y=0;
        }
         }
        break;

    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:
             in=in-0.5;//down arrow key
    /* for(int i=0;i<5;i++){
           sp[i]
        }*/

        break;
    case GLUT_KEY_UP:
        in=in+0.5;	// up arrow key
       /*for(int i=0;i<5;i++){
              pos[i].y+=sp[i].y*in;
        }*/
       // pos[i].y+=sp[i].y*in;
        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;
        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;
        break;

    case GLUT_KEY_PAGE_UP:
        break;
    case GLUT_KEY_PAGE_DOWN:
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


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
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


void init_bubbles(){
    drawLine(140);
    drawCircle(80,60,0);
    glTranslatef(-130,-130,0);
    time_bubble[0]=1;
    glPushMatrix();
    {
        glTranslatef(pos[0].x,pos[0].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();

    float f=(float((clock()-time_req)))/CLOCKS_PER_SEC;
    //printf("time in init %f",f);
    if(f>1.00){
         time_bubble[1]=1;
         glPushMatrix();
    {
        glTranslatef(pos[1].x,pos[1].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();
    }
    f=(float((clock()-time_req)))/CLOCKS_PER_SEC;
    if(f>2.00){
         time_bubble[2]=1;
         glPushMatrix();
    {
        glTranslatef(pos[2].x,pos[2].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();
    }
    f=(float((clock()-time_req)))/CLOCKS_PER_SEC;
   if(f>3.00){
        time_bubble[3]=1;
         glPushMatrix();
    {
        glTranslatef(pos[3].x,pos[3].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();
    }
    f=(float((clock()-time_req)))/CLOCKS_PER_SEC;
    if(f>4.00){
        time_bubble[4]=1;
         glPushMatrix();
    {
        glTranslatef(pos[4].x,pos[4].y,0);
        drawCircle(10,60,1);
    }
    glPopMatrix();
    }



}
void display()
{

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0,0,200,	0,0,0,	0,1,0);
    glMatrixMode(GL_MODELVIEW);

    if((float((clock()-time_req)))/CLOCKS_PER_SEC<5.00){
        init_bubbles();
    }
    if((float((clock()-time_req)))/CLOCKS_PER_SEC>=5.00){
    drawSS();
    }
    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}
void random_speed()
{
    srand((unsigned int)time(NULL));
    for(int i=0; i<5; i++)
    {
        float rx=float(rand())/float(RAND_MAX/(90.0));
        float ry=float(rand())/float(RAND_MAX/(90.0));
        sp[i].x=cos(rx)*0.02;
        sp[i].y=sin(rx)*0.02;
    }

}

float dot_product(struct position p1,struct position p2){
    float temp;
    temp=p1.x*p2.x+p1.y*p2.y;
    return temp;
}
struct position Normalize(struct position vector1){
    double mag=sqrt(vector1.x*vector1.x+vector1.y*vector1.y);
    vector1.x=vector1.x/mag;
    vector1.y=vector1.y/mag;
    return vector1;

}

void animate()
{
    //printf("time %f",(float)time_req/CLOCKS_PER_SEC);
    for(int i=0; i<5; i++){
    if(time_bubble[i]==1){
    double new_pos,new_pos_y;
    new_pos=pos[i].x+sp[i].x;
    new_pos_y=pos[i].y+sp[i].y;
    double check_radius,check_radius_bubble;

    check_radius=sqrt((((-130+new_pos)-0)*((-130+new_pos)-0))+(((-130+new_pos_y)-0)*((-130+new_pos_y)-0)));
    if(check_radius<=(80.0-10.0)){
        insidecircle[i]=1;
    }
    if(insidecircle[i]==1){
      check_radius=sqrt((((-130+new_pos)-0)*((-130+new_pos)-0))+(((-130+new_pos_y)-0)*((-130+new_pos_y)-0)));
       if(check_radius>=70.0){
        struct position temp1;
        temp1.x=0-(-130+new_pos);
        temp1.y=0-(-130+new_pos_y);
        temp1=Normalize(temp1);
        float dot=dot_product(sp[i],temp1);
        if(dot<0)
       {
           sp[i].x=sp[i].x-2*dot*temp1.x;
           sp[i].y=sp[i].y-2*dot*temp1.y;
       }

       }
       for(int j=0;j<5;j++){
            if(j!=i && insidecircle[j]==1){
            check_radius_bubble=sqrt((((-130+new_pos)-(-130+pos[j].x))*((-130+new_pos)-(-130+pos[j].x)))+(((-130+new_pos_y)-(-130+pos[j].y))*((-130+new_pos_y)-(-130+pos[j].y))));
            if(check_radius_bubble<=20){
                    struct position temp2;
                    temp2.x=(-130+new_pos)-(-130+pos[j].x);
                    temp2.y=(-130+new_pos_y)-(-130+pos[j].y);
                    temp2=Normalize(temp2);
                    float dot2=dot_product(sp[i],temp2);
                    if(dot2<0){
                    sp[i].x=sp[i].x-2*dot2*temp2.x;
                    sp[i].y=sp[i].y-2*dot2*temp2.y;
                    float dot3=dot_product(sp[j],temp2);
                    sp[j].x=sp[j].x-2*dot3*temp2.x;
                    sp[j].y=sp[j].y-2*dot3*temp2.y;}

            }
            }

    }


    }
    if(-130+new_pos>130.0 || -130+new_pos<-130.0){
            sp[i].x=-1.0*sp[i].x;
    }
    pos[i].x+=sp[i].x*in;
    if(-130+new_pos_y>130.0 || -130+new_pos_y<-130.0){
            sp[i].y=-1.0*sp[i].y;

    }
        pos[i].y+=sp[i].y*in;

    }

    }
    glutPostRedisplay();

}

void init()
{ //printf("hi");
    //codes for initialization
    time_req = clock();
    random_speed();
    drawgrid=0;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;
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

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
