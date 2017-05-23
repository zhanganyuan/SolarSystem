#include <iostream>
#include "Sun.h"
#include "TextrueHandler.h"
#include "Camera.h"
#include "Globals.h"

float starSizeScale = 0.000005f;
char* renderMode = "Wire";
static int buttonType;
// the screen size
int screenWidth, screenHeight;

// toggles if the help dialogue is enabled
bool helpDialogue = false;
// toggles if orbits are drawn
bool showOrbits = true;
// holds the index of the last planet that was selected with the 1 to 9 number keys
int planetSelected = 1;

TextrueHandler* tex_help;
TextrueHandler* tex_stars;
TextrueHandler* tex_moon;

// These control the simulation of time
double time;
double timeSpeed;

// holds the state of the controls for the camera - when true, the key for that control is being pressed
struct ControlStates
{
	bool forward, backward, left, right, yawLeft, yawRight, pitchUp,
	     pitchDown, rollLeft, rollRight;
} controls;

// timer function called every 10ms or more
void timer(int)
{
	glutPostRedisplay(); // post for display func
	glutTimerFunc(10, timer, 0); // limit frame drawing to 100fps
}

Sun* sun;
// The instance of the camera
Camera camera;

void init()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);

	// set up lighting
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	GLfloat matSpecular[] = {GLfloat(1.0), GLfloat(1.0), GLfloat(1.0), GLfloat(1.0)};
	GLfloat matAmbience[] = {GLfloat(0.3), GLfloat(0.3), GLfloat(0.3), GLfloat(1.0)};
	GLfloat matShininess[] = {GLfloat(20.0)};
	glClear(GL_COLOR_BUFFER_BIT);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbience);

	GLfloat lightAmbient[] = {GLfloat(0.3), GLfloat(0.3), GLfloat(0.3), GLfloat(1.0)};
	GLfloat lightDiffuse[] = {GLfloat(1.0), GLfloat(1.0), GLfloat(1.0), GLfloat(1.0)};
	GLfloat lightSpecular[] = {GLfloat(1.0), GLfloat(1.0), GLfloat(1.0), GLfloat(1.0)};


	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	// Load all image data
	tex_help = new TextrueHandler("help.bmp");
	tex_stars = new TextrueHandler("stars.bmp");
	tex_moon = new TextrueHandler("moon.bmp");

	TextrueHandler* tex_sun = new TextrueHandler("sun.bmp");
	TextrueHandler* tex_mercury = new TextrueHandler("mercury.bmp");
	TextrueHandler* tex_venus = new TextrueHandler("venus.bmp");
	TextrueHandler* tex_earth = new TextrueHandler("earth.bmp");
	TextrueHandler* tex_mars = new TextrueHandler("mars.bmp");
	TextrueHandler* tex_jupiter = new TextrueHandler("jupiter.bmp");
	TextrueHandler* tex_saturn = new TextrueHandler("saturn.bmp");
	TextrueHandler* tex_uranus = new TextrueHandler("uranus.bmp");
	TextrueHandler* tex_neptune = new TextrueHandler("neptune.bmp");
	TextrueHandler* tex_pluto = new TextrueHandler("pluto.bmp");

	// Add all the planets with accurate data. Distance measured in km, time measured in earth days.
	sun = new Sun(695500, 500, tex_sun->getTextureHandle()); // sun
	// radius,  rotationTime,  orbitTime,  TextureHandle,  distanceFromSun
	sun->addPlanet(2440, 58.6, 88, tex_mercury->getTextureHandle(), 57910000); // mercury
	sun->addPlanet(6052, 243, 224.65, tex_venus->getTextureHandle(), 108200000); // venus
	sun->addPlanet(6371, 1, 365, tex_earth->getTextureHandle(),149600000); // earth
	sun->addPlanet(3389, 1.03, 686, tex_mars->getTextureHandle(), 227939100); // mars
	sun->addPlanet(69911, 0.4139, 4332, tex_jupiter->getTextureHandle(), 778500000); // jupiter
	sun->addPlanet(58232, 0.44375, 10759, tex_saturn->getTextureHandle(), 1433000000); // saturn
	sun->addPlanet(25362, 0.718056, 30685, tex_uranus->getTextureHandle(), 2877000000); // uranus
	sun->addPlanet(24622, 0.6713, 60188, tex_neptune->getTextureHandle(), 4503000000); // neptune
	sun->addPlanet(1137, 6.39, 90616, tex_pluto->getTextureHandle(), 5906380000); // pluto

	sun->planets[2].addMoon(1738, 27.3, 27.3, tex_moon->getTextureHandle(), 7000000); // test moon for the earth

	// set up time
	time = 2.552f;
	timeSpeed = 0.1f;

	// reset controls
	controls.forward = false;
	controls.backward = false;
	controls.left = false;
	controls.right = false;
	controls.rollRight = false;
	controls.rollLeft = false;
	controls.pitchDown = false;
	controls.pitchUp = false;
	controls.yawLeft = false;
	controls.yawRight = false;

	timer(0);
}

void drawCube()
{
	glBegin(GL_QUADS);
	// new face
	/*
	glTexCoord2f(GLfloat s, GLfloat t)， 用于绘制图形时指定纹理的坐标。
	第一个参数是X坐标，0.0是纹理的左侧，0.5是纹理的中点，1.0是纹理的右侧。
	第二个参数是Y坐标，0.0是纹理的底部，0.5是纹理的中点，1.0是纹理的顶部。
	*/
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	// new face
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);

	glEnd();
}

