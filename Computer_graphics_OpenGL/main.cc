/*
	Final project CG Dennis Buurman s2027100

	Goal: Squidward fireworks show.

	https://stackoverflow.com/questions/686353/random-float-number-generation
	https://stackoverflow.com/questions/36538563/opengl-drawing-half-circle
*/

#include <stdio.h>
#include <stdlib.h>
#include "GL/glut.h"
#include "SOUND/include/irrKlang.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#define MAXBUBBLES 500
#define MAXPARTICLES 150
#define MAXARROWS 20
#define TRAILSPORES 10
#define TRAIL 8
#define ALPHA_DECREASE 0.015
#define BUBBLE_ALPHA_DEC 0.003

// using namespace irrklang;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

/*************GLOBAL VARIABLES*************/
GLfloat angle = 0.0;

irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

float g_posX = 0.0, g_posY = 50.0, g_posZ = 80.0;
float g_orientation = 0.0; // y axis

float n1 = 0.0, n2 = 0.0, n3 = 0.0;

int bubbletime = 15000;

int trailclock = 50;
int trailtime = 0;

int randclock = 15;
int randtime = 0;

int burstdelay = 250;
int bursttime = 0;

int arrow_count = 0;

int theta = 0; // rotation angle

bool scenery = true;
bool blockmode = false;
bool burstfire = false;

//diffuse light color variables
GLfloat dlr = 1.0;
GLfloat dlg = 1.0;
GLfloat dlb = 1.0;

//ambient light color variables
GLfloat alr = 1.0;
GLfloat alg = 1.0;
GLfloat alb = 1.0;

//light position variables
GLfloat lx = 0.0;
GLfloat ly = 1.0;
GLfloat lz = 1.0;
GLfloat lw = 0.0;

//interface lighting flags
int aflag = 1;
int dflag = 1;
int mflag = 1;

//commonly used material values
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat default_ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat default_diffuse[] = {0.8, 0.8, 0.8, 1.0};
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat no_shininess[] = { 0.0 };
GLfloat low_shininess[] = { 5.0 };
GLfloat high_shininess[] = { 100.0 };
GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};

struct binfo
{
	float x, y, z;
	float vx, vy, vz;
	float scale;
	float alpha = 0.6;

	int time_alive = bubbletime;

} bubbles[MAXBUBBLES];

struct ainfo
{
	float x, y, z;
	float vx, vy, vz;
	float r, g, b;

	float alpha;

	int swap;

	bool splode, set, start = false;

	void explosion();
	void drawExplosion();
	void drawExplosionParticle();
	void setTrail();
	void drawTrail();
	void drawTrailParticle();

	struct tinfo
	{
		float x, y, z;
		float vx, vy, vz;
	} trail_part[TRAILSPORES*TRAIL];

	struct einfo
	{
		float x, y, z;
		float vx, vy, vz;
		float r, g, b;
		float active;
	} blast_particles[MAXPARTICLES];	

} arrows[MAXARROWS];

/*************DRAWING FUNCTIONS*************/
void rainbowColor3f()
{
	int max = 10, min = 0;

	srand(time(NULL));
	float r = ((float)(rand()%(max-min + 1) + min))/10.0;
	float g = ((float)(rand()%(max-min + 1) + min))/10.0;
	float b = ((float)(rand()%(max-min + 1) + min))/10.0;

	glColor3f(r, g, b);
}

void quadNormal(float xa, float ya, float za, float xb, float yb, float zb, float xc, float yc, float zc, float xd, float yd, float zd, float &n1, float &n2, float &n3)
{
	float V1[3] = {0.0, 0.0, 0.0};
	float V2[3] = {0.0, 0.0, 0.0};

	// v1 = (c - a)
	V1[0] = xc - xa;
	V1[1] = yc - ya;
	V1[2] = zc - za;
	// v2 = (d - b)
	V2[0] = xd - xb;
	V2[1] = yd - yb;
	V2[2] = zd - zb;

	n1 = V1[1] * V2[2] - V1[2] * V2[1];
	n2 = V2[0] * V1[2] - V2[2] * V1[0];
	n3 = V1[0] * V2[1] - V1[1] * V2[0];
}

