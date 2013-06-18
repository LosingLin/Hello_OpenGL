#include <gl\glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "leOne2leNine.h"
#include "lessonNine.h"
#include "lessonTen.h"


static clock_t last = 600;
static clock_t current;
static GLfloat angle = 0.0f;


void logicUpdate(void)
{
	angle += 1.0f;

	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}
}

void viewUpdate(void)
{
	//lessonTen_display("lanlan02.bmp");
	//lessonTen_display_copy();
	static GLint grass, wood;
	static int load = 0;
	if (load == 0)
	{
		grass = load_texture("grass.bmp");
		wood = load_texture("wood.bmp");
		load = 1;
	}
	display_texture(wood, grass);
}

void idle(void)
{
	double FPS;
	int stopSwapBuffer = 0;
	
	stopSwapBuffer = 0;
	current = clock();
	//printf("%lu - %lu = %lu", last, current, last - current);
	if	(current - last <= 10)
	{
		stopSwapBuffer = 1;
	}
	if (stopSwapBuffer == 1)
	{
		return;
	}
	last = current;

	logicUpdate();
	
	FPS = CalFrequency();
	printf("FPS = %f\n", FPS);

	viewUpdate();
}



int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);//Ë«»º´æ
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(600, 600);
	glutCreateWindow("My first OpenGL programe");
	//glutDisplayFunc(&myDisplay);
	//glutDisplayFunc(&drawRadiu);
	//glutDisplayFunc(&displayFrongAndBack);
	//glutDisplayFunc(&displayPolygonStipple);
	//glutDisplayFunc(&displayColor);

	//glutDisplayFunc(&displaySEM);
	//glutIdleFunc(&myIdle);

	glutDisplayFunc(&viewUpdate);
	glutIdleFunc(&idle);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);


	glutMainLoop();
	return 0;
}