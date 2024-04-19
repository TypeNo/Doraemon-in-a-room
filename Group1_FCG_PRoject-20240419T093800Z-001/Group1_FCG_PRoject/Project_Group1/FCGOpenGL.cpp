#include <Windows.h>
#include <GL/glut.h>
#include <math.h>
#include "imageloader.h"
#include <iostream>
#include <fstream>

static double M_PI = 3.14159;

GLuint doraemon;
//Variable use to do 3D transformation on Doraemon
static int rotateX = 0, rotateY = 0;
static int headRotateX = 0, headRotateY = 0;
static int rightArmRotateX = 0, rightArmRotateY = 0;
static int leftArmRotateX = 0, leftArmRotateY = 0;
static int rightLegRotateX = 0, rightLegRotateY = 0;
static int leftLegRotateX = 0, leftLegRotateY = 0;
static float translateX = 0, translateZ = 0, translateY = 0;
bool turn = true;

//Variable use to control the camera
float angle = 0.0f;
float x2 = 0.0f, lx = 0.0f;
float y2 = 0.0f, ly = 0.0f;
float z2 = 12.0f, lz = -1.0f;
float deltaX = 0, deltaY = 0, deltaRotate = 0;
float deltaMove = 0;


void computePos(float deltaMove, float deltaX, float deltaY) {
	x2 += deltaMove * lx * 0.1f;
	z2 += deltaMove * lz * 0.1f;
	y2 += deltaY * 0.1f;

	x2 += cos(angle) * deltaX;
	z2 += sin(angle) * deltaX;
	std::cout << "x" << x2 << "y" << y2 << "z" << z2 << std::endl;
}

void computeRotate(float deltaRotate) {
	lx = sin(angle += deltaRotate);
	lz = -cos(angle += deltaRotate);
	std::cout << "lx" << lx << "lz" << lz << std::endl;
}

void pressKey(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_UP:
		deltaMove = 0.5f;
		break;
	case GLUT_KEY_DOWN:
		deltaMove = -0.5f;
		break;
	case GLUT_KEY_PAGE_UP: deltaY = 0.05f;  break;
	case GLUT_KEY_PAGE_DOWN: deltaY = -0.05f; break;
	case GLUT_KEY_LEFT:
		deltaRotate = -0.005f;
		break;
	case GLUT_KEY_RIGHT:
		deltaRotate = 0.005f;
		break;
	}
}


void releaseKey(int key, int xx, int yy) {
	switch (key) {
	case GLUT_KEY_UP:
	case GLUT_KEY_DOWN:
		deltaMove = 0;
		break;
	case GLUT_KEY_PAGE_UP:
	case GLUT_KEY_PAGE_DOWN:
		deltaY = 0;
		break;
	case GLUT_KEY_LEFT:
	case GLUT_KEY_RIGHT:
		deltaRotate = 0;
		break;
	}
}


GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);//Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D, // Always GL_TEXTURE_2D
		0, // 0 for now
		GL_RGB, //Format OpenGL uses for image
		image->width, image->height,  // width and height
		0, //The border of the image
		GL_RGB, // GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		image->pixels //The actual pixel data
	);
	return textureId; // return the id of the texture
}

GLuint _textureId1;
GLuint _textureId2;
GLuint _textureId3;
GLuint _textureId4;
GLuint _textureId5;

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	Image* image1 = loadBMP("Texture\\tatami.bmp");
	_textureId1 = loadTexture(image1);
	delete image1;


	Image* image2 = loadBMP("Texture\\pic4.bmp");
	_textureId2 = loadTexture(image2);
	delete image2;

	Image* image3 = loadBMP("Texture\\pic1.bmp");
	_textureId3 = loadTexture(image3);
	delete image3;

	Image* image4 = loadBMP("Texture\\pic2.bmp");
	_textureId4 = loadTexture(image4);
	delete image4;

	Image* image5 = loadBMP("Texture\\pic3.bmp");
	_textureId5 = loadTexture(image5);
	delete image5;
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;


	// Set the viewport to be the entire window
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the correct perspective.
	gluPerspective(45, ratio, 1, 100);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	// Reset Matrix
	glLoadIdentity();
	
}

// Menu variable
static int window;
static int menu_id, hand_id, leg_id, music_id, reset_id;
static int value = 2;

// Animation variable
bool isAnimationComplete = false; // Flag to track animation completion
float animationTime = 0.0f;
float animationDuration = 15.0f; // Total duration of the animation in seconds
bool isAnimationStarted = false;
float Ycount = 0.0f;