// x,y,z = lower left corner
void drawSquare(float w, float h, float d, float x, float y, float z, bool side, bool top)
{
	// FIXME
	if(top) // b c d a
		quadNormal(x, y+h, z+d, x+w, y+h, z+d, x+w, y, z, x, y, z, n1, n2, n3);
	else if(!side) //c, b, a, d
		quadNormal(x+w, y+h, z+d, x, y+h, z+d, x, y, z, x+w, y, z, n1, n2, n3);
	else // b c d a
		quadNormal(x, y+h, z, x+w, y+h, z+d, x, y, z+d, x, y, z, n1, n2, n3);

	glBegin(GL_QUADS);
		glNormal3f(n1, n2, n3);
		glVertex3f(x, y, z); // a
		if(!side)
			glVertex3f(x, y+h, z+d); // b
		else
			glVertex3f(x, y+h, z); // b side
		glVertex3f(x+w, y+h, z+d); // c
		if(!side)
			glVertex3f(x+w, y, z); // d
		else
			glVertex3f(x, y, z+d); // d side
	glEnd();
}

// x,y,z = lower left corner
void drawBox(float w, float h, float d, float x, float y, float z)
{
	drawSquare(w, h, 0, x, y, z+d, 0, 0); //front
	drawSquare(w, h, 0, x, y, z, 0, 0); // back
	drawSquare(0, h, -1*d, x, y, z+d, 1, 0); // left
	drawSquare(0, h, d, x+w, y, z, 1, 0); // right
	drawSquare(w, 0, d, x, y+h, z, 0, 1); // top
	drawSquare(w, 0, d, x, y, z, 0, 1); // bottom
}

void drawHalfCircle(float radius)
{
	float twoPI = 2 * M_PI;

	glBegin(GL_TRIANGLE_FAN);
		for (float i = M_PI; i <= twoPI; i += 0.001)
			glVertex2f((sin(i)*radius), (cos(i)*radius));
	glEnd();
	glFlush();
}

void drawPlatform()
{
	glColor3f(0.4, 0.3, 0.4);
	drawBox(100, 10, 100, -50, -10, -50);
	rainbowColor3f();
	drawBox(100, 5, 100, -50, -15, -50);

	rainbowColor3f();
	drawBox(10, 20, 10, -50, 0, 40); // lower left corner
	drawBox(10, 20, 10, -50, 0, -50); // top left corner
	drawBox(10, 20, 10, 40, 0, 40); // lower right corner
	drawBox(10, 20, 10, 40, 0, -50); // upper right corner
}

void drawCannon()
{
	glColor3f(0.8, 0.8, 0.8);
	drawBox(16, 5, 16, -8, 0, -8);
	drawBox(8, 10, 8, -4, 5, -4);
}

void drawStreet()
{
		float w = 1500, h = 1, d = 500;
		float rand_spot = 1.0;
		float min = 10.0, max = 30.0;

		glColor3f(0.83, 0.78, 0.64);
		drawBox(w, h, d, -1*(w/2), -h, -1*(d/2));

		glColor3f(0.37, 0.37, 0.37);

		drawBox(10, 0.1, 3.0, -5, 0, 20);
		drawBox(10, 0.1, 3.0, -5, 0, 25);
		drawBox(10, 0.1, 3.0, -5, 0, 30);
		drawBox(10, 0.1, 3.0, -5, 0, 35);
		drawBox(10, 0.1, 3.0, -5, 0, 40);

		drawBox(w, 0.003, 20, -1*(w/2), 0, 45);

		glPushMatrix();
			glTranslatef(-70, 0.0, 15.0);
			drawBox(10, 0.003, 30, 0, 0, 0);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(60, 0.0, 15.0);
			drawBox(10, 0.003, 30, 0, 0, 0);

		glPopMatrix();

		// glColor3f(0.63, 0.58, 0.44);
		glColor3f(0.0, 0.0, 0.0);
		for(float i = -1*(d/2); i < d/2; i += 10)
		{
			for(float j = -1*(w/2); j < w/2; j += 8)
			{
				glPushMatrix();
					glTranslatef(j, 0.001, i);
					drawBox(0.3, 0.001, 0.3, 0, 0, 0);
				glPopMatrix();
				rand_spot = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max-min)));
			}
		}
}

