#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <math.h>

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

double xc = 2.0;
double yc = 2.0;
double zc = 2.0;

//chair
bool chair = false;
bool chairUp = true;
double chairY = 0.0;
double chairAngle;

//teapot
bool teapot = false;
bool bottleCapUp = true;
double capY = 0;
double teapotAngle = 0;

//cup
bool cup = false;
bool cupUp = true;
double cupY = 0;
double cupScale = 1;

//sweeper
bool sweeper = false;
bool sweepingDirection = true;
double sweeperAngle = 0;

//beast1
bool beast1 = false;
int B1Stage = 1;
double B1Stage1LowerRightArm = 0;
double B1Stage1LowerLeftArm = 0;
double B1Stage2RightArm = 0;
double B1Stage3RightArm = 0;
bool B1Stage4Decreasing = true;
double B1Stage4RightArm = 0;
double B1Stage4LeftArm = 0;
bool set = true;

//beast2
bool beast2 = false;
int B2Stage = 1;
double B2Stage1LowerRightArm = 0;
double B2Stage1LowerLeftArm = 0;
double B2Stage2RightArm = 0;
double B2Stage3RightArm = 0;
bool B2Stage4Decreasing = true;
double B2Stage4RightArm = 0;
double B2Stage4LeftArm = 0;
bool set2 = true;
bool B2S4Direction = true;
double B2S4Distance = 0;


//princess1
bool princess1 = false;
bool princessDirection = true;
bool princessFront = true;
double princessAngle = 0;
double princessDistance = 0;

//princess2
bool princess2 = false;
int P2Stage = 1;
double P2S1ArmAngle = 0;
double P2S2RightAngle = 0;
double P2S2LeftAngle = 0;
double P2S3RightAngle = 0;
double P2S3LeftAngle = 0;
double P2S3Head = 0;
double P2S4Left = 0;
double P2S4Right = 0;
double P2S4Head = 0;
bool P2S4HeadDirection = true;
bool P2S4Direction = true;
double P2S4Distance = 0;

//resets
bool Resetprincess1 = false;
bool Resetprincess2 = false;
bool Resetbeast1 = false;
bool Resetbeast2 = false;
bool Resetcup = false;
bool Resetsweeper = false;
bool Resetteapot = false;
bool Resetchair = false;

//camera
bool top = false;
double x = 0;
double y = 6;
double seta = 0;

class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(0, 40, 0);
Vector At(0, 1, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;

// Textures
GLTexture tex_ground;

//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, -20);
	glTexCoord2f(5, 0);
	glVertex3f(20, 0, -20);
	glTexCoord2f(5, 5);
	glVertex3f(20, 0, 20);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 20);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void wall(double thickness) {
	glPushMatrix();
	glTranslated(0.5, 0.5*thickness, 0.5);
	glScaled(1.0, thickness, 1.0);
	glutSolidCube(1);
	glPopMatrix();
}

void tableLeg(double thick, double len) {
	glPushMatrix();
	glTranslated(0, len / 2, 0);
	glScaled(thick, len, thick);
	glutSolidCube(1.0);
	glPopMatrix();
}

void jackpart() {

	glPushMatrix();
	glScaled(0.2, 0.2, 1.0);
	glutSolidSphere(1, 15, 15);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, 1.2);
	glutSolidSphere(0.2, 15, 15);
	glTranslated(0, 0, -2.4);
	glutSolidSphere(0.2, 15, 15);
	glPopMatrix();

}

void jack() {
	glPushMatrix();
	jackpart();
	glRotated(90.0, 0, 1, 0);
	jackpart();
	glRotated(90.0, 1, 0, 0);
	jackpart();
	glPopMatrix();
}

void table(double topWid, double topThick, double legThick, double legLen) {
	//draw a table - atop and four legs 
	glPushMatrix();
	glTranslated(0, legLen, 0);
	glScaled(topWid, topThick, topWid);
	glutSolidCube(1.0);
	glPopMatrix();

	double dist = 0.95*topWid / 2.0 - legThick / 2.0;

	glPushMatrix();
	glTranslated(dist, 0, dist);
	tableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	tableLeg(legThick, legLen);
	glTranslated(-2 * dist, 0, 2 * dist);
	tableLeg(legThick, legLen);
	glTranslated(0, 0, -2 * dist);
	tableLeg(legThick, legLen);
	glPopMatrix();
}

void SetupLights()
{
	GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7, 1.0f };
	GLfloat mat_diffuse[] = { 0.6f, 0.6f, 0.6, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat mat_shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	//set the light source properties
	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat light_position[] = { -7.0f, 6.0f, 3.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightIntensity);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void drawCircle(int x, int y, float r) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	GLUquadric *quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}