bool updateAnimation() {
	if (!isAnimationStarted) {
		x2 -= 30.0f;
		z2 += 15.0f;
		translateY = 5.0f;
		isAnimationStarted = true;
	}

	// Check if animation is complete
	if (animationTime >= animationDuration) {
		// Reset animation variables to their original values
		translateY = 0.0f;
		rotateY = 0.0f;
		Ycount = 0.0f;
		return false;
	}
	// Increment animation time based on the elapsed frame time
	float frameTime = 1.0f / 60.0f; // Assuming a fixed frame rate of 60 FPS
	animationTime += frameTime;
	std::cout << animationTime << std::endl; //Demo purpose
	std::cout << animationDuration << std::endl; //Demo purpose

	float yCountDelta = (5.0f / 15.0f) * frameTime; // Increase Ycount by 3 units in 10 seconds
	float yCountRotate = (1800.0f / 15.0f) * frameTime;
	// Animation logic
	if (animationTime <= 10.0f) {
		// First phase: Move x2
		x2 += (30.0f / 10.0f) * frameTime; // Move x2 by 30 units in 10 seconds
		rotateY += yCountRotate;

		Ycount += yCountDelta;
		translateY -= yCountDelta;

	}
	else if (animationTime <= 15.0f) {
		// Second phase: Move y2
		z2 -= (15.0f / 5.0f) * frameTime; // Move y2 by -15 units in 5 seconds
		rotateY += yCountRotate;

		Ycount += yCountDelta;
		translateY -= yCountDelta;
	}
}


void animate(int value) {
	// Update the animation
	if (animationTime >= animationDuration) {
		isAnimationComplete = true; // Set the flag to indicate animation completion
	}

	if (!isAnimationComplete) {
		// Schedule the next animation frame
		glutTimerFunc(16, animate, 0); // Assuming a target frame rate of 60 FPS (1000ms / 60 = 16.67ms)
		updateAnimation(); //Update Animation
		std::cout << "animate function called" << std::endl; //Demo Purpose

		// Request a redraw
		glutPostRedisplay();
	}
}

void menu(int num) {
	if (num == 1) {
		glutDestroyWindow(window);
		exit(0);
	}
	else if (num >= 2 && num <= 8) { //3D transformation
		value = num;
	}
	if (num == 9) {//Play Music
		PlaySound(TEXT("Music\\Opening.wav"), NULL, SND_ASYNC);
	}
	if (num == 10) { //Stop Music
		PlaySound(NULL, 0, 0);
	}
	if (num == 11) { //Reset Camera
		angle = 0.0f;
		x2 = 0.0f, lx = 0.0f;
		y2 = 0.0f, ly = 0.0f;
		z2 = 12.0f, lz = -1.0f;
		deltaX = 0, deltaY = 0, deltaRotate = 0;
		deltaMove = 0;
	}
	if (num == 12) { //Reset Doraemon
		rotateX = 0, rotateY = 0;
		headRotateX = 0, headRotateY = 0;
		rightArmRotateX = 0, rightArmRotateY = 0;
		leftArmRotateX = 0, leftArmRotateY = 0;
		rightLegRotateX = 0, rightLegRotateY = 0;
		leftLegRotateX = 0, leftLegRotateY = 0;
		translateX = 0, translateZ = 0;
		value = 2;
	}
	if (num == 13) { //Play Animation
		value = 13; // To let text hidden
		x2 = 0.0f;
		z2 = 12.0f;
		animationTime = 0.0f;
		isAnimationStarted = false;
		isAnimationComplete = false;
		PlaySound(TEXT("Music\\Opening.wav"), NULL, SND_ASYNC);
		glutTimerFunc(0, animate, 0);
	}
	glutPostRedisplay();
}