void drawPatrickHouse()
{
	glColor3f(0.65, 0.165, 0.165);
	glutSolidSphere(20, 32, 32);
	glColor3f(1.0, 1.0, 0.4);
	drawBox(0.6, 5, 0.6, 0, 20, 0);
	drawBox(12, 0.6, 0.6, -4, 25, 0.3);
	
	glPushMatrix();
		glTranslatef(6.4, 0, 0);
		glPushMatrix();
			glRotatef(45, 0.0, 0.0, 1.0);
			glTranslatef(10.0, 25, 0.3);
			drawBox(4, 0.6, 0.6, 0.0, 0.0, 0.0);
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-14.9, 0, 0);
		glPushMatrix();
			glRotatef(-45, 0.0, 0.0, 1.0);
			glTranslatef(-10.0, 25, 0.3);
			drawBox(4, 0.6, 0.6, 0.0, 0.0, 0.0);
		glPopMatrix();
	glPopMatrix();

	drawBox(0.3, 5, 0.3, 4.0, 22.5, 0.3);
	drawBox(0.3, 5, 0.3, 6.0, 22.5, 0.3);
}

void drawSquidwardHouse()
{
	float height = 50.0, r_roof = 10.0, r_base = 16.0;
	float r_window = (r_base-r_roof)/2;

	glPushMatrix();
		glColor3f(0.12, 0.25, 0.56);
		glRotatef(-90, 1.0, 0.0, 0.0);
		GLUquadricObj *walls;
		walls = gluNewQuadric();
		gluCylinder(walls, r_base, r_roof, height, 32, 32);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.12, 0.25, 0.56);
		glTranslatef(0, height, 0);
		glRotatef(-90, 1.0, 0.0, 0.0);
		GLUquadricObj *roof;
		roof = gluNewQuadric();
		gluDisk(roof, 0.0, r_roof, 32, 32);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.12, 0.25, 0.66);
		glTranslatef(0, height-(height/4), 2);
		glRotatef(-90, 1.0, 0.0, 0.0);
		glutSolidTorus(2.0, 9.0, 32, 32);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.46, 0.46, 0.46);
		glTranslatef((r_base - r_roof)-0.5, height-(height/4)-r_window-1, 12);
		GLUquadricObj *windowr;
		windowr = gluNewQuadric();
		gluDisk(windowr, 0.0, r_window, 32, 32);
	glPopMatrix();

	glPushMatrix();
		glColor4f(0.0, 0.0, 1.0, 0.5);
		glTranslatef((r_base - r_roof)-0.5, height-(height/4)-r_window-1, 12.001);
		GLUquadricObj *glassr;
		glassr = gluNewQuadric();
		gluDisk(glassr, 0.0, r_window-(r_window/2.5), 32, 32);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.46, 0.46, 0.46);
		glTranslatef(-1*(r_base - r_roof)+0.5, height-(height/4)-r_window-1, 12);
		GLUquadricObj *windowl;
		windowl = gluNewQuadric();
		gluDisk(windowl, 0.0, r_window, 32, 32);
	glPopMatrix();

	glPushMatrix();
		glColor4f(0.0, 0.0, 1.0, 0.5);
		glTranslatef(-1*(r_base - r_roof)+0.5, height-(height/4)-r_window-1, 12.001);
		GLUquadricObj *glassl;
		glassl = gluNewQuadric();
		gluDisk(glassl, 0.0, r_window-(r_window/2.5), 32, 32);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.12, 0.25, 0.56);
		glTranslatef(-r_base, height/2.5, 0.0);
		glRotatef(-8, 0.0, 0.0, 1.0);
		drawBox(7, 17, 7, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.12, 0.25, 0.56);
		glTranslatef(r_base - 7, height/2.5, 0.0);
		glRotatef(8, 0.0, 0.0, 1.0);
		drawBox(7, 17, 7, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.12, 0.25, 0.56);
		glTranslatef(r_base-r_roof-8.5, height/2.5-2, 10.0);
		glRotatef(-8, 1.0, 0.0, 0.0);
		drawBox(5, 17, 5, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.71, 0.52, 0.39);
		glTranslatef(0, (height/4)-13, r_base+0.12);
		glRotatef(-90, 0.0, 0.0, 1.0);
		glRotatef(-6, 0.0, 1.0, 0.0);
		glScalef(1.4, 0.4, 1.0);
		drawHalfCircle(11);
	glPopMatrix();	
}