void drawCup() {
	glPushMatrix();
	glScaled(0.7, 0.7, 0.7);
	//bottom
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, -0.1, 0);
	glRotated(90, 1, 0, 0);
	drawCircle(0, 0, 0.1);
	glPopMatrix();

	//middle
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, 0, 0);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj21;
	qobj21 = gluNewQuadric();
	gluQuadricDrawStyle(qobj21, GLU_FLAT);
	gluCylinder(qobj21, 0.2, 0.1, 0.1, 20, 20);
	glPopMatrix();

	//top
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, 0.2, 0);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj22;
	qobj22 = gluNewQuadric();
	gluQuadricDrawStyle(qobj22, GLU_FLAT);
	gluCylinder(qobj22, 0.2, 0.2, 0.2, 20, 20);
	glPopMatrix();

	//ear
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(-0.25, 0.1, 0);
	glScaled(0.3, 0.3, 0.5);
	glutSolidTorus(0.1, 0.2, 20, 20);
	glPopMatrix();

	//upper ring
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0, 0.2, 0);
	glRotated(90, 1, 0, 0);
	glScaled(0.9, 0.9, 0.7);
	glutSolidTorus(0.03, 0.2, 20, 20);
	glPopMatrix();

	glPopMatrix();
}

void drawPrincess() {

	//bottom body
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0, -1.3, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.7, 1, 20, 5);
	glPopMatrix();

	glPushMatrix();
	glRotated(princessAngle, 0, 1, 0);
	//dress
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0, 0, 0);
	glScaled(1.0, 1, 1.5);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FLAT);
	gluCylinder(qobj, 0.15, 0.1, 1, 20, 5);
	glPopMatrix();

	//upper body
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(-0.02, 0.05, 0);
	glScaled(0.8, 0.7, 1.1);
	glutSolidSphere(0.20, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0.08, 0, -0.09);
	glRotated(-25, 0, 0, 1);
	glScaled(1.0, 0.8, 0.97);
	glutSolidSphere(0.12, 8, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0.08, 0, 0.09);
	glRotated(-25, 0, 0, 1);
	glScaled(1.0, 0.8, 0.97);
	glutSolidSphere(0.12, 8, 20);
	glPopMatrix();


	//right hand
	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, 0.05, 0.2);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.05, 0.2);
	glRotated(-P2S1ArmAngle, 1, 0, 0);
	glTranslated(0, -0.05, -0.2);
	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, 0.05, 0.2);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj3;
	qobj3 = gluNewQuadric();
	gluQuadricDrawStyle(qobj3, GLU_FLAT);
	gluCylinder(qobj3, 0.1, 0.07, 0.4, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -0.35, 0.2);
	glRotated(45, 0, 0, 1);
	//glRotated(P2S4Right, 1, 0, 0);
	glRotated(P2S3RightAngle, 1, 0, 0);
	glRotated(P2S2RightAngle, 0, 0, 1);
	glTranslated(0, 0.35, -0.2);
	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, -0.35, 0.2);
	glutSolidSphere(0.07, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, -0.35, 0.2);
	glRotated(-45, 0, 1, 0);
	glRotated(135, 1, 0, 0);
	GLUquadricObj * qobj4;
	qobj4 = gluNewQuadric();
	gluQuadricDrawStyle(qobj4, GLU_FLAT);
	gluCylinder(qobj4, 0.07, 0.05, 0.3, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0.15, -0.55, 0.05);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//left hand
	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, 0.05, -0.2);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.05, -0.2);
	glRotated(P2S1ArmAngle, 1, 0, 0);
	glTranslated(0, -0.05, 0.2);
	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, 0.05, -0.2);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj5;
	qobj5 = gluNewQuadric();
	gluQuadricDrawStyle(qobj5, GLU_FLAT);
	gluCylinder(qobj5, 0.1, 0.07, 0.4, 4, 5);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -0.35, -0.2);
	glRotated(P2S4Left, 1, 0, 0);
	glRotated(P2S3LeftAngle, 0, 1, 0);
	glRotated(P2S2LeftAngle, 0, 0, 1);
	glTranslated(0, 0.35, 0.2);
	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, -0.35, -0.2);
	glutSolidSphere(0.07, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, -0.35, -0.2);
	glRotated(45, 0, 1, 0);
	glRotated(-45, 1, 0, 1);
	GLUquadricObj * qobj6;
	qobj6 = gluNewQuadric();
	gluQuadricDrawStyle(qobj6, GLU_FLAT);
	gluCylinder(qobj6, 0.07, 0.05, 0.3, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0.2, -0.2, -0.05);
	glutSolidSphere(0.05, 20, 20);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0.1, 0);
	glRotated(P2S4Head, 0, 1, 0);
	glRotated(P2S3Head, 0, 1, 0);
	glTranslated(0, -0.1, 0);
	//mouth
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0.18, 0.3, 0);
	glRotated(45, 0, 0, 1);
	glRotated(90, 0, 1, 0);
	glScaled(1, 0.5, 4);
	drawCircle(0, 0, 0.05);
	glPopMatrix();

	//head
	glPushMatrix();
	glTranslated(0, 0.1, 0);
	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, 0.3, 0);
	glScaled(1, 1.1, 0.8);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	//hair
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.1, 0.35, -0.11);
	glRotated(90, 0, 1, 0);
	glScaled(1, 1.2, 1.2);
	drawCircle(0, 0, 0.05);
	glPopMatrix();

	//more hair
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.1, 0.35, 0.11);
	glRotated(90, 0, 1, 0);
	glScaled(1, 1.2, 1.2);
	drawCircle(0, 0, 0.05);
	glPopMatrix();

	//right eye
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0.2, 0.35, 0.07);
	glScaled(1, 0.9, 2);
	glRotated(-25, 0, 1, 0);
	glRotated(90, 0, 1, 0);
	glScaled(1, 1.2, 2);
	drawCircle(0, 0, 0.03);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.99, 0.75, 0.07);
	glTranslated(0.22, 0.35, 0.07);
	glRotated(-14.5, 0, 1, 0);
	glRotated(90, 0, 1, 0);
	drawCircle(0, 0, 0.035);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.23, 0.35, 0.07);
	glRotated(-14.5, 0, 1, 0);
	glRotated(90, 0, 1, 0);
	drawCircle(0, 0, 0.017);
	glPopMatrix();

	//left eye
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0.2, 0.35, -0.07);
	glScaled(1, 0.9, 2);
	glRotated(25, 0, 1, 0);
	glRotated(90, 0, 1, 0);
	glScaled(1, 1.2, 2);
	drawCircle(0, 0, 0.03);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.99, 0.75, 0.07);
	glTranslated(0.22, 0.35, -0.07);
	glRotated(14.5, 0, 1, 0);
	glRotated(90, 0, 1, 0);
	drawCircle(0, 0, 0.035);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.23, 0.35, -0.07);
	glRotated(14.5, 0, 1, 0);
	glRotated(90, 0, 1, 0);
	drawCircle(0, 0, 0.017);
	glPopMatrix();

	//hair too
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0, 0.4, 0);
	glScaled(0.8, 0.7, 0.7);
	glRotated(30.0, 0, 0, 1);
	glRotated(90.0, 1, 0, 0);
	glutSolidTorus(0.1, 0.15, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(-0.05, 0.35, 0);
	glScaled(1, 1, 0.85);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(-0.3, 0.1, 0);
	glScaled(0.5, 2, 1.0);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	glPopMatrix();

	//neck
	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0, 0.2, 0);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj7;
	qobj7 = gluNewQuadric();
	gluQuadricDrawStyle(qobj7, GLU_FLAT);
	gluCylinder(qobj7, 0.07, 0.05, 0.1, 20, 20);
	glPopMatrix();

	//nose
	glPushMatrix();
	glColor3f(0.99, 0.68, 0.375);
	glTranslated(0.17, 0.37, 0);
	glScaled(0.5, 0.3, 0.2);
	glRotated(25, 0, 0, 1);
	glutSolidCube(0.2);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}

