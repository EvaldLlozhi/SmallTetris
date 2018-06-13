//#include<windows.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <string.h>

using namespace std;

#define ROCK 1
#define PAPER 2
#define SCIS 3
#define BOMB 4

#define ROWS 13
#define COLUMNS 6

#define SIZE 1600
#define WIDTH 40
#define HEIGHT 40

typedef struct s{
  int x,y;
  struct s *next;
}SQUARE;



int scene[ROWS][COLUMNS];
int x=-10,y=-10,shape=0;
int score=0;
int start=0,flag=0;

unsigned char scis[SIZE],rock[SIZE],paper[SIZE],bom[SIZE];

int random(int a,int b)
{
  //return(1); //testing game over
  return(rand()%(b-a+1) + a);
}

void moveright(){
  if(x<COLUMNS-1 && scene[y][x+1]==0)
    x++;
}

void moveleft(){
   if(x>0 && scene[y][x-1]==0)
    x--;
}

void readpgms(){
  int i,j,temp;
  char buf[SIZE];
  
  FILE *infile;
  
  infile=fopen("paper.pgm","r");
  if(infile==NULL){
    printf("Error opening file...\n");
    exit(0);
  }
  
  for(i=0;i<4;i++)
    fgets(buf,SIZE,infile);
  
  for(i=0;i<SIZE;i++){
    fscanf(infile,"%d",&temp);
    paper[i]=(unsigned char)temp;
    
  }
  
  fclose(infile);
  
    infile=fopen("rock.pgm","r");
  if(infile==NULL){
    printf("Error opening file...\n");
    exit(0);
  }
  
  for(i=0;i<4;i++)
    fgets(buf,SIZE,infile);
  
  for(i=0;i<SIZE;i++){
    fscanf(infile,"%d",&temp);
    rock[i]=(unsigned char)temp;
    
  }
  
  fclose(infile);
  
    infile=fopen("scissors.pgm","r");
  if(infile==NULL){
    printf("Error opening file...\n");
    exit(0);
  }
  
  for(i=0;i<4;i++)
    fgets(buf,SIZE,infile);
  
  for(i=0;i<SIZE;i++){
    fscanf(infile,"%d",&temp);
    scis[i]=(unsigned char)temp;
    
  }
  for(i=0;i<SIZE;i++){
    bom[i]=255;
    
  }
  
  fclose(infile);
  
}
void initRendering() {
	glEnable(GL_DEPTH_TEST);
}
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0,
				   (double)w / (double)h,
				   1.0,
				   200.0);
}

void newsquare(){
  
  y=12;
  x=random(0,COLUMNS-1);
  shape=random(1,4);
  
}
void gameover(){
  int i;
  
  for(i=0;i<COLUMNS;i++)
    if(scene[ROWS-1][i]!=0)
      start=0;
    
}
int wins(int s1,int s2){
  
  if(s1==ROCK && s2==SCIS)
    return(1);
   if(s1==PAPER && s2==ROCK)
    return(1);
  if(s1==SCIS && s2==PAPER)
    return(1);   

  return(0);
}


void eat(){
  SQUARE *temp=NULL,*last,*S=NULL;
  int count=0,newx=-1,newy=ROWS,oldx=x,oldy=y;
  
     temp=(SQUARE *)malloc(sizeof(SQUARE));
    if(temp==NULL)
      exit(0);
    
    temp->next=NULL;
   temp->x=x;
    temp->y=y;
    
    if(S==NULL){
      S=temp;
    last=temp;
    }
    
    
  while(S!=NULL){
    x=S->x;
    y=S->y;
    if(S->x+1<COLUMNS && wins(shape,scene[y][x+1])){//right
      scene[y][x+1]=0;
      temp=(SQUARE *)malloc(sizeof(SQUARE));
      if(temp==NULL)
	exit(0);
      
      
      temp->next=NULL;
      temp->x=x+1;
      temp->y=y;
      last->next=temp;
      last=temp;
      count++;
      if(newx<x+1){
	newx=x+1;
	newy=y;
      }
    }
    
    if(S->x-1>=0 && wins(shape,scene[y][x-1])){//left
      scene[y][x-1]=0;
      temp=(SQUARE *)malloc(sizeof(SQUARE));
      if(temp==NULL)
	exit(0);
      
      temp->next=NULL;
      temp->x=x-1;
      temp->y=y;
      last->next=temp;
      last=temp;
      count++;
      if(newx<x-1){
	newx=x-1;
      	newy=y;
      }
    }

    if(S->y+1<ROWS && wins(shape,scene[y+1][x])){//up
      scene[y+1][x]=0;
      temp=(SQUARE *)malloc(sizeof(SQUARE));
      if(temp==NULL)
	exit(0);
      
      temp->next=NULL;
      temp->x=x;
      temp->y=y+1;
      last->next=temp;
      last=temp;
      count++;
      if(newy>y+1 && newx<x){
	newy=y+1;
      	newx=x;
      }
    }
    
    if(S->y-1>=0 && wins(shape,scene[y-1][x])){//down
      scene[y-1][x]=0;
      temp=(SQUARE *)malloc(sizeof(SQUARE));
      if(temp==NULL)
	exit(0);
      
      temp->next=NULL;
      temp->x=x;
      temp->y=y-1;
      last->next=temp;
      last=temp;
      count++;
      if(newy>y-1 && newx<x){
	newy=y-1;
	newx=x;
      }
    }
    temp=S;   
    S=S->next;
    free(temp);
    
  }//while(S!=NULL);
  
  if(count!=0){
    scene[oldy][oldx]=0;
    scene[newy][newx]=shape;
  } 
  
  score+=10*count + count*count;

}