void drawSpongebobHouse()
{
	glColor3f(1.0, 0.35, 0.0);
	glScalef(1.0, 1.5, 1.0);
	glutSolidSphere(17, 32, 32);

	glPushMatrix();
		glColor3f(0.50, 0.50, 0.50);
		glTranslatef(0, -3.5, 14.0);
		glScalef(0.6, 1.0, 0.4);
		glRotatef(16, 1.0, 0.0, 0.0);
		glutSolidSphere(8, 32, 32);
	glPopMatrix();	

	glPushMatrix();
		glColor3f(0.50, 0.50, 0.50);
		glTranslatef(8, 10, 7.0);
		drawBox(9,1.5,1.5, 0,0,0);
		glTranslatef(9, 0.0, 0.0);
		drawBox(1.5,6,1.5, 0,0,0);
		glTranslatef(-0.625, 6, -0.625);
		drawBox(2.5, 1, 2.5, 0, 0, 0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.50, 0.50, 0.50);
		glTranslatef(-5.0, 5.0, 14.0);
		glScalef(1.0, 1.0, 0.6);
		glutSolidSphere(3, 32, 32);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.50, 0.50, 0.50);
		glTranslatef(7.0, 1, 14.0);
		glScalef(1.0, 1.0, 0.6);
		glutSolidSphere(3, 32, 32);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.1, 0.6, 0.2);
		glTranslatef(0.0, 20, 0.0);
		glScalef(0.3, 1.2, 0.5);
		glutSolidSphere(9, 32, 32);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(6, 2, 0);
		glPushMatrix();
			glRotatef(30, 0.0, 0.0, 1.0);
			glPushMatrix();
				glColor3f(0.1, 0.6, 0.2);
				glTranslatef(0.0, 20, 0.0);
				glScalef(0.3, 1.2, 0.5);
				glutSolidSphere(9, 32, 32);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(7, 3, 0);
		glPushMatrix();
			glRotatef(45, 0.0, 0.0, 1.0);
			glPushMatrix();
				glColor3f(0.1, 0.6, 0.2);
				glTranslatef(0.0, 20, 0.0);
				glScalef(0.3, 1.2, 0.5);
				glutSolidSphere(9, 32, 32);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-6, 2, 0);
		glPushMatrix();
			glRotatef(-30, 0.0, 0.0, 1.0);
			glPushMatrix();
				glColor3f(0.1, 0.6, 0.2);
				glTranslatef(0.0, 20, 0.0);
				glScalef(0.3, 1.2, 0.5);
				glutSolidSphere(9, 32, 32);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-7, 3, 0);
		glPushMatrix();
			glRotatef(-45, 0.0, 0.0, 1.0);
			glPushMatrix();
				glColor3f(0.1, 0.6, 0.2);
				glTranslatef(0.0, 20, 0.0);
				glScalef(0.3, 1.2, 0.5);
				glutSolidSphere(9, 32, 32);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
}

void draw()
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	//Draw the scene here
	// drawPlatform();
	// drawCannon();
	if(scenery)
	{
		drawStreet();
		drawSquidwardHouse();
		glPushMatrix();
			glTranslatef(65.0, 12.0, 0.0);
			drawSpongebobHouse();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(-70.0, 5.0, 0.0);
			drawPatrickHouse();
		glPopMatrix();

		glPopMatrix();
	}
}

void ainfo::drawTrailParticle()
{
	if(!blockmode)
		glutSolidSphere(0.1, 6, 6);
	else 
	{
		glRotatef(theta, 1.0, 1.0, 1.0);
		drawBox(2.0, 2.0, 2.0, 2.5, 2.5, 2.5);
	}

	glPushMatrix();
	glTranslatef(lx,ly,lz);

	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glPopMatrix();
}

void ainfo::drawTrail()
{
	if(vy > 0)
	{
		for(unsigned int i = 0; i < TRAILSPORES*TRAIL; i++)
		{
			glPushMatrix();
			glTranslatef(trail_part[i].x, trail_part[i].y, trail_part[i].z);
			glColor3f(1.0, 1.0, 1.0);
			drawTrailParticle();
			glPopMatrix();
		}
	}
}

