

#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

float camX = 00, camY = 3, camZ = -10;
float viewX = 0.0, viewZ = 1.0;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

GLuint* vertices;
double vertexCount;
GLuint buffers[1];

unsigned int t, tw, th;
unsigned char* imageData;
std::vector<float> vertexB;



float rotation = 0.0f;

int lastTimeStamp = 0;

void tickAnimation() {
	int time = glutGet(GLUT_ELAPSED_TIME);
	int deltaTime = time - lastTimeStamp;
	rotation += 0.01f * deltaTime;
	lastTimeStamp = time;
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

double h(int i, int j) {
	//double pixel = imageData[i * tw + j];
	//return pixel / 255 * 30;
	return (imageData[i * tw + j] * 100) / 255;
}

float hf(float x, float z) {
	x += th / 2;
	z += tw / 2;
	float x1 = floor(x);
	float x2 = ceil(x);
	float z1 = floor(z);
	float z2 = ceil(z);
	float fz = z - z1;
	float h_x1_z = h(x1, z1) * (1 - fz) + h(x1, z2) * fz;
	float h_x2_z = h(x2, z1) * (1 - fz) + h(x2, z2) * fz;
	float fx = x - x1;
	float height_xz = h_x1_z * (1 - fx) + h_x2_z * fx;

	return height_xz;
}

void drawTerrain() {
	// colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPSº
	// p06.detailed.pdf 
	glColor3f(85.0f/255, 107.0f/255, 47.0f/255);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	for (int i = 0; i < th - 1; i++) {
		glDrawArrays(GL_TRIANGLE_STRIP, (tw) * 2 * i, (tw) * 2);
	}
}


void drawTree(float x, float z) {
	glPushMatrix();
	glTranslatef(x, hf(x,z), z);
	glPushMatrix();
	glColor3f(153.0 / 255.0, 76.0 / 255.0, 0.0 / 255.0);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glutSolidCone(1, 8, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f, 255.0f, 0.0f);
	glRotatef(-90, 1.0, 0.0, 0.0);
	glTranslatef(0, hf(0,3), 3);
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

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(135.0f/255,206.0f/255,235.0f/255,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      viewX, camY, viewZ,
			  0.0f,1.0f,0.0f);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	drawTerrain();

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

		drawTree(x, z);
	}

	// Torus

	glColor3f(0.0f, 1.0f, 1.0f);

	glutSolidTorus(1, 3, 20, 20);

	//Cowboys
	glPushMatrix();
	glRotatef(-rotation, 0, 1, 0);
	drawCowboys(15);
	glPopMatrix();


	//Indians
	glPushMatrix();
	glRotatef(rotation, 0, 1, 0);
	drawIndians(35);
	glPopMatrix();

	tickAnimation();

// End of frame
	glutSwapBuffers();
}



void processSpecialKeys(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_UP:
		camZ += 1.5f;
		//viewX -= 1;
		viewZ += 3;
		
		break;

	case GLUT_KEY_DOWN:
		camZ -= 1.5f;
		break;

	case GLUT_KEY_RIGHT:
		camX -= 1.5f;
		viewX -= 1.5;
		break;

	case GLUT_KEY_LEFT:
		camX += 1.5f;
		viewX += 1.5;
		break;
	}
	camY = hf(camX, camZ) + 3;
	//spherical2Cartesian();
	tickAnimation();
	glutPostRedisplay();
}



void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}

void init() {

// 	Load the height map "terreno.jpg"

	ilGenImages(1, &t);
	ilBindImage(t);
	// terreno.jpg is the image containing our height map
	ilLoadImage((ILstring)"terreno.jpg");
	// convert the image to single channel per pixel
	// with values ranging between 0 and 255
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	// important: check tw and th values
	// both should be equal to 256
	// if not there was an error loading the image
	// most likely the image could not be found
	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	// imageData is a LINEAR array with the pixel values
	imageData = ilGetData();

// 	Build the vertex arrays
	vertices = (GLuint*)calloc(th - 1, sizeof(GLuint));
	glGenBuffers(1, buffers);

	int halfH = th / 2;
	int halfW = tw / 2;

	vertexCount = 2 * tw;

	for (int i = 0; i < th - 1; i++) {
		std::vector<double> strip;
		for (int j = 0; j < tw; j++) {
			vertexB.push_back(i + 1 - halfH);
			vertexB.push_back(h(i + 1, j));
			vertexB.push_back(j - halfW);

			vertexB.push_back(i - halfH);
			vertexB.push_back(h(i, j));
			vertexB.push_back(j - halfW);

		}

	}
	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, vertexB.size() * sizeof(float), vertexB.data(), GL_STATIC_DRAW);


	// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	//glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	//glutMouseFunc(processMouseButtons);
	//glutMotionFunc(processMouseMotion);

	glewInit();
	glEnableClientState(GL_VERTEX_ARRAY);

	ilInit();
	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

