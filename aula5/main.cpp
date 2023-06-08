#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;

int time_diff = -1;
float time_angle_diff = (2 * M_PI) / 175;
float time_angle_diff_2 = 360.0f / 175.0f;

float rotation = 0.0f;

int lastTimeStamp = 0;

void tickAnimation() {
	int time = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = time - lastTimeStamp;
	rotation += 0.01f * deltaTime;
	lastTimeStamp = time;
}


void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawTree(float x, float z){
	glPushMatrix();
		glTranslatef(x, 0, z);
		glPushMatrix();
			glColor3f(153.0/255.0, 76.0/255.0, 0.0/255.0);
			glRotatef(-90, 1.0, 0.0, 0.0);
			glutSolidCone(1, 8, 20, 20);
		glPopMatrix();

		glPushMatrix();
			glColor3f(0.0f, 255.0f, 0.0f);
			glRotatef(-90, 1.0, 0.0, 0.0);
			glTranslatef(0, 0, 3);
			glutSolidCone(3, 10, 20, 20);
		glPopMatrix();
	glPopMatrix();
}

void drawCowboys(float radius) {
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		for (float i = 0; i < 8; i++) {
			float angle = -(360 / 8) * i;
			glPushMatrix();
				glRotatef(angle, 0, 1, 0);
				glTranslatef(0, 0, radius);
				glPushMatrix();
					glTranslatef(0, 3, 0);
					glRotatef(-90, 0, 1, 0);
					glutSolidTeapot(2);
				glPopMatrix();
			glPopMatrix();
		}
	glPopMatrix();
}

void drawIndians(float radius) {
	glPushMatrix();
		glColor3f(0.0f, 0.0f, 0.0f);
		for (float i = 0; i < 16; i++) {
			float angle = -(360 / 16) * i;
			glPushMatrix();
				glRotatef(angle, 0, 1, 0);
				glTranslatef(0, 0, radius);
				glPushMatrix();
					glTranslatef(0, 3, 0);
					glRotatef(180, 0, 1, 0);
					glutSolidTeapot(2);
				glPopMatrix();
			glPopMatrix();
		}
	glPopMatrix();
}




void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
	
	// put code to draw scene in here
	
	srand(1);

	for (int i = 0; i < 200; i++) {
		float x = rand();
		x = (x / RAND_MAX) * 200 - 100;

		float z = rand();
		z = (z / RAND_MAX) * 200 - 100;

		if (sqrt(x * x + z * z) < 50) {
			i--;
			continue;
		}

		//glTranslatef(x, 0.0, z);
		drawTree(x,z);
		//glTranslatef(-x, 0.0, -z);
	}
	
	// Torus

	glColor3f(0.0f, 1.0f, 1.0f);

	glutSolidTorus(1, 3, 20, 20);

	//for (int i = 0; i < 8; i++) {

		//Cowboys EU:		
		/*
		float alfa_angle = M_PI_2 / 8;

		glColor3f(1.0f, 1.0f, 1.0f);
		glRotatef(-(360/8) * i, 0.0, 1.0, 0.0);
		glTranslatef(15, 1.75, 0);
		glutSolidTeapot(2);
		glTranslatef(-15, -1.75, 0);
		glRotatef((360/8) * i, 0.0, 1.0, 0.0);
		*/
	//}
	
	//Cowboys
	glPushMatrix();
		glRotatef(-rotation, 0, 1, 0);
		drawCowboys(15);
	glPopMatrix();
	

	/*
	for (int i = 0; i < 16; i++) {

		//Indians

		float alfa_angle = M_PI_2 / 16;

		glColor3f(0.0f, 0.0f, 0.0f);
		glRotatef(-(360/16) * i, 0.0, 1.0, 0.0);
		glTranslatef(35, 1.75, 0);
		glRotatef(-90, 0.0, 1.0, 0.0);
		glutSolidTeapot(2);
		glRotatef(90, 0.0, 1.0, 0.0);
		glTranslatef(-35, -1.75, 0);
		glRotatef((360 / 16) * i, 0.0, 1.0, 0.0);

	}
	*/

	//Indians
	glPushMatrix();
		glRotatef(rotation, 0, 1, 0);
		drawIndians(35);
	glPopMatrix();	

	tickAnimation();
	
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.05; break;

	case GLUT_KEY_LEFT:
		alfa += 0.05; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;
	}
	spherical2Cartesian();
	tickAnimation();
	glutPostRedisplay();
	
}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

	

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	//Função para dar sempre render !!!!
	glutIdleFunc(renderScene);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