void display()
{
	// update the logic and simulation
	time += timeSpeed;
	sun->caculatePosition(time);


	if (controls.forward) camera.forward();
	if (controls.backward) camera.backward();
	if (controls.left) camera.left();
	if (controls.right) camera.right();
	if (controls.yawLeft) camera.yawLeft();
	if (controls.yawRight) camera.yawRight();
	if (controls.rollLeft) camera.rollLeft();
	if (controls.rollRight) camera.rollRight();
	if (controls.pitchUp) camera.pitchUp();
	if (controls.pitchDown) camera.pitchDown();

	// clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);


	// set up the perspective matrix for rendering the 3d world
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.001f, 500.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// draw the skybox
	glBindTexture(GL_TEXTURE_2D, tex_stars->getTextureHandle());
	drawCube();

	// perform the camera orientation transform
	camera.transformOrientation();

//	// draw the skybox
//	glBindTexture(GL_TEXTURE_2D, tex_stars->getTextureHandle());
//	drawCube();

	// perform the camera translation transform
	camera.transformTranslation();


	GLfloat lightPosition[] = {0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// render the solar system
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	sun->render();

	glDisable(GL_LIGHTING);

	// possibly render orbits
	if (showOrbits)
		sun->renderOrbit();

	glDisable(GL_DEPTH_TEST);

	// set up ortho matrix for showing the UI (help dialogue)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, static_cast<GLdouble>(screenWidth), static_cast<GLdouble>(screenHeight), 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// draw the help dialogue
	if (helpDialogue)
	{
		glBindTexture(GL_TEXTURE_2D, tex_help->getTextureHandle());
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(512.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(512.0f, 512.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(0.0f, 512.0f);
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y)
{
	// check for numerical keys
	if (key > '0' && key <= '9')
	{
		// point at the specified planet
		float vec[3];
		sun->planets[key - '0'].getPosition(vec);
		camera.pointAt(vec);

		// select that planet
		planetSelected = key - '0';
	}
	static char* modes[] = { "None", "Wire", "Solid" };
	static int no = 0;
	switch (key)
	{
	case '-':
		timeSpeed /= 2.0f; // half the rate of time passing
		break;
	case '=':
		timeSpeed *= 2.0f; // double the rate of time passing
		break;
	case 'h':
		helpDialogue = !helpDialogue; // toggle the dialogue
		break;
	case '[':
		starSizeScale /= 1.2; // make planet scale smaller
		break;
	case ']':
		starSizeScale *= 1.2; // make planet scale bigger
		break;
	case 'o':
		showOrbits = !showOrbits; // toggle show orbits
		break;
	case 'm':
		//		addMoon(); // add a moon to the selected planet
		break;
	case 'r':
		starSizeScale = distanceScale;
		break;
	case ',':
		camera.slowDown(); // slow down camera
		break;
	case '.':
		camera.speedUp(); // speed up camera
		break;
		// these are all camera controls
	case 'w':
		controls.forward = true;
		break;
	case 's':
		controls.backward = true;
		break;
	case 'a':
		controls.left = true;
		break;
	case 'd':
		controls.right = true;
		break;
	case 'l':
		controls.rollRight = true;
		break;
	case 'j':
		controls.rollLeft = true;
		break;
	case 'i':
		controls.pitchDown = true;
		break;
	case 'k':
		controls.pitchUp = true;
		break;
	case 'q':
		controls.yawLeft = true;
		break;
	case 'e':
		controls.yawRight = true;
		break;
	case 'c':
		renderMode = modes[no];
		no = (no + 1) % 3;
	default: 
		break;
	}
}

void keyUp(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		controls.forward = false;
		break;
	case 's':
		controls.backward = false;
		break;
	case 'a':
		controls.left = false;
		break;
	case 'd':
		controls.right = false;
		break;
	case 'l':
		controls.rollRight = false;
		break;
	case 'j':
		controls.rollLeft = false;
		break;
	case 'i':
		controls.pitchDown = false;
		break;
	case 'k':
		controls.pitchUp = false;
		break;
	case 'q':
		controls.yawLeft = false;
		break;
	case 'e':
		controls.yawRight = false;
		break;
	default: 
		break;
	}
}

void reshape(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	glViewport(0, 0, static_cast<GLsizei>(w), static_cast<GLsizei>(h));
}

void mouseKnock(int button, int state, int x, int y)
{
	buttonType = button;
	if (button == GLUT_MIDDLE_BUTTON)
	{
//		std::cout << button << ":" << state << ":" << x << ":" << y << std::endl;
		if (state == GLUT_UP)
		{
			starSizeScale *= 1.2;
		}
		else if (state == GLUT_DOWN)
		{
			starSizeScale /= 1.2;
		}
	}
	display();
}

void mouseMove(int x, int y)
{
	static int prex = x, prey = y;//初始化
	static int difx = 0, dify = 0;
//	std::cout << difx << ":" << dify << std::endl;
	difx = x - prex;
	dify = y - prey;
	prex = x;
	prey = y;
	if (buttonType == GLUT_LEFT_BUTTON)
	{
		if (difx > 0)
		{
			camera.left();
		}
		else if (difx < 0)
		{
			camera.right();
		}
		if (dify > 0)
		{
			camera.forward();
		}
		else if (dify < 0)
		{
			camera.backward();
		}
	}
	else if (buttonType == GLUT_RIGHT_BUTTON)
	{
		if (difx > 0)
		{
			camera.rollLeft();
		}
		else if (difx < 0)
		{
			camera.rollRight();
		}
		if (dify > 0)
		{
			camera.pitchUp();
		}
		else if (dify < 0)
		{
			camera.pitchDown();
		}
	}
	display();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 700);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	//		void glutMotionFunc(void(*func)(int x, int y))

	glutMouseFunc(mouseKnock);
	glutMotionFunc(mouseMove);
	//	void glutMouseFunc(void(*func)(int button, int state, int x, int y))
	glutMainLoop();
	return 0;
}