void ainfo::setTrail()
{
	int max = 2, min = -2, ymax = -1, ymin = -4;
	unsigned int i, count = 0;

	if(swap == TRAILSPORES*TRAIL)
		swap = 0;
	i = swap;
	while(count != TRAIL)
	{
		trail_part[i].x = x;
		trail_part[i].y = y - 3;
		trail_part[i].z = z;
		trail_part[i].vx = (float)(rand()%(max-min + 1) + min);
		trail_part[i].vy = (float)(rand()%(ymax-ymin + 1) + ymin);
		trail_part[i].vz = (float)(rand()%(max-min + 1) + min);
		i++;
		count++;
	}
	swap += TRAIL;
	glutGet(GLUT_ELAPSED_TIME);
}

void ainfo::drawExplosionParticle()
{
	if(!blockmode)
		glutSolidSphere(0.3, 6, 6);
	else
	{
		glRotatef(theta, 1.0, 1.0, 1.0);
		// glDisable(GL_LIGHTING);
		// glDisable(GL_TEXTURE_2D);
		drawBox(5.0, 5.0, 5.0, 0, 0, 0);
		// glEnable(GL_LIGHTING);
		// glEnable(GL_TEXTURE_2D);
	}

	glPushMatrix();
	glTranslatef(lx,ly,lz);

	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glPopMatrix();
}

void ainfo::drawExplosion()
{
	if(splode)
	{
		for(unsigned int i = 0; i < MAXPARTICLES; i++)
		{
			if(blast_particles[i].y > 10)
			{
				glPushMatrix();
				glTranslatef(blast_particles[i].x, blast_particles[i].y, blast_particles[i].z);
				if(scenery)
					glColor4f(0.2, 0.4, 0.9, alpha);
				else if(!blockmode)
					glColor4f(blast_particles[i].r, blast_particles[i].g, blast_particles[i].b, alpha);
				else
					glColor3f(blast_particles[i].r, blast_particles[i].g, blast_particles[i].b);
				drawExplosionParticle();
				glPopMatrix();
			}
		}
	}
}

void ainfo::explosion()
{
	vx = 0.0;
	vy = 0.0;
	vz = 0.0;

	float red = 1.0 * (rand() / (float)RAND_MAX);
	float green = 1.0 * (rand() / (float)RAND_MAX);
	float blue = 1.0 * (rand() / (float)RAND_MAX);

	engine->play2D("Pop sound effect [Mpgun.com](edit).wav");

	for(unsigned int i = 0; i < MAXPARTICLES; i++)
	{
		blast_particles[i].x = x;
		blast_particles[i].y = y;
		blast_particles[i].z = z;
		blast_particles[i].vx = 10.0 * (rand() / (float)RAND_MAX) - 5.0;
		blast_particles[i].vy = 10.0 * (rand() / (float)RAND_MAX) - 5.0;
		blast_particles[i].vz = 10.0 * (rand() / (float)RAND_MAX) - 5.0;
		blast_particles[i].r = red;
		blast_particles[i].g = green;
		blast_particles[i].b = blue;
		blast_particles[i].active = 0.0;
	}
	glutGet(GLUT_ELAPSED_TIME);
}

void drawOneArrow()
{
	if(!blockmode)
	{
		// glutSolidSphere(0.3, 6, 2);
		drawBox(0.3, 1.5, 0.3, -0.15, -3.0, 0);
		GLUquadricObj *quadratic;
		quadratic = gluNewQuadric();
		glRotatef(90, 1.0, 0.0, 0.0);
		gluCylinder(quadratic, 0.0, 0.5, 1.5, 16, 16);
		glRotatef(-90, 1.0, 0.0, 0.0);
		glColor3f(0.63, 0.32, 0.18);
		drawBox(0.1, 3.5, 0.1, 0.1, -5.0, 0.1);
	}
	else
	{
		glRotatef(theta, 1.0, 1.0, 1.0);
		// glDisable(GL_LIGHTING);
		// glDisable(GL_TEXTURE_2D);
		drawBox(6.0, 6.0, 6.0, 0, 0, 0);
		// glEnable(GL_LIGHTING);
		// glEnable(GL_TEXTURE_2D);
	}

	glPushMatrix();
	glTranslatef(lx,ly,lz);

	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glPopMatrix();
}

