#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alpha_cam = M_PI, beta_cam = M_PI / 6, radius_cam = 10;
float eyeX, eyeY, eyeZ;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
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
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawEsfera(float radius, float height, int slices) {

	float x1, x2, x3, x4, y1, y2, z1, z2, z3, z4, alpha = 2 * M_PI / slices, beta = M_PI / 10;
	glPolygonMode(GL_FRONT, GL_LINE);
	glBegin(GL_TRIANGLES);
	
	for (int j = 0; j < 10; j++) {
		for (int i = 0; i < slices; i++) {
		
			y1 = radius * sin(M_PI_2 - beta * j);
			y2 = radius * sin(M_PI_2 - beta * (j + 1));

			x1 = radius * cos(M_PI_2 - beta * j)       * sin(alpha * i);
			x2 = radius * cos(M_PI_2 - beta * (j + 1)) * sin(alpha * i);
			x3 = radius * cos(M_PI_2 - beta * (j + 1)) * sin(alpha * (i + 1));
			x4 = radius * cos(M_PI_2 - beta * j)       * sin(alpha * (i + 1));

			z1 = radius * cos(M_PI_2 - beta * j)       * cos(alpha * i);
			z2 = radius * cos(M_PI_2 - beta * (j + 1)) * cos(alpha * i);
			z3 = radius * cos(M_PI_2 - beta * (j + 1)) * cos(alpha * (i + 1));
			z4 = radius * cos(M_PI_2 - beta * j)       * cos(alpha * (i + 1));

			glVertex3f(x1, y1, z1);
			glVertex3f(x3, y2, z3);
			glVertex3f(x4, y1, z4);

			glVertex3f(x1, y1, z1);
			glVertex3f(x2, y2, z2);
			glVertex3f(x3, y2, z3);
		}
	}
	glEnd();
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	// put axis drawing in here
	glBegin(GL_LINES);
	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);
	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();


	drawEsfera(1, 2, 100);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

	// put code to process regular keys in here

	//float alpha_mov, beta_mov;
	if (c == 'd') {
		alpha_cam += 0.01;
	}
	else if (c == 'a') {
		alpha_cam -= 0.01;
	}
	else if (c == 'w') {
		beta_cam += 0.01;
		if (beta_cam > M_PI_2)
			beta_cam = M_PI_2;
	}
	else if (c == 's') {
		beta_cam -= 0.01;
		if (beta_cam < -M_PI_2)
			beta_cam = -M_PI_2;
	}
	eyeX = sin(alpha_cam) * cos(beta_cam) * radius_cam;
	eyeY = sin(beta_cam) * radius_cam;
	eyeZ = cos(alpha_cam) * cos(beta_cam) * radius_cam;
	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {

	// put code to process special keys in here

}


int main(int argc, char** argv) {

	eyeX = sin(alpha_cam) * cos(beta_cam) * radius_cam;
	eyeY = sin(beta_cam) * radius_cam;
	eyeZ = cos(alpha_cam) * cos(beta_cam) * radius_cam;

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