void drop(){
  int i,j,k;
  for(j=0;j<COLUMNS;j++){
    
    for(i=0;i<ROWS;i++){
      if(scene[i][j]!=0)
	continue;
      
      k=i+1;
      while(k<ROWS && scene[k][j]==0)
	k++;
      if(k!=ROWS){
	scene[i][j]=scene[k][j];
	scene[k][j]=0;
      }
    }
  }
}
int canmove(){

  if(y>0 && scene[y-1][x]==0)
    return(1);
  return(0);
}
void bomb()
{
  int count=0;
  scene[y][x]=0;
  if(y-1>=0 && scene[y-1][x]!=0){
    count++;
    scene[y-1][x]=0;
  }
  if(y+1<ROWS && scene[y+1][x]!=0){
    count++;
    scene[y+1][x]=0;
  }
  if(x-1>=0 && scene[y][x-1]!=0){
    count++;
    scene[y][x-1]=0;
  }
  if(x+1<ROWS && scene[y][x+1]!=0){
    count++;
    scene[y][x+1]=0;
  }
  score+=10*count + count*count;
}

void drawsquare(int x,int y,int shape)
{
  GLenum type,format;
  
  format = GL_LUMINANCE;
  type = GL_UNSIGNED_BYTE;
  glRasterPos2i(x*WIDTH,(y+1)*HEIGHT);
  if(shape==PAPER)
    glDrawPixels((GLsizei)WIDTH, (GLsizei)HEIGHT, format, type, paper);
  else if(shape==ROCK)
    glDrawPixels((GLsizei)WIDTH, (GLsizei)HEIGHT, format, type, rock);
  else if(shape==SCIS)
    glDrawPixels((GLsizei)WIDTH, (GLsizei)HEIGHT, format, type, scis);
  else
    glDrawPixels((GLsizei)WIDTH, (GLsizei)HEIGHT, format, type, bom);
      
}
void drawScene() {
  
	int i,j;
	char scorestr[150];
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix ();
	glLoadIdentity ();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix ();
	glLoadIdentity();
	//glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 240.0, -30, 520.0);	
	glColor3f(1,1,1);
	glBegin(GL_LINES);
	glVertex2i(0,0);
	glVertex2i(240,0);

	glEnd();
	

	 glPixelZoom(1.0f, -1.0f);
	
	for(i=0;i<ROWS;i++)
	  for(j=0;j<COLUMNS;j++)
	    if(scene[i][j]!=0){
	      drawsquare(j,i,scene[i][j]);
	    }

	drawsquare(x,y,shape);
	glPixelZoom(1.0f, 1.0f);           // swap the y-direction of image to top-to-bottom
    

    if(start==0 && flag==1){
      
    sprintf(scorestr,"SCORE: %d GAME OVER. press b ",score);
    
    glRasterPos2i(10,-10);

    for(i=0;i<strlen(scorestr);i++){
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, scorestr[i]);
    }
    
      sprintf(scorestr,"to restart or esc to exit",score);
      
      glRasterPos2i(10,-20);

      for(i=0;i<strlen(scorestr);i++){
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, scorestr[i]);
      }
    }
    else
    {
       sprintf(scorestr,"SCORE: %d",score);
      
      glRasterPos2i(20,-20);

      for(i=0;i<strlen(scorestr);i++){
	glutBitmapCharacter(GLUT_BITMAP_8_BY_13, scorestr[i]);
      }
    }
    
	glutSwapBuffers(); //Send the 3D scene to the screen
}

void update(int value){
  
    if(value==1)
    {
      
      if(canmove())
	y--;
      else
      {
	scene[y][x]=shape;

	if(shape==BOMB)
	  bomb();
	else
	  eat();
	drop();
	gameover();
	newsquare();
      }
      
      glutPostRedisplay(); // Inform GLUT that the display has changed
      glutTimerFunc(250,update,1);//Call update after each 25 millisecond
    }
}

void handleKeypress(unsigned char key, int x, int y) {
  int i,j;
	switch (key) {
		case 27:
			printf("key b pressed\n");
			exit(0);
		case 'b':
		case 'B':
		   printf("key b pressed\n");
		   if(start==0){
		    flag=1;
		    newsquare();
		    start=1;
		    for(i=0;i<ROWS;i++)
		      for(j=0;j<COLUMNS;j++)
			scene[i][j]=0;
		    glutTimerFunc(250,update,start);//Call update after each 25 millisecond
		   }
		   break;
		case 's':
		case 'S':
		   moveleft();
		   break;
		case 'd':
		case 'D':
		   moveright();
		   break;
		  
	}
}

int main(int argc, char** argv) {
  
	int i,j;
	

	readpgms();
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(240, 550);
	glutInitWindowPosition (100,100);	
	glutCreateWindow("1h Askhsh");
	initRendering();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutTimerFunc(250,update,start);
	glutMainLoop();
	return 0;
}