void drawArrows()
{
	for(unsigned int i = 0; i < MAXARROWS; i++)
	{
		if(!arrows[i].splode && arrows[i].start)
		{
			glPushMatrix();
			glTranslatef(arrows[i].x, arrows[i].y, arrows[i].z);
			// glRotatef(, 1.0, 0.0, 0.0);
			// glRotatef(, 0.0, 0.0, 1.0);
			glScalef(1.5, 1.5, 1.5);
			glColor3f(arrows[i].r, arrows[i].g, arrows[i].b);
			drawOneArrow();
			glPopMatrix();
		}
	}
}

void fireArrows()
{
	int max = 8, min = -8, ymax = 70, ymin = 50;

	for(unsigned int i = 0; i < MAXARROWS; i++)
	{
		engine->play2D("firework_start.wav");
		arrows[i].splode = false;
		arrows[i].start = true;
		arrows[i].swap = 0;
		arrows[i].alpha = 1.0;
		arrows[i].set = false;
		arrows[i].x = 0.0;
		arrows[i].y = 15.0;
		arrows[i].z = 0.0;
		arrows[i].vx = (float)(rand()%(max-min + 1) + min);
		arrows[i].vy = (float)(rand()%(ymax-ymin + 1) + ymin);
		arrows[i].vz = (float)(rand()%(max-min + 1) + min);
		arrows[i].r = 1.0;
		arrows[i].g = 0.0;
		arrows[i].b = 0.0;
		for(unsigned int j = 0; j < TRAILSPORES*TRAIL; j++)
		{
			arrows[i].trail_part[j].x = 0;
			arrows[i].trail_part[j].y = 0;
			arrows[i].trail_part[j].z = 0;
			arrows[i].trail_part[j].vx = 0;
			arrows[i].trail_part[j].vy = 0;
			arrows[i].trail_part[j].vz = 0;
		}
	}
	glutGet(GLUT_ELAPSED_TIME);
}

void addArrow()
{
	int max = 8, min = -8, ymax = 70, ymin = 50;

	engine->play2D("firework_start.wav");
	arrows[arrow_count].splode = false;
	arrows[arrow_count].start = true;
	arrows[arrow_count].swap = 0;
	arrows[arrow_count].alpha = 1.0;
	arrows[arrow_count].set = false;
	arrows[arrow_count].x = 0.0;
	arrows[arrow_count].y = 15.0;
	arrows[arrow_count].z = 0.0;
	arrows[arrow_count].vx = (float)(rand()%(max-min + 1) + min);
	arrows[arrow_count].vy = (float)(rand()%(ymax-ymin + 1) + ymin);
	arrows[arrow_count].vz = (float)(rand()%(max-min + 1) + min);
	arrows[arrow_count].r = 1.0;
	arrows[arrow_count].g = 0.0;
	arrows[arrow_count].b = 0.0;
	for(unsigned int j = 0; j < TRAILSPORES*TRAIL; j++)
	{
		arrows[arrow_count].trail_part[j].x = 0;
		arrows[arrow_count].trail_part[j].y = 0;
		arrows[arrow_count].trail_part[j].z = 0;
		arrows[arrow_count].trail_part[j].vx = 0;
		arrows[arrow_count].trail_part[j].vy = 0;
		arrows[arrow_count].trail_part[j].vz = 0;
	}
	glutGet(GLUT_ELAPSED_TIME);
}

void burstfireMode()
{
	if(burstfire)
	{
		if(arrow_count >= MAXARROWS)
		{
			burstfire = false;
			arrow_count = 0;
		}
		else if(bursttime > burstdelay)
		{
			addArrow();
			arrow_count++;
			bursttime = 0;
		}
	}
}

void drawOneBubble()
{
	glutSolidSphere(1.5, 10, 10);

	glPushMatrix();
	glTranslatef(lx,ly,lz);

	glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glPopMatrix();
}

