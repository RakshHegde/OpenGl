#include<GL/glut.h>

#include<stdlib.h>
#include<string.h>
#include<cstdio>

#define QUIT 0

struct bricks
{
	float x,y,w,h;
	bool alive;
} b[45];

struct ball
{
	float ballx,bally,ballwh,velx,vely;
	float red;
	float green;
	float blue;
	bool left,right,up,down;
} ball;

struct pd
{
	float myx;
	float myy;
	float width;
	float height;
	bool lft;
	bool rgt;
	float clr[3];
}pd;

int score,flag,winflag=0,win;
GLfloat pdcolor[3][3]={{1.0,0.5,0.3},{1,0.3,0.4}};

void specialUp(int,int,int);
void specialDown(int,int,int);
void idle(void);
void draw(void);
void main_menu(int);
void Mymouse(int,int,int,int);

bool collision(float Ax, float Ay, float Aw, float Ah, float Bx, float By, float Bw, float Bh)
{
	if ( Ay+Ah < By ) return false;
	else if ( Ay > By+Bh ) return false;
	else if ( Ax+Aw < Bx ) return false;
	else if ( Ax > Bx+Bw ) return false;

	return true;
}

void initialize(){
    for(int n=0,x=4,y=370;n<45;n++,x+=66)
	{
	   if(x>560)
	   {
	     x=4;
	     y-=25;
	   }
	   b[n].x=x;
	   b[n].y=y;
	   b[n].w=60;
	   b[n].h=20;
	   b[n].alive=true;
	}

	pd.myx=100;
	pd.myy=0;
	pd.width=100;
	pd.height=20;
	pd.lft=false;
	pd.rgt=false;

	ball.ballx=300;
	ball.bally=200;
	ball.ballwh=30;
	ball.velx=0.75;
	ball.vely=0.8;

	ball.red=0.65;
	ball.green=0.49;
	ball.blue=0.24;

	pd.clr[0]=0.1;
    pd.clr[1]=0.5;
    pd.clr[2]=0.8;


}
void subdisp()
{
    char str1[50]="GAME OVER",str2[50]="Score: ",str3[10]="NEW GAME",str4[6]="EXIT";
    int i;

    if(flag==0){
        char str3[50]="  YOU WON ";
        strcpy(str3,str1);
        flag=0;
    }

    glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0,0,0,0);

    glColor3f(1,1,1);
    glRasterPos2f(230,350);
    for(i=0;i<10;i++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str1[i]);
    //glFlush();
    glColor3f(0.6,0.4,1.0);
    glBegin(GL_POLYGON);
    glVertex2f(220,270);
    glVertex2f(380,270);
    glVertex2f(380,230);
    glVertex2f(220,230);
    glEnd();
    glColor3f(1,1,1);
     glRasterPos2f(250,240);
    sprintf(str2,"%s %d",str2,score);
    for(i=0;i<11;i++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str2[i]);


    glColor3f(0,0.9,0.1);
    glBegin(GL_POLYGON);
    glVertex2f(220,220);
    glVertex2f(380,220);
    glVertex2f(380,180);
    glVertex2f(220,180);
    glEnd();

    glColor3f(1,1,1);
    glRasterPos2f(235,190);
    for(i=0;i<10;i++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str3[i]);

    glColor3f(1,0.0,0);
    glBegin(GL_POLYGON);
    glVertex2f(220,170);
    glVertex2f(380,170);
    glVertex2f(380,130);
    glVertex2f(220,130);
    glEnd();

    glColor3f(1,1,1);
    glRasterPos2f(265,140);
    for(i=0;i<4;i++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str4[i]);
    glFlush();
    glutMouseFunc(Mymouse);
}

void myinit()
{
	glViewport(0,0,600,400);
	glLoadIdentity();
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0,600,0,400);
}

void idle()
{
	if(pd.myx<0)
	  pd.myx=0;
	if(pd.myx+pd.width>620)
	  pd.myx=520;
	if(pd.lft==true)
	  pd.myx=pd.myx-0.1;
	if(pd.rgt==true)
	  pd.myx=pd.myx+0.1;

	ball.ballx+=0.05*ball.velx;
	ball.bally+=0.05*ball.vely;

	for(int n=0;n<45;n++)
	{
	   if(b[n].alive==true)
	   {
	   	if(collision(ball.ballx,ball.bally,ball.ballwh,ball.ballwh,b[n].x,b[n].y,b[n].w,b[n].h)==true)
	   	{
	   	  ball.vely=-ball.vely;
	   	  b[n].alive=false;
	   	  score+=1;
	   	  ball.down=true;
	   	  ball.up=false;
	   	  break;
	   	}
	   }
	}
	if(ball.ballx<0)
	{
		ball.velx=-ball.velx;
		ball.right=true;
		ball.left=false;
	}
	if(ball.ballx+ball.ballwh>600)
	{
		ball.right=false;
		ball.left=true;
		ball.velx=-ball.velx;
	}
	if(ball.bally+ball.ballwh>400)
		ball.vely=-ball.vely;
	else if(ball.bally<0)
	{
        myinit();
        glutDisplayFunc(subdisp);//game over
        return;
	}
		if(collision(ball.ballx,ball.bally,ball.ballwh,ball.ballwh,pd.myx,pd.myy,pd.width,pd.height)==true)
	{
			ball.vely=-ball.vely;
			ball.up=true;
			ball.down=false;
	}
	draw();

	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0,0,0,1);
	glDisable(GL_DEPTH_TEST);
	draw();

	glFlush();
	idle();
}