void drawSweeper() {

	glPushMatrix();
	glScaled(0.7, 0.7, 0.7);

	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0, -0.5, 0);
	glScaled(0.5, 1, 1.5);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj11;
	qobj11 = gluNewQuadric();
	gluQuadricDrawStyle(qobj11, GLU_FLAT);
	gluCylinder(qobj11, 0.03, 0.2, 0.4, 20, 5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, -0.9, 0);
	glScaled(0.5, 1, 1.5);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj10;
	qobj10 = gluNewQuadric();
	gluQuadricDrawStyle(qobj10, GLU_FLAT);
	gluCylinder(qobj10, 0.2, 0.25, 0.1, 20, 5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9, 0.5, 0.2);
	glTranslated(0, 0, 0);
	glScaled(1.0, 1, 1.5);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj12;
	qobj12 = gluNewQuadric();
	gluQuadricDrawStyle(qobj12, GLU_FLAT);
	gluCylinder(qobj12, 0.02, 0.03, 0.5, 20, 5);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.9, 0.5, 0.2);
	glTranslated(0, 0.05, 0);
	glutSolidSphere(0.09, 20, 20);
	glPopMatrix();
	glPopMatrix();
}

void drawBeast() {

	glPushMatrix();
	glTranslated(0, -0.9, -0.15);
	glRotated(B2Stage4LeftArm, 0, 0, 1);
	glRotated(B1Stage4LeftArm, 0, 0, 1);
	glTranslated(0, 0.9, 0.15);
	//left top leg
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, -0.9, -0.15);
	glRotated(45, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj35;
	qobj35 = gluNewQuadric();
	gluQuadricDrawStyle(qobj35, GLU_FLAT);
	gluCylinder(qobj35, 0.15, 0.1, 0.3, 20, 20);
	glPopMatrix();

	//left upper knee
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.2, -1.1, -0.15);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	//left upper leg
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.2, -1.1, -0.15);
	glRotated(-45, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj33;
	qobj33 = gluNewQuadric();
	gluQuadricDrawStyle(qobj33, GLU_FLAT);
	gluCylinder(qobj33, 0.1, 0.1, 0.3, 20, 20);
	glPopMatrix();

	//left lower knee
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0, -1.3, -0.15);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	//left bottom leg
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0, -1.3, -0.15);
	glRotated(45, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj31;
	qobj31 = gluNewQuadric();
	gluQuadricDrawStyle(qobj31, GLU_FLAT);
	gluCylinder(qobj31, 0.1, 0.1, 0.3, 20, 20);
	glPopMatrix();

	//left foot
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.3, -1.5, -0.15);
	glScaled(1.5, 0.5, 1);
	glutSolidCube(0.2);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -0.9, 0.15);
	glRotated(-B2Stage4RightArm, 0, 0, 1);
	glRotated(-B1Stage4RightArm, 0, 0, 1);
	glTranslated(0, 0.9, -0.15);
	//right top leg
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, -0.9, 0.15);
	glRotated(45, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj36;
	qobj36 = gluNewQuadric();
	gluQuadricDrawStyle(qobj36, GLU_FLAT);
	gluCylinder(qobj36, 0.15, 0.1, 0.3, 20, 20);
	glPopMatrix();

	//right upper knee
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.2, -1.1, 0.15);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	//right upper leg
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.2, -1.1, 0.15);
	glRotated(-45, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj34;
	qobj34 = gluNewQuadric();
	gluQuadricDrawStyle(qobj34, GLU_FLAT);
	gluCylinder(qobj34, 0.1, 0.1, 0.3, 20, 20);
	glPopMatrix();

	//right lower knee
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0, -1.3, 0.15);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	//right bottom leg
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0, -1.3, 0.15);
	glRotated(45, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	GLUquadricObj * qobj32;
	qobj32 = gluNewQuadric();
	gluQuadricDrawStyle(qobj32, GLU_FLAT);
	gluCylinder(qobj32, 0.1, 0.1, 0.3, 20, 20);
	glPopMatrix();

	//right foot
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.3, -1.5, 0.15);
	glScaled(1.5, 0.5, 1);
	glutSolidCube(0.2);
	glPopMatrix();
	glPopMatrix();

	//lower body
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, -0.9, 0);
	glScaled(0.7, 1, 1.5);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	//middle body
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, 0, 0);
	glRotated(0, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glScaled(0.8, 1.4, 1.9);
	GLUquadricObj * qobj37;
	qobj37 = gluNewQuadric();
	gluQuadricDrawStyle(qobj37, GLU_FLAT);
	gluCylinder(qobj37, 0.25, 0.2, 0.5, 20, 20);
	glPopMatrix();

	//yellow shirt
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0.08, 0, 0);
	glRotated(0, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 1, 1.7);
	GLUquadricObj * qobj38;
	qobj38 = gluNewQuadric();
	gluQuadricDrawStyle(qobj38, GLU_FLAT);
	gluCylinder(qobj38, 0.25, 0.2, 0.5, 20, 20);
	glPopMatrix();

	//upper body
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, 0, 0);
	glRotated(0, 0, 0, 1);
	glRotated(-90, 1, 0, 0);
	glScaled(0.8, 1.4, 1.9);
	glutSolidCone(0.25, 0.25, 20, 20);
	glPopMatrix();

	//top shirt
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0.05, 0.1, 0);
	glRotated(30, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 1, 1.7);
	GLUquadricObj * qobj39;
	qobj39 = gluNewQuadric();
	gluQuadricDrawStyle(qobj39, GLU_FLAT);
	gluCylinder(qobj39, 0.25, 0.19, 0.1, 20, 20);
	glPopMatrix();

	//neck
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0, 0.13, 0);
	glRotated(0, 0, 0, 1);
	glRotated(-90, 1, 0, 0);
	glScaled(0.8, 1.4, 1.9);
	glutSolidCone(0.2, 0.2, 20, 20);
	glPopMatrix();

	//head
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.1, 0.25, 0);
	glScaled(1, 1.5, 1.3);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	//beard
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.5);
	glTranslated(0.08, 0.15, 0);
	glScaled(1, 1.5, 0.8);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	//ears
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.17, 0.4, 0.02);
	glScaled(0.2, 0.1, 2);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	//right horn
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslated(0.17, 0.42, -0.15);
	glRotated(-45, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.04, 0.2, 20, 20);
	glPopMatrix();

	//left horn
	glPushMatrix();
	glColor3f(0.5, 0.5, 0.5);
	glTranslated(0.17, 0.42, 0.15);
	glRotated(45, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.04, 0.2, 20, 20);
	glPopMatrix();

	//mouth
	glPushMatrix();
	glColor3f(0.87, 0.156, 0.137);
	glTranslated(0.2, 0.18, 0);
	glRotated(90, 1, 0, 0);
	glScaled(1, 1, 0.5);
	glutSolidTorus(0.07, 0.05, 20, 20);
	glPopMatrix();

	//tooth
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0.295, 0.18, -0.05);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.01, 0.07, 20, 20);
	glPopMatrix();

	//tooth
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0.295, 0.18, 0.05);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.01, 0.07, 20, 20);
	glPopMatrix();

	//nose
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.5);
	glTranslated(0.27, 0.3, 0);
	glRotated(25, 0, 0, 1);
	glScaled(1, 1, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	//eye brows
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.5);
	glTranslated(0.27, 0.4, -0.1);
	glScaled(0.5, 0.5, 1);
	glutSolidCube(0.1);
	glPopMatrix();

	//eye brows
	glPushMatrix();
	glColor3f(0.9, 0.9, 0.5);
	glTranslated(0.27, 0.4, 0.1);
	glScaled(0.5, 0.5, 1);
	glutSolidCube(0.1);
	glPopMatrix();

	//left eye
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0.3, 0.32, -0.1);
	glRotated(-90, 0, 1, 0);
	glScaled(1, 0.6, 1);
	drawCircle(0, 0, 0.05);
	glPopMatrix();

	//el nini
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0.31, 0.32, -0.1);
	glRotated(-90, 0, 1, 0);
	glScaled(1, 1, 1);
	drawCircle(0, 0, 0.03);
	glPopMatrix();

	//gowa el nini
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.315, 0.32, -0.1);
	glRotated(-90, 0, 1, 0);
	glScaled(1, 1, 1);
	drawCircle(0, 0, 0.015);
	glPopMatrix();

	//right eye
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0.3, 0.32, 0.105);
	glRotated(-90, 0, 1, 0);
	glScaled(1, 0.6, 1);
	drawCircle(0, 0, 0.05);
	glPopMatrix();

	//el nini
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0.31, 0.32, 0.105);
	glRotated(-90, 0, 1, 0);
	glScaled(1, 1, 1);
	drawCircle(0, 0, 0.03);
	glPopMatrix();

	//gowa el nini
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslated(0.315, 0.32, 0.105);
	glRotated(-90, 0, 1, 0);
	glScaled(1, 1, 1);
	drawCircle(0, 0, 0.015);
	glPopMatrix();

	//back neck
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0, 0.15, 0);
	glRotated(-25, 0, 0, 1);
	glScaled(0.5, 1, 0.5);
	glutSolidSphere(0.5, 20, 20);
	glPopMatrix();

	//right arm
	glPushMatrix();
	glTranslated(0, -0.02, 0.2);
	glRotated(-B2Stage4RightArm, 0, 1, 0);
	//glRotated(B2Stage3RightArm - 20, 0, 1, 0);
	glRotated(B2Stage2RightArm, 0, 0, 1);
	glRotated(B1Stage4RightArm, 0, 0, 1);
	glRotated(B1Stage3RightArm - 20, 0, 1, 0);
	glRotated(B1Stage2RightArm, 0, 0, 1);
	glTranslated(0, 0.02, -0.2);
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, -0.02, 0.2);
	glRotated(25, 0, 1, 0);
	glRotated(45, 1, 0, 0);
	glScaled(1, 1, 1);
	GLUquadricObj * qobj310;
	qobj310 = gluNewQuadric();
	gluQuadricDrawStyle(qobj310, GLU_FLAT);
	gluCylinder(qobj310, 0.13, 0.11, 0.35, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0.1, -0.27, 0.41);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.1, -0.27, 0.41);
	glRotated(B2Stage1LowerRightArm + 10, 1, 0, 0);
	glRotated(B1Stage1LowerRightArm + 10, 1, 0, 0);
	glTranslated(-0.1, 0.27, -0.41);
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0.1, -0.27, 0.41);
	glRotated(-10, 0, 1, 0);
	glRotated(155, 1, 0, 0);
	glScaled(1, 1, 1);
	GLUquadricObj * qobj313;
	qobj313 = gluNewQuadric();
	gluQuadricDrawStyle(qobj313, GLU_FLAT);
	gluCylinder(qobj313, 0.11, 0.09, 0.35, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(0.18, -0.45, 0.05);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//left arm
	glPushMatrix();
	glTranslated(0, -0.02, -0.21);
	glRotated(B2Stage4RightArm, 0, 1, 0);
	glRotated(-B2Stage3RightArm, 0, 1, 0);
	glRotated(B2Stage2RightArm + 10, 0, 0, 1);
	glRotated(B1Stage4RightArm, 0, 0, 1);
	glRotated(-B1Stage3RightArm, 0, 1, 0);
	glRotated(B1Stage2RightArm + 10, 0, 0, 1);
	glTranslated(0, 0.02, 0.21);
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(0, -0.02, -0.21);
	glRotated(25, 0, 1, 0);
	glRotated(135, 1, 0, 0);
	glScaled(1, 1, 1);
	GLUquadricObj * qobj311;
	qobj311 = gluNewQuadric();
	gluQuadricDrawStyle(qobj311, GLU_FLAT);
	gluCylinder(qobj311, 0.13, 0.11, 0.35, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(-0.11, -0.3, -0.45);
	glutSolidSphere(0.11, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.11, -0.3, -0.45);
	glRotated(B2Stage1LowerLeftArm, 1, 0, 0);
	glRotated(B1Stage1LowerLeftArm, 1, 0, 0);
	glTranslated(0.11, 0.3, 0.45);
	glPushMatrix();
	glColor3f(0, 0, 1);
	glTranslated(-0.11, -0.3, -0.45);
	glRotated(0, 0, 1, 0);
	glRotated(45, 1, 0, 0);
	glScaled(1, 1, 1);
	GLUquadricObj * qobj312;
	qobj312 = gluNewQuadric();
	gluQuadricDrawStyle(qobj312, GLU_FLAT);
	gluCylinder(qobj312, 0.11, 0.09, 0.39, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(-0.1, -0.6, -0.15);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//tail
	glPushMatrix();
	glColor3f(0.61, 0.24, 0.02);
	glTranslated(-0.2, -1.05, 0);
	glRotated(-25, 0, 0, 1);
	glScaled(0.7, 2, 0.5);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();
}

void drawChair() {
	//top
	glPushMatrix();
	glColor3f(1, 0, 0);
	glTranslated(0, 0, 0);
	glScaled(1, 0.3, 0.7);
	glutSolidCube(0.6);
	glPopMatrix();

	//leg1
	glPushMatrix();
	glColor3f(0.9, 0.7, 0.1);
	glTranslated(0.2, -0.05, 0.1);
	glRotated(90, 1, 0, 0);
	glutSolidCone(0.05, 0.3, 20, 20);
	glPopMatrix();

	//leg2
	glPushMatrix();
	glColor3f(0.9, 0.7, 0.1);
	glTranslated(0.2, -0.05, -0.1);
	glRotated(90, 1, 0, 0);
	glutSolidCone(0.05, 0.3, 20, 20);
	glPopMatrix();

	//leg3
	glPushMatrix();
	glColor3f(0.9, 0.7, 0.1);
	glTranslated(-0.2, -0.05, -0.1);
	glRotated(90, 1, 0, 0);
	glutSolidCone(0.05, 0.3, 20, 20);
	glPopMatrix();

	//leg4
	glPushMatrix();
	glColor3f(0.9, 0.7, 0.1);
	glTranslated(-0.2, -0.05, 0.1);
	glRotated(90, 1, 0, 0);
	glutSolidCone(0.05, 0.3, 20, 20);
	glPopMatrix();

	//tail1
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(0.4, -0.05, 0);
	glRotated(45, 0, 0, 1);
	glScaled(0.5, 1.5, 0.5);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();

	//tail2
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(-0.4, -0.05, 0);
	glRotated(-45, 0, 0, 1);
	glScaled(0.5, 1.5, 0.5);
	glutSolidSphere(0.1, 20, 20);
	glPopMatrix();
}

void drawTeapot() {
	//base
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(0, -0.15, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.15, 0.05, 20, 20);
	glPopMatrix();

	//body
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0, 0, 0);
	glScaled(0.9, 0.7, 0.9);
	glutSolidSphere(0.2, 20, 20);
	glPopMatrix();

	//mouth
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslated(0.15, 0, 0);
	glRotated(45, 0, 0, 1);
	glRotated(90, 0, 1, 0);
	GLUquadricObj * qobj40;
	qobj40 = gluNewQuadric();
	gluQuadricDrawStyle(qobj40, GLU_FLAT);
	gluCylinder(qobj40, 0.03, 0.025, 0.1, 20, 20);
	glPopMatrix();

	//top
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(0, 0.13 + capY, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(0.09, 0.05, 20, 20);
	glPopMatrix();

	//ball
	glPushMatrix();
	glColor3f(1, 0, 1);
	glTranslated(0, 0.17 + capY, 0);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();

	//torus
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(-0.2, 0, 0);
	glScaled(0.3, 0.4, 0.4);
	glutSolidTorus(0.1, 0.2, 20, 20);
	glPopMatrix();
}

//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}


void Timer(int value) {

	//checking step
	if (Resetbeast1) {
		beast1 = false;
		B1Stage = 1;
		B1Stage1LowerRightArm = 0;
		B1Stage1LowerLeftArm = 0;
		B1Stage2RightArm = 0;
		B1Stage3RightArm = 0;
		B1Stage4Decreasing = true;
		B1Stage4RightArm = 0;
		B1Stage4LeftArm = 0;
		set = true;
		Resetbeast1 = false;
	}

	if (Resetbeast2) {
		beast2 = false;
		B2Stage = 1;
		B2Stage1LowerRightArm = 0;
		B2Stage1LowerLeftArm = 0;
		B2Stage2RightArm = 0;
		B2Stage3RightArm = 0;
		B2Stage4Decreasing = true;
		B2Stage4RightArm = 0;
		B2Stage4LeftArm = 0;
		set2 = true;
		B2S4Direction = true;
		B2S4Distance = 0;
		Resetbeast2 = false;
	}

	if (Resetprincess1) {
		princess1 = false;
		princessDirection = true;
		princessFront = true;
		princessAngle = 0;
		princessDistance = 0;
		Resetprincess1 = false;
	}

	if (Resetprincess2) {
		princess2 = false;
		P2Stage = 1;
		P2S1ArmAngle = 0;
		P2S2RightAngle = 0;
		P2S2LeftAngle = 0;
		P2S3RightAngle = 0;
		P2S3LeftAngle = 0;
		P2S3Head = 0;
		P2S4Left = 0;
		P2S4Right = 0;
		P2S4Head = 0;
		P2S4HeadDirection = true;
		P2S4Direction = true;
		P2S4Distance = 0;
		Resetprincess2 = false;
	}

	if (Resetchair) {
		chair = false;
		chairUp = true;
		chairY = 0.0;
		chairAngle = 0;
		Resetchair = false;
	}

	if (Resetcup) {
		cup = false;
		cupUp = true;
		cupY = 0;
		cupScale = 1;
		Resetcup = false;
	}

	if (Resetteapot) {
		teapot = false;
		bottleCapUp = true;
		capY = 0;
		teapotAngle = 0;
		Resetteapot = false;
	}

	if (Resetsweeper) {
		sweeper = false;
		sweepingDirection = true;
		sweeperAngle = 0;
		Resetsweeper = false;
	}

	if (!top) {
		Eye.y = 3;
		seta += 0.01;
		Eye.x = 10 * cos(seta);
		Eye.z = 10 * sin(seta);
		glLoadIdentity();
		gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	}
	if (top) {
		Eye.x = 0;
		Eye.y = 10;
		Eye.z = 0;
		myMotion(500, 500);
	}

	//chair
	if (chair) {
		if (chairUp) {
			chairY += 0.01;
			chairAngle += 2;
			if (chairY >= 1.781)
				chairUp = false;
		}
		else {
			chairY -= 0.01;
			chairAngle += 2;
			if (chairY <= 0)
				chairUp = true;
		}
	}

	//teapot
	if (teapot) {
		if (bottleCapUp) {
			capY += 0.01;
			teapotAngle++;
			if (capY >= 0.5)
				bottleCapUp = false;
		}
		else {
			capY -= 0.01;
			teapotAngle++;
			if (capY <= 0)
				bottleCapUp = true;
		}
	}

	//cup
	if (cup) {
		if (cupUp) {
			cupY += 0.01;
			cupScale += 0.03;
			if (cupY >= 0.5)
				cupUp = false;
		}
		else {
			cupY -= 0.01;
			cupScale -= 0.03;
			if (cupY <= 0)
				cupUp = true;
		}
	}

	//sweeper
	if (sweeper) {
		if (sweepingDirection) {
			sweeperAngle++;
			if (sweeperAngle >= 25)
				sweepingDirection = false;
		}
		else {
			sweeperAngle--;
			if (sweeperAngle <= -25)
				sweepingDirection = true;
		}
	}

	//beast1
	if (beast1) {
		if (B1Stage == 1) {
			B1Stage1LowerLeftArm += 0.65;
			B1Stage1LowerRightArm -= 1;
			if (B1Stage1LowerRightArm == -55)
				B1Stage = 2;
		}
		else if (B1Stage == 2) {
			B1Stage2RightArm += 1;
			if (B1Stage2RightArm >= 60)
				B1Stage = 3;
		}
		else if (B1Stage == 3) {
			B1Stage3RightArm += 1;
			if (B1Stage3RightArm == 45)
				B1Stage = 4;
		}
		else if (B1Stage = 4) {
			if (set) {
				B1Stage4LeftArm = 25;
				set = false;
			}
			if (B1Stage4Decreasing) {
				B1Stage4RightArm -= 1;
				B1Stage4LeftArm -= 1;
				if (B1Stage4RightArm == -25)
					B1Stage4Decreasing = false;
			}
			else {
				B1Stage4RightArm += 1;
				B1Stage4LeftArm += 1;
				if (B1Stage4RightArm == 0)
					B1Stage4Decreasing = true;
			}
		}
	}

	//beast2
	if (beast2) {
		if (B2Stage == 1) {
			B2Stage1LowerLeftArm += 0.65;
			B2Stage1LowerRightArm -= 1;
			if (B2Stage1LowerRightArm == -55)
				B2Stage = 2;
		}
		else if (B2Stage == 2) {
			B2Stage2RightArm += 1;
			if (B2Stage2RightArm >= 60)
				B2Stage = 4;
		}
		//else if (B1Stage == 3) {
		//B1Stage3RightArm += 1;
		//if (B1Stage3RightArm == 45)
		//B1Stage = 4;
		//}
		else if (B2Stage = 4) {
			if (set2) {
				B2Stage4LeftArm = 25;
				set2 = false;
			}
			if (B2Stage4Decreasing) {
				B2Stage4RightArm -= 1;
				B2Stage4LeftArm -= 1;
				if (B2Stage4RightArm == -45)
					B2Stage4Decreasing = false;
			}
			if (!B2Stage4Decreasing) {
				B2Stage4RightArm += 1;
				B2Stage4LeftArm += 1;
				if (B2Stage4RightArm == 0)
					B2Stage4Decreasing = true;
			}
			if (B2S4Direction) {
				B2S4Distance += 0.01;
				if (B2S4Distance >= 0.75)
					B2S4Direction = false;
			}
			if (!B2S4Direction) {
				B2S4Distance -= 0.01;
				if (B2S4Distance <= -0.75)
					B2S4Direction = true;
			}
		}
	}

	//princess1
	if (princess1) {
		if (princessDirection) {
			princessAngle += 0.5;

			if (princessAngle == 25)
				princessDirection = false;
		}
		if (!princessDirection) {
			princessAngle -= 0.5;

			if (princessAngle == -25)
				princessDirection = true;
		}
		if (princessFront) {
			princessDistance += 0.01;

			if (princessDistance >= 0.5)
				princessFront = false;
		}
		if (!princessFront) {
			princessDistance -= 0.01;

			if (princessDistance <= 0)
				princessFront = true;
		}

	}

	//princess2
	if (princess2) {
		if (P2Stage == 1) {
			P2S1ArmAngle += 3;
			if (P2S1ArmAngle >= 90)
				P2Stage = 2;
		}
		else if (P2Stage == 2) {
			if (P2S2LeftAngle > -5) {
				P2S2LeftAngle -= 3;
			}
			P2S2RightAngle -= 3;
			if (P2S2RightAngle <= -90)
				P2Stage = 3;
		}
		else if (P2Stage == 3) {
			if (P2S3RightAngle > -45)
				P2S3RightAngle -= 3;
			if (P2S3Head > -75)
				P2S3Head -= 1;
			P2S3LeftAngle += 3;
			if (P2S3LeftAngle >= 115)
				P2Stage = 4;
		}
		else if (P2Stage == 4) {
			if (P2S4Left > -115)
				P2S4Left--;
			P2S4Right--;
			if (P2S4Right <= -180)
				P2Stage = 5;
		}
		else if (P2Stage == 5) {
			princessAngle++;
			if (P2S4HeadDirection) {
				P2S4Head += 0.2;
				if (P2S4Head >= 90) {
					P2S4HeadDirection = false;
				}
			}
			if (!P2S4HeadDirection) {
				P2S4Head -= 0.2;
				if (P2S4Head <= 0) {
					P2S4HeadDirection = true;
				}
			}
			//if (P2S4Direction) {
			P2S4Distance += 0.01;
			//if (P2S4Distance >= 1.5)
			//P2S4Direction = false;
			//}
			//if (!P2S4Direction) {
			//P2S4Distance -= 0.01;
			//if (P2S4Distance <= -1.5)
			//P2S4Direction = true;
			//}

		}

	}

	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();

	// recall the Timer function after 1 millisecond 
	glutTimerFunc(1 * 10, Timer, 0);
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0 * 64 / 48.0, 2.0 * 64 / 48.0, -2.0, 2.0, 0.00001, 100);
	//gluPerspective(60, 640 / 480, 0.001, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(xc + 2, yc + 2, zc + 2, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void Keyboard(unsigned char key, int x, int y) {
	if (key == 'w')
		yc += 1;
	if (key == 'a')
		xc -= 1;
	if (key == 's')
		yc -= 1;
	if (key == 'd')
		xc += 1;
	if (key == 'e')
		zc += 1;
	if (key == 'f')
		zc -= 1;
	glutPostRedisplay();
}


//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//setupCamera();

	GLfloat lightIntensity[] = { 0.7, 0.7, 0.7, 1.0f };
	GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);

	// Draw Ground
	RenderGround();

	// Draw Tree Model
	//glPushMatrix();
	//glTranslatef(10, 0, 0);
	//glScalef(0.7, 0.7, 0.7);
	//model_tree.Draw();
	//glPopMatrix();

	// Draw house Model
	//glPushMatrix();
	//glRotatef(90.f, 1, 0, 0);
	//model_house.Draw();
	//glPopMatrix();


	//sky box
	glPushMatrix();

	GLUquadricObj * qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	//done
	glPushMatrix();
	glTranslated(princessDistance + sin(P2S4Distance), 1.3, cos(P2S4Distance));
	drawPrincess();
	glPopMatrix();

	//done
	glPushMatrix();
	glTranslated(-3, 0.7, -1);
	glRotated(sweeperAngle, 0, 0, 1);
	drawSweeper();
	glPopMatrix();

	//done
	glPushMatrix();
	glTranslated(-3, 0.1 + cupY, -2);
	glScaled(cupScale, cupScale, cupScale);
	drawCup();
	glPopMatrix();

	//done 
	glPushMatrix();
	glTranslated(0, 1.53, B2S4Distance - 3);
	drawBeast();
	glPopMatrix();

	//done
	glPushMatrix();
	glTranslated(-3, 0.32 + chairY, 1);
	glRotated(chairAngle, 0, 0, 1);
	drawChair();
	glPopMatrix();

	//done
	glPushMatrix();
	glTranslated(-3, 0.2, 2);
	glRotated(teapotAngle, 0, 1, 0);
	drawTeapot();
	glPopMatrix();


	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 27:
		exit(0);
		break;
	case 'p':
	{
		if (!princess1) {
			princess1 = true;
			Resetprincess2 = true;
		}
		else
			Resetprincess1 = true;
	}
	break;
	case '[':
	{
		if (!princess2) {
			princess2 = true;
			Resetprincess1 = true;
		}
		else
			Resetprincess2 = true;
	}
	break;
	case 'b':
	{
		if (!beast1) {
			beast1 = true;
			Resetbeast2 = true;
		}
		else
			Resetbeast1 = true;
	}
	break;
	case 'n':
	{
		if (!beast2) {
			beast2 = true;
			Resetbeast1 = true;
		}
		else
			Resetbeast2 = true;
	}
	break;
	case 'c':
	{
		if (!chair)
			chair = true;
		else
			Resetchair = true;
	}
	break;
	case 't':
	{
		if (!teapot)
			teapot = true;
		else
			Resetteapot = true;
	}
	break;
	case 's':
	{
		if (!sweeper)
			sweeper = true;
		else
			Resetsweeper = true;
	}
	break;
	case 'v':
	{
		if (!cup)
			cup = true;
		else
			Resetcup = true;
	}
	break;
	case 'a':
	{
		if (princess1 || princess2 || beast1 || beast2 || cup || chair || sweeper || teapot) {
			Resetbeast1 = true;
			Resetbeast2 = true;
			Resetprincess1 = true;
			Resetprincess2 = true;
			Resetcup = true;
			Resetchair = true;
			Resetsweeper = true;
			Resetteapot = true;
		}
		else {
			beast1 = true;
			//beast2 = true;
			princess1 = true;
			//princess2 = true;
			cup = true;
			chair = true;
			teapot = true;
			sweeper = true;
		}
	}
	break;
	case '`':
	{
		if (top)
			top = false;
		else {
			top = true;
		}
	}
	break;
	default:
		break;
	}

	glutPostRedisplay();
}


//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");

	// Loading texture files
	tex_ground.Load("Textures/dance2.bmp");
	loadBMP(&tex, "Textures/dance1.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);

	//glutKeyboardFunc(Keyboard);

	glutTimerFunc(0, Timer, 0);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}