void createMenu(void) {

	hand_id = glutCreateMenu(menu);
	glutAddMenuEntry("Right Hand", 4);
	glutAddMenuEntry("Left Hand", 5);

	leg_id = glutCreateMenu(menu);
	glutAddMenuEntry("Right Leg", 6);
	glutAddMenuEntry("Left Leg", 7);

	music_id = glutCreateMenu(menu);
	glutAddMenuEntry("Play", 9);
	glutAddMenuEntry("Stop", 10);

	reset_id = glutCreateMenu(menu);
	glutAddMenuEntry("Camera", 11);
	glutAddMenuEntry("Doraemon", 12);


	menu_id = glutCreateMenu(menu);
	glutAddSubMenu("Reset", reset_id);
	glutAddMenuEntry("Whole", 2);
	glutAddMenuEntry("Head", 3);
	glutAddSubMenu("Hand", hand_id);
	glutAddSubMenu("Leg", leg_id);
	glutAddMenuEntry("Move", 8);
	glutAddSubMenu("Music", music_id);
	glutAddMenuEntry("Animation", 13);
	glutAddMenuEntry("Quit", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

void renderBitmap(float x, float y, void* font, char* string) {
	char* c;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void displayText() {

	char buffer[100] = { 0 };
	glColor3f(0, 0, 1);
	if (value != 13) {
		sprintf_s(buffer, "------------------");
		renderBitmap(30, 870, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Doraemon Control Guide");
		renderBitmap(30, 850, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "------------------");
		renderBitmap(30, 830, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use Arrow Key, '[', and ']', PageUp, PageDown to control camera");
		renderBitmap(30, 20, GLUT_BITMAP_TIMES_ROMAN_24, buffer);

	}
	if (value == 2) {
		sprintf_s(buffer, "Doraemon Rotation");
		renderBitmap(30, 800, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "--------------");
		renderBitmap(30, 780, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'w' key to rotate the Doraemon upward");
		renderBitmap(30, 750, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 's' key to rotate the Doraemon downward");
		renderBitmap(30, 720, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'd' key to rotate the Doraemon to the right");
		renderBitmap(30, 690, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'a' key to rotate the Doraemon to the left");
		renderBitmap(30, 660, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
	}
	if (value == 3) {
		sprintf_s(buffer, "Head Rotation");
		renderBitmap(30, 800, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "----------");
		renderBitmap(30, 780, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'w' key to rotate the head upward");
		renderBitmap(30, 750, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 's' key to rotate the head downward");
		renderBitmap(30, 720, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'd' key to rotate the head to the right");
		renderBitmap(30, 690, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'a' key to rotate the head to the left");
		renderBitmap(30, 660, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
	}
	if (value == 4 || value == 5) {
		sprintf_s(buffer, "Arm Rotation");
		renderBitmap(30, 800, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "----------");
		renderBitmap(30, 780, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'w' key to rotate the right arm upward");
		renderBitmap(30, 750, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 's' key to rotate the right arm downward");
		renderBitmap(30, 720, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'd' key to rotate the right arm to the right");
		renderBitmap(30, 690, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'a' key to rotate the right arm to the left");
		renderBitmap(30, 660, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
	}
	if (value == 6 || value == 7) {
		sprintf_s(buffer, "Leg Rotation");
		renderBitmap(30, 800, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "----------");
		renderBitmap(30, 780, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'w' key to rotate the right leg upward");
		renderBitmap(30, 750, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 's' key to rotate the right leg downward");
		renderBitmap(30, 720, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
	}
	if (value == 8) {
		sprintf_s(buffer, "Special Movement");
		renderBitmap(30, 800, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "-------------");
		renderBitmap(30, 780, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'w' key to move Doraemon forward");
		renderBitmap(30, 750, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 's' key to move Doraemon backward");
		renderBitmap(30, 720, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'a' key to move Doraemon to the left");
		renderBitmap(30, 690, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
		sprintf_s(buffer, "Use the 'd' key to move Doraemon to the right");
		renderBitmap(30, 660, GLUT_BITMAP_TIMES_ROMAN_24, buffer);

	}
}


//drawHemiSphere
void drawSemiSphere(float radius, int slices, int stacks) {
	for (int i = 0; i < slices; ++i) {
		float theta1 = i * 2.0 * 3.14159 / slices;
		float theta2 = (i + 1) * 2.0 * 3.14159 / slices;

		glBegin(GL_TRIANGLE_STRIP);

		for (int j = 0; j <= stacks; ++j) {
			float phi = j * 3.14159 / (2.0 * stacks);
			float x = radius * cos(phi) * sin(theta1);
			float y = radius * sin(phi);
			float z = radius * cos(phi) * cos(theta1);
			glVertex3f(x, y, z);

			x = radius * cos(phi) * sin(theta2);
			y = radius * sin(phi);
			z = radius * cos(phi) * cos(theta2);
			glVertex3f(x, y, z);
		}
		glEnd();
	}
}

static void init(void)
{
	doraemon = glGenLists(22);

	//Outer Blue Head
	glNewList(doraemon, GL_COMPILE);
	glutSolidSphere(1.0, 100, 64);
	glEndList();

	//Inner White Head
	glNewList(doraemon + 1, GL_COMPILE);
	glutSolidSphere(0.93, 100, 64);
	glEndList();

	//Eye
	glNewList(doraemon + 2, GL_COMPILE);
	glutSolidSphere(0.3, 40, 16);
	glEndList();

	//Eyeball & Nose
	glNewList(doraemon + 3, GL_COMPILE);
	glutSolidSphere(0.1, 40, 16);
	glEndList();

	//Beard connect nose and mouth
	glNewList(doraemon + 4, GL_COMPILE);
	glutSolidSphere(1.0, 100, 64);
	glEndList();

	//Beard
	glNewList(doraemon + 5, GL_COMPILE);
	drawSemiSphere(0.5, 50, 100);
	glEndList();

	//Mouth
	glNewList(doraemon + 6, GL_COMPILE);
	glScalef(0.60, 0.40, 0.35);
	glRotatef(185, 1.0, 0.0, 0.0);
	drawSemiSphere(1.0, 50, 100);
	glEndList();

	//Blue Belly
	glNewList(doraemon + 7, GL_COMPILE);
	glutSolidSphere(0.8f, 100, 64);
	glEndList();

	//White Belly
	glNewList(doraemon + 8, GL_COMPILE);
	glutSolidSphere(0.73f, 100, 64);
	glEndList();

	//Torus
	glNewList(doraemon + 9, GL_COMPILE);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidTorus(0.1f, 0.5f, 50, 50);
	glEndList();

	//Pocket
	glNewList(doraemon + 10, GL_COMPILE);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	drawSemiSphere(0.45, 50, 50);
	glEndList();

	//Leg (Cylinder)
	glNewList(doraemon + 11, GL_COMPILE);
	GLUquadricObj* cylinder = gluNewQuadric();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	gluCylinder(cylinder, 0.3f, 0.3f, 0.6f, 50, 50);
	glEndList();

	//Leg (Sphere)
	glNewList(doraemon + 12, GL_COMPILE);
	glScalef(0.75, 0.4, 0.75);
	glutSolidSphere(0.5f, 100.0f, 64.0f);
	glEndList();

	GLUquadric* disk = gluNewQuadric();
	//Right Hand (Cylinder)
	glNewList(doraemon + 13, GL_COMPILE);
	GLUquadricObj* cylinder2 = gluNewQuadric();
	glRotatef(90.0f, 0.0f, -1.0f, 0.0f);
	gluCylinder(cylinder2, 0.25f, 0.17f, 0.75f, 50, 50);
	gluDisk(disk, 0.0f, 0.25f, 50, 1);
	glEndList();

	//Left Hand (Cylinder)
	glNewList(doraemon + 14, GL_COMPILE);
	GLUquadricObj* cylinder3 = gluNewQuadric();
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	gluCylinder(cylinder3, 0.25f, 0.17f, 0.75f, 50, 50);
	gluDisk(disk, 0.0f, 0.25f, 50, 1);
	glEndList();

	//Hand (Sphere)
	glNewList(doraemon + 15, GL_COMPILE);
	glutSolidSphere(0.2f, 100.0f, 64.0f);
	glEndList();

	//Ring (Sphere)
	glNewList(doraemon + 16, GL_COMPILE);
	glutSolidSphere(0.15f, 100.0f, 64.0f);
	glEndList();

	//Ring (Torus)
	glNewList(doraemon + 17, GL_COMPILE);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidTorus(0.04f, 0.13f, 50, 50);
	glEndList();

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 0.0);

}

void drawCube() {
	float cubeSize = 1.0f;
	float halfCubeSize = cubeSize / 2.0f;

	glPushMatrix();

		// Back face
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glBindTexture(GL_TEXTURE_2D, _textureId3);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBegin(GL_QUADS);
		//glColor3f(1.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-halfCubeSize, -halfCubeSize, -halfCubeSize);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(halfCubeSize, -halfCubeSize, -halfCubeSize);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(halfCubeSize, halfCubeSize, -halfCubeSize);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-halfCubeSize, halfCubeSize, -halfCubeSize);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// Top face
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glBindTexture(GL_TEXTURE_2D, _textureId2);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-halfCubeSize, halfCubeSize, -halfCubeSize);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(halfCubeSize, halfCubeSize, -halfCubeSize);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(halfCubeSize, halfCubeSize, halfCubeSize);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-halfCubeSize, halfCubeSize, halfCubeSize);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// Bottom face
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glBindTexture(GL_TEXTURE_2D, _textureId1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-halfCubeSize, -halfCubeSize, -halfCubeSize);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(halfCubeSize, -halfCubeSize, -halfCubeSize);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(halfCubeSize, -halfCubeSize, halfCubeSize);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-halfCubeSize, -halfCubeSize, halfCubeSize);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// Left face
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glBindTexture(GL_TEXTURE_2D, _textureId4);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBegin(GL_QUADS);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-halfCubeSize, -halfCubeSize, -halfCubeSize);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-halfCubeSize, halfCubeSize, -halfCubeSize);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-halfCubeSize, halfCubeSize, halfCubeSize);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-halfCubeSize, -halfCubeSize, halfCubeSize);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

		// Right face
		glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glBindTexture(GL_TEXTURE_2D, _textureId5);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(halfCubeSize, -halfCubeSize, -halfCubeSize);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(halfCubeSize, halfCubeSize, -halfCubeSize);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(halfCubeSize, halfCubeSize, halfCubeSize);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(halfCubeSize, -halfCubeSize, halfCubeSize);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glPopMatrix();

	glPopMatrix();
}

void drawCube2() {
	float cubeSize = 1.0f;
	float halfCubeSize = cubeSize / 2.0f;

	glPushMatrix();
	// Front face
	glBegin(GL_QUADS);
	glVertex3f(-halfCubeSize, -halfCubeSize, halfCubeSize);
	glVertex3f(halfCubeSize, -halfCubeSize, halfCubeSize);
	glVertex3f(halfCubeSize, halfCubeSize, halfCubeSize);
	glVertex3f(-halfCubeSize, halfCubeSize, halfCubeSize);
	glEnd();

	// Back face
	glBegin(GL_QUADS);
	glVertex3f(-halfCubeSize, -halfCubeSize, -halfCubeSize);
	glVertex3f(halfCubeSize, -halfCubeSize, -halfCubeSize);
	glVertex3f(halfCubeSize, halfCubeSize, -halfCubeSize);
	glVertex3f(-halfCubeSize, halfCubeSize, -halfCubeSize);
	glEnd();

	// Top face
	glBegin(GL_QUADS);
	glVertex3f(-halfCubeSize, halfCubeSize, -halfCubeSize);
	glVertex3f(halfCubeSize, halfCubeSize, -halfCubeSize);
	glVertex3f(halfCubeSize, halfCubeSize, halfCubeSize);
	glVertex3f(-halfCubeSize, halfCubeSize, halfCubeSize);
	glEnd();

	// Bottom face
	glBegin(GL_QUADS);
	glVertex3f(-halfCubeSize, -halfCubeSize, -halfCubeSize);
	glVertex3f(halfCubeSize, -halfCubeSize, -halfCubeSize);
	glVertex3f(halfCubeSize, -halfCubeSize, halfCubeSize);
	glVertex3f(-halfCubeSize, -halfCubeSize, halfCubeSize);
	glEnd();

	// Left face
	glBegin(GL_QUADS);
	glVertex3f(-halfCubeSize, -halfCubeSize, -halfCubeSize);
	glVertex3f(-halfCubeSize, halfCubeSize, -halfCubeSize);
	glVertex3f(-halfCubeSize, halfCubeSize, halfCubeSize);
	glVertex3f(-halfCubeSize, -halfCubeSize, halfCubeSize);
	glEnd();

	// Right face
	glBegin(GL_QUADS);
	glVertex3f(halfCubeSize, -halfCubeSize, -halfCubeSize);
	glVertex3f(halfCubeSize, halfCubeSize, -halfCubeSize);
	glVertex3f(halfCubeSize, halfCubeSize, halfCubeSize);
	glVertex3f(halfCubeSize, -halfCubeSize, halfCubeSize);
	glEnd();

	glPopMatrix();
}

void drawTable() {
	glColor3f(0.8f, 0.6f, 0.4f);
	//leg1 //Left Inside
	glPushMatrix();
	glTranslatef(-3.9f, -1.0f, -0.35f);
	glScalef(0.25f, 2.0f, 0.2f);
	drawCube2();
	glPopMatrix();

	//leg2 // Left Outside
	glPushMatrix();
	glTranslatef(-3.9f, -1.0f, 0.35f);
	glScalef(0.25f, 2.0f, 0.2f);
	drawCube2();
	glPopMatrix();

	//leg3 //Right Inside
	glPushMatrix();
	glTranslatef(-1.15f, -1.0f, -0.35f);
	glScalef(0.25f, 2.0f, 0.2f);
	drawCube2();
	glPopMatrix();
	//leg4 //Right Outside
	glPushMatrix();
	glTranslatef(-1.15f, -1.0f, 0.35f);
	glScalef(0.25f, 2.0f, 0.2f);
	drawCube2();
	glPopMatrix();
	//table
	glPushMatrix();
	glColor3f(0.4f, 0.2f, 0.0f);
	glTranslatef(-2.5f, 0.0f, 0.0f);
	glScalef(3.0f, 0.1f, 1.0f);
	drawCube2();
	glPopMatrix();
}


void display(void) {
	if (deltaMove || deltaX || deltaY)
		computePos(deltaMove, deltaX, deltaY);

	if (deltaRotate) {
		computeRotate(deltaRotate);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(x2, y2, z2,
		x2 + lx, y2, z2 + lz,
		0.0f, 1.0f, 0.0f);

	//Add ambient light
	GLfloat ambientColor[] = { 0.6f, 0.6f, 0.6f, 1.0f }; // Color 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightColor0[] = { 0.9f, 0.9f, 0.9f, 1.0f }; // Color 
	GLfloat lightPos0[] = { -8.0f, 2.5f, 2.5f, 1.0f }; // Positioned 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.6f, 0.6f, 0.6f, 1.0f }; // Color 

	////Coming from the direction 
	GLfloat lightPos1[] = { 0.0f, 3.5f, 0.0f, 0.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//glOrtho(0, 1400, 0, 900, 0.0f, 1.0);
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();

	//// Render your text here using `renderBitmap` function
	//displayText();

	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45, 1400 / 900, 1, 100);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	// Continue rendering the rest of the scene


	//table	
	glPushMatrix();
	glTranslatef(-0.5, -0.75, 0.1);
	drawTable();
	glPopMatrix();


	//Cube
	glPushMatrix();
	glTranslatef(0, 0.9, 4.7);
	glScalef(14.0, 7.0, 18.0);
	drawCube();
	glPopMatrix();

	glPushMatrix();

		glTranslatef(0.0f, 0.0f, (GLfloat)translateZ);
		glTranslatef((GLfloat)translateX, 0.0f, 0.0f);
		glTranslatef((GLfloat)0.0f, translateY, 0.0f);
		glRotatef((GLfloat)rotateY, 0.0, 1.0, 0.0);
		glRotatef((GLfloat)rotateX, 1.0, 0.0, 0.0);

		//Body		
		glPushMatrix();

			//Blue Belly
			glPushMatrix();
			glColor3f(81.0f / 255.0f, 161.0f / 255.0f, 196.0f / 255.0f);
			glTranslatef(0.0, -1.5f, 0.0f);
			glCallList(doraemon + 7);
			glPopMatrix();

			//White Belly
			glPushMatrix();
			glColor3f(0.96, 0.96, 0.96);
			glTranslatef(0.0, -1.52, 0.12f);
			glCallList(doraemon + 8);
			glPopMatrix();

			//necklace
			glPushMatrix();
			glColor3f(1.0f, 0.0f, 0.0f);
			glTranslatef(0.0, -0.88, 0.03f);
			glCallList(doraemon + 9);
			glPopMatrix();

			//pocket
			glPushMatrix();
			glColor3f(0.9, 0.90, 0.90);
			glTranslatef(0.0, -1.55, 0.50f);
			glCallList(doraemon + 10);
			glPopMatrix();

			//Necklace Ring1
			glPushMatrix();
			glColor3f(0.94, 0.94, 0.5);
			glTranslatef(0.0f, -0.96, 0.75f);
			glCallList(doraemon + 16);
			glPopMatrix();

			//Necklace Ring2
			glPushMatrix();
			glColor3f(0.85, 0.85, 0.3);
			glTranslatef(0.0f, -0.93, 0.75f);
			glCallList(doraemon + 17);
			glPopMatrix();

		glPopMatrix();

		//Head
		glPushMatrix();

			glRotatef((GLfloat)headRotateX, 1.0f, 0.0f, 0.0f);
			glRotatef((GLfloat)headRotateY, 0.0f, 1.0f, 0.0f);

			//Outer Blue Head
			glPushMatrix();
			glColor3f(81.0f / 255.0f, 161.0f / 255.0f, 196.0f / 255.0f);
			glCallList(doraemon);
			glPopMatrix();

			//Inner White Head
			glPushMatrix();
			glColor3f(0.96, 0.96, 0.96);
			glTranslatef(0, -0.07, 0.12);
			glCallList(doraemon + 1);
			glPopMatrix();

			//Eye
			glPushMatrix();
				//Right Eye
				glPushMatrix();
					//Right Eye White
					glPushMatrix();
					glColor3f(1.0, 1.0, 1.0);
					glTranslatef(0.18, 0.3, 0.7);
					glCallList(doraemon + 2);
					glPopMatrix();

					//Right Eyeballs
					glPushMatrix();
					glColor3f(0.0, 0.0, 0.0);
					glTranslatef(0.15, 0.35, 0.90);
					glCallList(doraemon + 3);
					glPopMatrix();
				glPopMatrix();
				//Left Eye
				glPushMatrix();
					//Left Eye White
					glPushMatrix();
					glColor3f(1.0, 1.0, 1.0);
					glTranslatef(-0.18, 0.3, 0.7);
					glCallList(doraemon + 2);
					glPopMatrix();

					//Left Eyeballs
					glPushMatrix();
					glColor3f(0.0, 0.0, 0.0);
					glTranslatef(-0.15, 0.35, 0.90);
					glCallList(doraemon + 3);
					glPopMatrix();
				glPopMatrix();
			glPopMatrix();

			//Nose
			glPushMatrix();
			glColor3f(1.0, 0.0, 0.0);
			glTranslatef(0.0, 0.20, 1.0);
			glCallList(doraemon + 3);
			glPopMatrix();

			//Bread connect nose and mouth
			glPushMatrix();
			glColor3f(0.83, 0.77, 0.69);
			glTranslatef(0.0, -0.07, 0.9525);
			glScalef(0.04, 0.3, 0.1);
			glCallList(doraemon + 4);
			glPopMatrix();

			//Beard1
			glPushMatrix();
			glColor3f(0, 0, 0);
			glTranslatef(0.0, -0.07, 0.9525);
			glRotatef(15, 0, 0, 1);
			glScalef(1.5, 0.025, 0.025);
			glCallList(doraemon + 5);
			glPopMatrix();

			//Beard2
			glPushMatrix();
			glColor3f(0, 0, 0);
			glTranslatef(0.0, -0.07, 0.9525);
			glRotatef(-15, 0, 0, 1);
			glScalef(1.5, 0.025, 0.025);
			glCallList(doraemon + 5);
			glPopMatrix();

			//Beard3
			glPushMatrix();
			glColor3f(0, 0, 0);
			glTranslatef(0.0, -0.07, 0.9525);
			glScalef(1.5, 0.025, 0.025);
			glCallList(doraemon + 5);
			glPopMatrix();

			//Mouth
			glPushMatrix();
			glColor3f(0.7372, 0.3567, 0.2980);
			glTranslatef(0, -0.3, 0.67);
			glCallList(doraemon + 6);
			glPopMatrix();

		glPopMatrix();

		glPushMatrix();

			//Right Hand Arm
			glPushMatrix();
			glColor3f(81.0f / 255.0f, 161.0f / 255.0f, 196.0f / 255.0f);
			glTranslatef(-0.60f, -1.2, 0.0f);
			glRotatef((GLfloat)rightArmRotateX, 1.0, 0.0, 0.0);
			glRotatef((GLfloat)rightArmRotateY, 0.0, 1.0, 0.0);
			glCallList(doraemon + 13);
			glPopMatrix();

			//Right Shoulder Circle
			glPushMatrix();
			glColor3f(81.0f / 255.0f, 161.0f / 255.0f, 196.0f / 255.0f);
			glTranslatef(-0.63f, -1.2f, 0.0f);
			glScalef(1.1, 1.2, 1.2);
			glRotatef((GLfloat)rightLegRotateX, 1.0, 0.0, 0.0);
			glCallList(doraemon + 15);
			glPopMatrix();

			//Right Hand Palm
			glPushMatrix();
			glColor3f(0.96, 0.96, 0.96);
			glTranslatef(-0.60f, -1.2, 0.0f);
			glRotatef((GLfloat)rightArmRotateX, 1.0, 0.0, 0.0);
			glRotatef((GLfloat)rightArmRotateY, 0.0, 1.0, 0.0);
			glTranslatef(-0.70f, 0.0, 0.0f);
			glCallList(doraemon + 15);
			glPopMatrix();

		glPopMatrix();

		glPushMatrix();

			//Left Hand Arm
			glPushMatrix();
			glColor3f(81.0f / 255.0f, 161.0f / 255.0f, 196.0f / 255.0f);
			glTranslatef(0.60f, -1.2, 0.0f);
			glRotatef((GLfloat)leftArmRotateX, 1.0, 0.0, 0.0);
			glRotatef((GLfloat)leftArmRotateY, 0.0, 1.0, 0.0);
			glCallList(doraemon + 14);
			glPopMatrix();

			//Left Shoulder Circle
			glPushMatrix();
			glColor3f(81.0f / 255.0f, 161.0f / 255.0f, 196.0f / 255.0f);
			glTranslatef(0.63f, -1.2f, 0.0f);
			glScalef(1.1, 1.2, 1.2);
			glRotatef((GLfloat)rightLegRotateX, 1.0, 0.0, 0.0);
			glCallList(doraemon + 15);
			glPopMatrix();

			//Left Hand Palm
			glPushMatrix();
			glColor3f(0.96, 0.96, 0.96);
			glTranslatef(0.60f, -1.2, 0.0f);
			glRotatef((GLfloat)leftArmRotateX, 1.0, 0.0, 0.0);
			glRotatef((GLfloat)leftArmRotateY, 0.0, 1.0, 0.0);
			glTranslatef(0.70f, 0.0, 0.0f);
			glCallList(doraemon + 15);
			glPopMatrix();

		glPopMatrix();


		glPushMatrix();

			//Right Leg Thigh
			glPushMatrix();
			glColor3f(81.0f / 255.0f, 161.0f / 255.0f, 196.0f / 255.0f);
			glTranslatef(-0.40f, -1.8f, 0.0f);
			glRotatef((GLfloat)rightLegRotateX, 1.0, 0.0, 0.0);
			glCallList(doraemon + 11);
			glPopMatrix();

			//Right sole
			glPushMatrix();
			glColor3f(0.96, 0.96, 0.96);
			glTranslatef(-0.40f, -1.8f, 0.0f);
			glRotatef((GLfloat)rightLegRotateX, 1.0, 0.0, 0.0);
			glTranslatef(0.0f, -0.7f, 0.0f);
			glCallList(doraemon + 12);
			glPopMatrix();

		glPopMatrix();

		glPushMatrix();

			//Left Leg Thigh
			glPushMatrix();
			glColor3f(81.0f / 255.0f, 161.0f / 255.0f, 196.0f / 255.0f);
			glTranslatef(0.40f, -1.8f, 0.0f);
			glRotatef((GLfloat)leftLegRotateX, 1.0, 0.0, 0.0);
			glCallList(doraemon + 11);
			glPopMatrix();

			//Left sole
			glPushMatrix();
			glColor3f(0.96, 0.96, 0.96);
			glTranslatef(0.40f, -1.8f, 0.0f);
			glRotatef((GLfloat)leftLegRotateX, 1.0, 0.0, 0.0);
			glTranslatef(0.0f, -0.7f, 0.0f);
			glCallList(doraemon + 12);
			glPopMatrix();

		glPopMatrix();

	glPopMatrix();


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1400, 0, 900, 0.0f, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	
	displayText();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, float(1400.0 / 900.0), 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutSwapBuffers();

}

void moveMotion() {
	if (turn == true) {
		if (rightLegRotateX > -30) {
			rightLegRotateX -= 5;
			leftLegRotateX += 5;
		}
		if (rightLegRotateX == -30) turn = false;
	}
	if (turn == false) {
		if (rightLegRotateX <= 30) {
			rightLegRotateX += 5;
			leftLegRotateX -= 5;
		}
		if (rightLegRotateX == 30) turn = true;
	}
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '[':
		deltaX = -0.05f;
		break;
	case ']':
		deltaX = 0.05f;
		break;
	}
	if (value == 2) {
		switch (key) {
		case 'w':
			rotateX = (rotateX - 5) % 360;
			glutPostRedisplay();
			break;
		case 's':
			rotateX = (rotateX + 5) % 360;
			glutPostRedisplay();
			break;
		case 'd':
			rotateY = (rotateY + 5) % 360;
			glutPostRedisplay();
			break;
		case 'a':
			rotateY = (rotateY - 5) % 360;
			glutPostRedisplay();
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
		}
	}
	if (value == 3) {
		switch (key) {
		case 'w':
			if (headRotateX >= -10)
				headRotateX = (headRotateX - 5);
			glutPostRedisplay();
			break;
		case 's':
			if (headRotateX <= 15)
				headRotateX = (headRotateX + 5);
			glutPostRedisplay();
			break;
		case 'd':
			if (headRotateY <= 30)
				headRotateY = (headRotateY + 5);
			glutPostRedisplay();
			break;
		case 'a':
			if (headRotateY >= -30)
				headRotateY = (headRotateY - 5);
			glutPostRedisplay();
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
		}
	}
	if (value == 4) {
		switch (key) {
		case 'd':
			if (rightArmRotateY <= 40)
				rightArmRotateY = (rightArmRotateY + 5);
			glutPostRedisplay();
			break;
		case 'a':
			if (rightArmRotateY >= -40)
				rightArmRotateY = (rightArmRotateY - 5);
			glutPostRedisplay();
			break;
		case 's':
			if (rightArmRotateX <= 90)
				rightArmRotateX = (rightArmRotateX + 5);
			glutPostRedisplay();
			break;
		case 'w':
			if (rightArmRotateX >= -90)
				rightArmRotateX = (rightArmRotateX - 5);
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}
	if (value == 5) {
		switch (key) {
		case 'd':
			if (leftArmRotateY <= 40)
				leftArmRotateY = (leftArmRotateY + 5);
			glutPostRedisplay();
			break;
		case 'a':
			if (leftArmRotateY >= -40)
				leftArmRotateY = (leftArmRotateY - 5);
			glutPostRedisplay();
			break;
		case 's':
			if (leftArmRotateX <= 90)
				leftArmRotateX = (leftArmRotateX + 5);
			glutPostRedisplay();
			break;
		case 'w':
			if (leftArmRotateX >= -90)
				leftArmRotateX = (leftArmRotateX - 5);
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}
	if (value == 6) {
		switch (key) {
		case 's':
			if (rightLegRotateX <= 30)
				rightLegRotateX = (rightLegRotateX + 5);
			glutPostRedisplay();
			break;
		case 'w':
			if (rightLegRotateX >= -30)
				rightLegRotateX = (rightLegRotateX - 5);
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}
	if (value == 7) {
		switch (key) {
		case 's':
			if (leftLegRotateX <= 30)
				leftLegRotateX = (leftLegRotateX + 5);
			glutPostRedisplay();
			break;
		case 'w':
			if (leftLegRotateX >= -30)
				leftLegRotateX = (leftLegRotateX - 5);
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}

	if (value == 8) {
		switch (key) {
		case 's':
			rotateY = 0;
			translateZ += 0.1;
			moveMotion();
			glutPostRedisplay();
			break;
		case 'w':
			rotateY = 180;
			translateZ -= 0.1;
			moveMotion();
			glutPostRedisplay();
			break;
		case 'a':
			rotateY = -90;
			translateX -= 0.1;
			moveMotion();
			glutPostRedisplay();
			break;
		case 'd':
			rotateY = 90;
			translateX += 0.1;
			moveMotion();
			glutPostRedisplay();
			break;
		default:
			break;
		}
	}
}

void keyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case '[':
	case ']':
		deltaX = 0;
		break;
	}
}

int main(int argc, char** argv) {
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 0);
	glutInitWindowSize(1400, 900);
	glutCreateWindow("Group Project");

	init();
	initRendering();
	createMenu();
	// register callbacks
	glutReshapeFunc(changeSize);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);

	glutIdleFunc(display);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);


	glEnable(GL_DEPTH_TEST);
	// enter GLUT event processing loop
	glutMainLoop();

	return 1;
}