void specialUp(int key,int x,int y)
{
	switch(key)
	{
		case (GLUT_KEY_LEFT): pd.lft=false;break;
		case (GLUT_KEY_RIGHT): pd.rgt=false;break;
	}
}

void specialDown(int key,int x,int y)
{
	switch(key)
	{
		case (GLUT_KEY_LEFT): pd.lft=true;break;
		case (GLUT_KEY_RIGHT): pd.rgt=true;break;
	}
}

void keyboard(unsigned char key,int x,int y){
if(key==27) 		//27 corresponds to the esc key
	{
		ball.velx=0;
		ball.vely=0;
		}
}

void draw()
{
	for(int i=0;i<45;i++)
	{
		if(b[i].alive==true)
		{
		   if(i%2==0) glColor3fv(pdcolor[0]);
		   else glColor3fv(pdcolor[1]);
		   glBegin(GL_POLYGON);
			   glVertex2f(b[i].x,b[i].y);
			   glVertex2f(b[i].x+b[i].w,b[i].y);
			   glVertex2f(b[i].x+b[i].w,b[i].y+b[i].h);
			   glVertex2f(b[i].x,b[i].y+b[i].h);
		   glEnd();
		   winflag=1;
		}
    }
    if(winflag==0){
        glutDisplayFunc(subdisp);//game over
    }

	glColor3f(pd.clr[0],pd.clr[1],pd.clr[2]);
	glBegin(GL_POLYGON);
		glVertex2f(pd.myx,pd.myy);
		glVertex2f(pd.myx+pd.width,pd.myy);
		glVertex2f(pd.myx+pd.width,pd.myy+pd.height);
		glVertex2f(pd.myx,pd.myy+pd.height);
	glEnd();

	glColor3f(0.85885,0.8,0.449216);
	glBegin(GL_POLYGON);
		glVertex2f(ball.ballx,ball.bally);
		glVertex2f(ball.ballx+ball.ballwh,ball.bally);
		glVertex2f(ball.ballx+ball.ballwh,ball.bally+ball.ballwh);
		glVertex2f(ball.ballx,ball.bally+ball.ballwh);
	glEnd();
	glutPostRedisplay();
}

void main_menu(int opt)
{
	switch(opt)
	{

		case QUIT: exit(0);
	}
}

void revert()
{
	ball.velx=0.35;
	ball.vely=0.35;
	if(ball.up==true)
	{
		if(ball.right==true)
		{
			ball.velx=ball.velx;
			ball.vely=ball.vely;
		}
		else if(ball.left==true)
		{
			ball.velx=-ball.velx;
			ball.vely=ball.vely;
		}
	}
	else if(ball.down==true)
	{
		if(ball.right==true)
		{
			ball.velx=ball.velx;
			ball.vely=-ball.vely;
		}
		else if(ball.left==true)
		{
			ball.velx=-ball.velx;
			ball.vely=-ball.vely;
		}
	}
}

void mymain(){

	glutCreateMenu(main_menu);
	glutAddMenuEntry("Quit Game",QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	initialize();
	myinit();

	draw();
	glutDisplayFunc(display);
      glutSpecialFunc(specialDown);
    	glutSpecialUpFunc(specialUp);
    	glutKeyboardFunc(keyboard);
}

void Mymouse(int btn,int st,int mx,int my){
     int j=0;

    my=400-my;
    if(mx>=220&&mx<=380){
        if(my<=220&&my>=180)//new game
                j=1;
        else if(my>=130&&my<=170)//exit
                j=2;
    }
      if(st==GLUT_DOWN&&j==1)
      {
            j=0;
            mymain();
            score=0;
            flag=1;
      }
       if(st==GLUT_DOWN&&j==2) {
            exit(0);
      }
}

int main(int argc,char *argv[])
{
	glutInit(&argc,argv);
    flag=0;
    glutInitWindowSize(600,400);
	glutInitWindowPosition(500,100);
    win=glutCreateWindow("Brick Breaker");

    mymain();

	glutMainLoop();
	return 0;
}