void drawBubbles()
{
	int xmax = 750, xmin = -750, ymax = 0, ymin = -70, zmax = 250, zmin = -250;
	int vxmax = 2, vxmin = -2, vymax = 15, vymin = 1;
	int smax = 3, smin = 1;

	for(unsigned int i = 0; i < MAXBUBBLES; i++)
	{
		if(bubbles[i].time_alive > bubbletime || bubbles[i].alpha < 0)
		{
			bubbles[i].x = (float)(rand()%(xmax-xmin + 1) + xmin);
			bubbles[i].y = (float)(rand()%(ymax-ymin + 1) + ymin);
			bubbles[i].z = (float)(rand()%(zmax-zmin + 1) + zmin);
			bubbles[i].vx = (float)(rand()%(vxmax-vxmin + 1) + vxmin);
			bubbles[i].vy = (float)(rand()%(vymax-vymin + 1) + vymin);
			bubbles[i].scale = (float)(rand()%(smax-smin + 1) + smin);
			bubbles[i].alpha = 0.6;
			bubbles[i].time_alive = rand()%(10000 - 0 + 1) + 0;
		}
		else
		{
			glPushMatrix();
				glTranslatef(bubbles[i].x, bubbles[i].y, bubbles[i].z);
				glScalef(bubbles[i].scale, bubbles[i].scale, bubbles[i].scale);
				glColor4f(0.2, 0.4, 0.9, bubbles[i].alpha);
				drawOneBubble();
			glPopMatrix();
		}
	}
}

/*************SETUP FUNCTIONS*************/
void update()
{
	// View setup
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(g_orientation, 0.0, 1.0, 0.0); // rotate in y axis
	glTranslatef(-g_posX, -g_posY, -g_posZ);

	if(scenery)
		glClearColor(0.2, 0.53, 0.85, 1.0); //clear the screen to black
	else
		glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer

	// Lighting setup
	GLfloat DiffuseLight[] = {dlr, dlg, dlb}; //set DiffuseLight[] to the specified values
	GLfloat AmbientLight[] = {alr, alg, alb}; //set AmbientLight[] to the specified values
	glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //change the light accordingly
	glLightfv (GL_LIGHT1, GL_AMBIENT, AmbientLight); //change the light accordingly
	GLfloat LightPosition[] = {lx, ly, lz, lw}; //set the LightPosition to the specified values
	glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly
	gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //camera position, x,y,z, looking
	
	// Drawing
	draw();
	if(scenery)
		drawBubbles();
	drawArrows();
	burstfireMode();
	for(unsigned int i = 0; i < MAXARROWS; i++)
	{
		arrows[i].drawExplosion();
		arrows[i].drawTrail();
	}

	// Buffer
	glutSwapBuffers();

	// Global variable changes
	angle++;
	theta += 5;
}

void reshape(int w, int h)
{
	// Set viewport of current window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Changes

	// Set matrix back to model
	glMatrixMode(GL_MODELVIEW);
}

