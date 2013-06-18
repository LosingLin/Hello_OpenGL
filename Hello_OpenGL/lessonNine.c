#include "lessonNine.h"


void setLight(void)
{
	static const GLfloat light_position[] = {1.0f, 1.0f, -1.0f, 1.0f};
	static const GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
	static const GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
	static const GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
}

void setMatirial(const GLfloat mat_diffuse[4], GLfloat mat_shininess)
{
	static const GLfloat mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
	static const GLfloat mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};

	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);
}

void lessonNine_myDisplay(void)
{
	static const GLfloat red_color[] = {1.0f, 0.0f, 0.0f, 1.0f};
	static const GLfloat green_color[] = {0.0f, 1.0f, 0.0f, 0.3333f};
	static const GLfloat blue_color[] = {0.0f, 0.0f, 1.0f, 0.5f};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setLight();

	//不透明的红球
	setMatirial(red_color, 30.0);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.5f);
	glutSolidSphere(0.3, 30, 30);
	glPopMatrix();

	//将深度缓冲设置为只读
	glDepthMask(GL_FALSE);
	
	//半透明的蓝球
	setMatirial(blue_color, 30.0);
	glPushMatrix();
	glTranslatef(0.2f, 0.0f, -0.5f);
	glutSolidSphere(0.2, 30, 30);
	glPopMatrix();


	//半透明的绿球
	setMatirial(green_color, 30.0);
	glPushMatrix();
	glTranslatef(0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.15, 30, 30);
	glPopMatrix();

	//将深度缓冲恢复为可读可写
	glDepthFunc(GL_TRUE);

	glutSwapBuffers();
}