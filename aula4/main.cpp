#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

bool optimized = false;
int timebase = glutGet(GLUT_ELAPSED_TIME);
float frames, time_passed, fps;
char fps_buffer[100];

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;
float* vertexB;

GLuint vertices, verticeCount, body, top;

std::vector<float> vertice_array;

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


/*----------------------------------------------------------------------------------- 
	Draw Cylinder with strips and fans

	  parameters: radius, height, sides

-----------------------------------------------------------------------------------*/

void cylinder0(float radius, float height, int sides) {
	int i;
	float step;

	step = 360.0 / sides;


	//top
	// glVertex3f(0,height*0.5,0);
	vertice_array.push_back(0);
	vertice_array.push_back(0.5 * height);
	vertice_array.push_back(0);
	for (i = 0; i <= sides; i++) {
		// glVertex3f(cos(i * step * M_PI/180.0)*radius,height*0.5,-sin(i * step *M_PI/180.0)*radius);
		vertice_array.push_back(cos(i * step * M_PI / 180.0) * radius);
		vertice_array.push_back(height * 0.5);
		vertice_array.push_back(-sin(i * step * M_PI / 180.0) * radius);
	}
	top = 3 * (sides);

	// base
	// glVertex3f(0,-height*0.5,0);
	vertice_array.push_back(0);
	vertice_array.push_back(-0.5 * height);
	vertice_array.push_back(0);
	for (i = 0; i <= sides; i++) {
		// glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,sin(i * step *M_PI/180.0)*radius);
		vertice_array.push_back(cos(i * step * M_PI / 180.0) * radius);
		vertice_array.push_back(-0.5 * height);
		vertice_array.push_back(sin(i * step * M_PI / 180.0) * radius);
	}

	// body
	for (i = 0; i <= sides; i++) {
		// glVertex3f(cos(i * step * M_PI/180.0)*radius, height*0.5,-sin(i * step *M_PI/180.0)*radius);
		vertice_array.push_back(cos(i * step * M_PI / 180.0) * radius);
		vertice_array.push_back(height * 0.5);
		vertice_array.push_back(-sin(i * step * M_PI / 180.0) * radius);
		// glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,-sin(i * step *M_PI/180.0)*radius);
		vertice_array.push_back(cos(i * step * M_PI / 180.0) * radius);
		vertice_array.push_back(-height * 0.5);
		vertice_array.push_back(-sin(i * step * M_PI / 180.0) * radius);
	}
	body = 6 * sides;
}

/*-----------------------------------------------------------------------------------
	Draw Cylinder
		parameters: radius, height, sides
-----------------------------------------------------------------------------------*/

void cylinder(float radius, float height, int sides) {
	int i;
	float step;

	step = 360.0 / sides;

	// top
	for (i = 0; i < sides; i++) {
		// glVertex3f(0, height*0.5, 0);
		vertice_array.push_back(0);
		vertice_array.push_back(0.5 * height);
		vertice_array.push_back(0);
		// glVertex3f(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos(i * step * M_PI / 180.0) * radius);
		vertice_array.push_back(height * 0.5);
		vertice_array.push_back(-sin(i * step * M_PI / 180.0) * radius);
		// glVertex3f(cos((i+1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos((i + 1) * step * M_PI / 180.0) * radius);
		vertice_array.push_back(height * 0.5);
		vertice_array.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);
	}

	// bottom
	for (i = 0; i < sides; i++) {
		// glVertex3f(0, -height*0.5, 0);
		vertice_array.push_back(0);
		vertice_array.push_back(-height * 0.5);
		vertice_array.push_back(0);
		// glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos((i + 1) * step * M_PI / 180.0) * radius);
		vertice_array.push_back(-height * 0.5);
		vertice_array.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);
		// glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos(i * step * M_PI / 180.0) * radius);
		vertice_array.push_back(-height * 0.5);
		vertice_array.push_back(-sin(i * step * M_PI / 180.0) * radius);
	}

	// body
	for (i = 0; i <= sides; i++) {

		// glVertex3f(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos(i * step * M_PI / 180.0) * radius);
		vertice_array.push_back(height * 0.5);
		vertice_array.push_back(-sin(i * step * M_PI / 180.0) * radius);
		// glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos(i * step * M_PI / 180.0) * radius);
		vertice_array.push_back(-height * 0.5);
		vertice_array.push_back(-sin(i * step * M_PI / 180.0) * radius);
		// glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos((i + 1) * step * M_PI / 180.0) * radius);
		vertice_array.push_back(height * 0.5);
		vertice_array.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);

		// glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos(i * step * M_PI / 180.0) * radius);
		vertice_array.push_back(-height * 0.5);
		vertice_array.push_back(-sin(i * step * M_PI / 180.0) * radius);
		// glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos((i + 1) * step * M_PI / 180.0) * radius);
		vertice_array.push_back(-height * 0.5);
		vertice_array.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);
		// glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
		vertice_array.push_back(cos((i + 1) * step * M_PI / 180.0) * radius);
		vertice_array.push_back(height * 0.5);
		vertice_array.push_back(-sin((i + 1) * step * M_PI / 180.0) * radius);
	}
	// glEnd();
}



void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, verticeCount);

	frames++;
	time_passed = glutGet(GLUT_ELAPSED_TIME);
	if (time_passed - timebase > 1000) {
		fps = frames * 1000.0 / (time_passed - timebase);
		timebase = time_passed;
		frames = 0;
	}

	sprintf(fps_buffer, "%d", (int)fps);

	glutSetWindowTitle(fps_buffer);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

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

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Page Up and Page Down control the distance from the camera to the origin");
}


void prepare_data() {

	cylinder(1, 2, 10);

	verticeCount = vertice_array.size() / 3;

	glGenBuffers(1, &vertices);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);

	glBufferData(
		GL_ARRAY_BUFFER, // tipo do buffer, s� � relevante na altura do desenho
		sizeof(float) * vertice_array.size(), // tamanho do vector em bytes
		vertice_array.data(), // os dados do array associado ao vector
		GL_STATIC_DRAW); // indicativo da utiliza��o (est�tico e para desenho)
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
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	// init GLEW
#ifndef __APPLE__
	glewInit();
#endif

	prepare_data();


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);
	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