void timer(int value)
{
	// Time variables
	static int lastTime;
	int thisTime;
	float time;
	thisTime = glutGet(GLUT_ELAPSED_TIME);
	int usedTime = (thisTime - lastTime);
	trailtime += (thisTime - lastTime);
	randtime += (thisTime - lastTime);
	bursttime += (thisTime - lastTime);
	time = (thisTime - lastTime) / 500.0;
	lastTime = thisTime;

	float max = 0.5, min = -0.5;
	float randomizerx = 0.0, randomizerz = 0.0;	

	float a = 8.0; // gravity
	float a1 = 1.2; // gravity for explosion

	// Changes
	for(unsigned int i = 0; i < MAXARROWS; i++)
	{	
		if(randtime > randclock)
		{
			randomizerx = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max-min)));
			randomizerz = min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/(max-min)));
			arrows[i].vx += randomizerx;
			arrows[i].vz += randomizerz;
		}

		if(arrows[i].alpha > 0 && arrows[i].set)
			arrows[i].alpha -= ALPHA_DECREASE;
		if(!arrows[i].splode)
			arrows[i].vy = arrows[i].vy - a*time;
		if(arrows[i].y <= 0 && arrows[i].vy < 0)
		{
			arrows[i].vx = 0.0;
			arrows[i].vy = 0.0;
			arrows[i].vz = 0.0;
		}
		arrows[i].x = arrows[i].x + arrows[i].vx*time;
		arrows[i].y = arrows[i].y + arrows[i].vy*time;
		arrows[i].z = arrows[i].z + arrows[i].vz*time;
		if(arrows[i].vy > 0)
		{
			if(trailtime > trailclock)
			{
				arrows[i].setTrail();
			}
			for(unsigned j = 0; j < TRAILSPORES*TRAIL; j++)
			{
				arrows[i].trail_part[j].x = arrows[i].trail_part[j].x + arrows[i].trail_part[j].vx*time;
				arrows[i].trail_part[j].y = arrows[i].trail_part[j].y + arrows[i].trail_part[j].vy*time;
				arrows[i].trail_part[j].z = arrows[i].trail_part[j].z + arrows[i].trail_part[j].vz*time;
			}
		}
		if(arrows[i].vy < - 3)
		{
			arrows[i].splode = true;
			if(!arrows[i].set){
				arrows[i].set = true;
				arrows[i].explosion();
			}
		}
		if(arrows[i].splode){
			for(unsigned int j = 0; j < MAXPARTICLES; j++)
			{
				arrows[i].blast_particles[j].active += time;
				arrows[i].blast_particles[j].vy = arrows[i].blast_particles[j].vy - a1*time;
				if (arrows[i].blast_particles[j].y <= 0 && arrows[i].blast_particles[j].vy < 0)
				{
					arrows[i].blast_particles[j].vx = 0.0;
					arrows[i].blast_particles[j].vy = 0.0;
					arrows[i].blast_particles[j].vz = 0.0;
				} 
				arrows[i].blast_particles[j].x = arrows[i].blast_particles[j].x + arrows[i].blast_particles[j].vx*time;
				arrows[i].blast_particles[j].y = arrows[i].blast_particles[j].y + arrows[i].blast_particles[j].vy*time;
				arrows[i].blast_particles[j].z = arrows[i].blast_particles[j].z + arrows[i].blast_particles[j].vz*time;
			}
		}
	}

	for(unsigned int i = 0; i < MAXBUBBLES; i++)
	{

		bubbles[i].x += bubbles[i].vx * time;
		bubbles[i].y += bubbles[i].vy * time;
		bubbles[i].alpha -= BUBBLE_ALPHA_DEC;	
		bubbles[i].time_alive += usedTime;
	}

	if(trailtime > trailclock)
		trailtime = 0;
	if(randtime > randclock)
		randtime = 0;

	// Redisplay
	glutPostRedisplay();
	glutTimerFunc(50, &timer, 0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'e':
			g_posY = g_posY + 4.0;
			break;
		case 'r':
			g_posY = g_posY - 4.0;
			break;
		case 'w':
			g_posX = g_posX + sin(g_orientation / 180.0 * M_PI);
			g_posZ = g_posZ - cos(g_orientation / 180.0 * M_PI);
			break;
		case 's':
			g_posX = g_posX - sin(g_orientation / 180.0 * M_PI);
			g_posZ = g_posZ + cos(g_orientation / 180.0 * M_PI);
			break;
		case 'a':
			g_orientation = g_orientation - 15.0;
			break;
		case 'd':
			g_orientation = g_orientation + 15.0;
			break;
		case 'h':
			g_posX = 0.0; g_posY = 50.0; g_posZ = 80.0;
			g_orientation = 0.0;
			break;
		case 'i': // light up
			ly += 0.2; //move the light up
			break;
		case 'k': // light down
			if(ly > 0)
				ly -= 0.2; // move light down
			break;
		case 'j': // light left
			lx -= 0.2; // move light left
			break;
		case 'l': // light right
			lx += 0.2; // move light right
			break;
		case 'o':
			if(arrow_count >= MAXARROWS)
				arrow_count = 0;
			addArrow();
			arrow_count++;
			break;
		case 'p':
			fireArrows();
			break;
		case '[':
			burstfire = true;
			break;
		case '1':
			blockmode = !blockmode;
			break;
		case '2':
			scenery = !scenery;
			break;
		case 'q':
			engine->drop();
			exit(0);
			break;
	}
	glutPostRedisplay();
}

void init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_SMOOTH);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
}

void setup()
{
	glMatrixMode(GL_PROJECTION);
	gluPerspective(120.0, 1.0, 1.0, 1000.0);
	glEnable(GL_DEPTH_TEST);
}

/*************MAIN*************/
int main(int argc, char** argv)
{
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize (1200, 1200);
	glutInitWindowPosition (300, 300);
	glutCreateWindow("Final project s2027100");

	init();
	setup();
	glutDisplayFunc(&update);
	glutIdleFunc(&update);
	glutReshapeFunc(&reshape);
	glutKeyboardFunc(&keyboard);
	glutTimerFunc(50, &timer, 0);
	glutMainLoop();
	return 0;
}