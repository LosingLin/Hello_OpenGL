#include "leOne2leNine.h"


const GLfloat Pi = 3.1415926536f;

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLE_STRIP);
	//glVertex4i(10, 10, 100, 100);
	glVertex2f(0.0f, 0.0f);  //v0
	glVertex2f(0.5f, 0.5f);  //v1
	glVertex2f(0.0f, 0.5f);  //v2
	glVertex2f(0.5f, 0.0f);	 //v3
	glVertex2f(-0.5f, 0.0f); //v4
	glVertex2f(0.0f, -0.5f); //v5
	glEnd();
	glFlush();
}

void drawRadiu(void)
{
	const int n = 20;
	const GLfloat R = 0.5f;
	
	int i=0;

	glClear(GL_COLOR_BUFFER_BIT);
	
	/*
	glBegin(GL_POLYGON);
	
	for(i = 0; i < n; ++ i)
	{
		glVertex2f(R*cos(2*Pi/n*i), R*sin(2*Pi/n*i));   //��Բ
	}
	glEnd();
	*/
	/*
	glPointSize(5.0f);       //���õ�Ĵ�С����λΪ����
	glBegin(GL_POINTS);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.0f, 0.5f);
	glEnd();
	*/
	glLineWidth(10.0f);
	glEnable(GL_LINE_STIPPLE);  //��������ģʽ
	//�������ߵı�ʾģʽ��0x00FF�ӵ�λ��ʼ�����Ϊ1��ʾ��������2���㽫��Ϊʵ�����Ϊ0���������2���㽫��Ϊ�顣
	glLineStipple(2, 0x00FF);  
	glBegin(GL_LINES);
	glVertex2f(0.0f, -0.5f);
	glVertex2f(0.0f, 0.5f);
	glEnd();
	glFlush();
}

void displayFrongAndBack(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0f, 1.0f, 1.0f);
	glPolygonMode(GL_FRONT, GL_FILL); //��������Ϊ���ģʽ
	glPolygonMode(GL_BACK, GL_LINE); //���÷���Ϊ����ģʽ
	glFrontFace(GL_CCW); //������ʱ�뷽��Ϊ����
	glEnable(GL_CULL_FACE); //�����޳�����
	glCullFace(GL_FRONT);
	glBegin(GL_POLYGON);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.0f, -0.5f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(-0.5f, 0.0f);
	glEnd();
	glBegin(GL_POLYGON);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(0.5f, 0.0f);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(0.0f, 0.5f);
	glEnd();
	glFlush();
}

void displayPolygonStipple() //�οն����
{
	static GLubyte Mask[128];
	FILE *fp;
	int i = 0;
	fp = fopen("mask.bmp", "rb");
	if(!fp)
		exit(0);
	if(fseek(fp,-(int)sizeof(Mask), SEEK_END))
		exit(0);
	//if(fseek(fp, 0, 0))
	//	exit(0);
	if(!fread(Mask, sizeof(Mask), 1, fp))
		exit(0);
	fclose(fp);
	for (i=0; i<sizeof(Mask); ++ i)
	{
		printf("%d ", Mask[i]);
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_POLYGON_STIPPLE);
	glPolygonStipple(Mask);
	glRectf(-0.5f, -0.5f, 0.0f, 0.0f);
	glDisable(GL_POLYGON_STIPPLE);
	glRectf(0.0f, 0.0f, 0.5f, 0.5f);
	glFlush();
}

void displayColor(void)
{
	int i;
	//glClearColor(1.0f, 1.0f, 0.0f, 0.0f); //ָ�����ա�����ɫ
	glShadeModel(GL_FLAT); //���õ�ɫģʽ��GL_SMOOTH:ƽ��ģʽ��Ĭ�ϣ�
	glClear(GL_COLOR_BUFFER_BIT);
	//glColor3f(0.0f, 1.0f, 1.0f); //RGBAģʽ
	//glRectf(-0.5f, -0.5f, 0.5f, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.0f, 0.0f);
	for (i = 0; i <= 8; ++ i)
	{
		glColor3f(i&0x04, i&0x02, i&0x01);
		glVertex2f(cos(i*Pi / 4), sin(1*Pi/4));
	}
	glEnd();
	glFlush();
}

double CalFrequency()
{
	static int count;
	static double save;
	static clock_t last, current;
	double timegap;

	++ count;
	if (count <= 50)
	{
		return save;
	}
	count = 0;
	last = current;
	current = clock();
	timegap = (current - last) / (double)CLK_TCK;
	save = 50.0 / timegap;

	return save;
}

static float day = 360.0f;
static clock_t last = 600;
static clock_t current;

void displaySEM(void)
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
	
	FPS = CalFrequency();
	printf("FPS = %f\n", FPS);

	day += 0.1;
	if(day >= 360.0f)
	{
		day = 0.0f;
	}

	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, 1, 1, 100000000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, -200000000, 200000000, 0, 0, 0, 0, 0, 1);

	//sun
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(69600000, 20, 20);

	//earth
	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(day/30.0*360.0 - day/360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(150000000, 0.0f, 0.0f);
	glutSolidSphere(15945000, 20, 20);

	//moon
	glColor3f(1.0f, 1.0f, 0.0f);
	glRotatef(day/30.0*360.0 - day/360.0*360.0, 0.0f, 0.0f, -1.0f);
	glTranslatef(38000000, 0.0f, 0.0f);
	glutSolidSphere(4345000, 20, 20);

	glFlush();

	glutSwapBuffers();
}

void myIdle(void)
{
	/*
	++ day;
	if(day >= 360)
	{
		day = 0;
	}
	*/
	displaySEM();
}

static GLfloat angle = 0.0f;

void displaySEMLight(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//create projection view
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, 1.0f, 1.0f, 20.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 5.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	//����̫����Դ�� ����һ�ְ�ɫ�Ĺ�Դ
	{
		GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}

	//����̫���Ĳ��ʲ�����̫��
	{
		GLfloat sun_mat_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_mat_diffuse[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_mat_specular[] = {0.0f, 1.0f, 0.0f, 1.0f};
		GLfloat sun_mat_emission[] = {0.5f, 1.0f, 0.5f, 0.5f};
		GLfloat sun_mat_shininess = 30.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);

		glutSolidSphere(2.0, 40, 32);
	}

	//�������Ĳ��ʲ����Ƶ���
	{
		GLfloat earth_mat_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
		GLfloat earth_mat_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
		GLfloat earth_mat_specular[] = {0.0f, 1.0f, 0.0f, 1.0f};
		GLfloat earth_mat_emission[] = {0.0f, 0.5f, 0.0f, 1.0f};
		GLfloat earth_mat_shininess = 80.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);


		glRotatef(angle, 0.0f, -1.0f, 0.0f);
		glTranslatef(8.0f, 0.0f, 0.0f);
		glutSolidSphere(1.5, 40, 32);
	}

	//��������Ĳ��ʲ���������
	{
		GLfloat moon_mat_ambient[] = {0.5f, 0.5f, 0.5f, 1.0f};
		GLfloat moon_mat_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
		GLfloat moon_mat_specular[] = {0.0f, 1.0f, 0.0f, 1.0f};
		GLfloat moon_mat_emission[] = {0.0f, 0.5f, 0.0f, 1.0f};
		GLfloat moon_mat_shininess = 80.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, moon_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, moon_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, moon_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, moon_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, moon_mat_shininess);


		glRotatef(angle, 0.0f, -1.0f, 0.0f);
		glTranslatef(3.0f, 0.0f, 0.0f);
		glutSolidSphere(0.5, 40, 32);
	}

	glutSwapBuffers();
}

#define ColoredVertex(c, v) do{glColor3fv(c); glVertex3fv(v);}while(0);

void setNormal(GLfloat *point1, GLfloat *point2, GLfloat *point3)
{
	GLfloat normal[3];
	int i = 0;
	for (i = 0; i < 3; ++ i)
	{
		normal[i] = (point1[i], point2[i], point3[i]) / 3;
	}
	glNormal3fv(normal);
}

void myDisplaySBX(void)
{
	static int list = 0;

	if (list == 0)
	{
		GLfloat PointA[] = {0.5f, -sqrt(6.0f)/12, -sqrt(3.0f)/6},
				PointB[] = {-0.5f, -sqrt(6.0f)/12, -sqrt(3.0f)/6},
				PointC[] = {0.0f, -sqrt(6.0f)/12, -sqrt(3.0f)/3},
				PointD[] = {0.0f, -sqrt(6.0f)/4, 0};
		GLfloat ColorR[] = {1, 0, 0},
				ColorG[] = {0, 1, 0},
				ColorB[] = {0, 0, 1},
				ColorY[] = {1, 1, 0};

		list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		glBegin(GL_TRIANGLES);

		//ABC
		//ColoredVertex(ColorR, PointA);
		//ColoredVertex(ColorG, PointB);
		//ColoredVertex(ColorB, PointC);
		{
			GLfloat earth_mat_ambient[] = {1.0f, 0.0f, 0.0f, 1.0f};
			GLfloat earth_mat_diffuse[] = {1.0f, 0.0f, 0.0f, 1.0f};
			GLfloat earth_mat_specular[] = {1.0f, 0.0f, 0.0f, 1.0f};
			GLfloat earth_mat_emission[] = {0.5f, 0.0f, 0.0f, 1.0f};
			GLfloat earth_mat_shininess = 80.0f;

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, earth_mat_ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, earth_mat_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, earth_mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, earth_mat_emission);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, earth_mat_shininess);

			glVertex3fv(PointA);
			glVertex3fv(PointB);
			glVertex3fv(PointC);

			setNormal(PointA, PointB, PointC);
		}

		//ACD
		//ColoredVertex(ColorR, PointA);
		//ColoredVertex(ColorB, PointC);
		//ColoredVertex(ColorY, PointD);
		{
			GLfloat earth_mat_ambient[] = {0.0f, 1.0f, 0.0f, 1.0f};
			GLfloat earth_mat_diffuse[] = {0.0f, 1.0f, 0.0f, 1.0f};
			GLfloat earth_mat_specular[] = {0.0f, 1.0f, 0.0f, 1.0f};
			GLfloat earth_mat_emission[] = {0.0f, 0.5f, 0.0f, 1.0f};
			GLfloat earth_mat_shininess = 80.0f;

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, earth_mat_ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, earth_mat_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, earth_mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, earth_mat_emission);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, earth_mat_shininess);

			glVertex3fv(PointA);
			glVertex3fv(PointC);
			glVertex3fv(PointD);

			setNormal(PointA, PointC, PointD);
		}

		//CBD
		//ColoredVertex(ColorB, PointC);
		//ColoredVertex(ColorG, PointB);
		//ColoredVertex(ColorY, PointD);
		{
			GLfloat earth_mat_ambient[] = {0.0f, 0.0f, 1.0f, 1.0f};
			GLfloat earth_mat_diffuse[] = {0.0f, 0.0f, 1.0f, 1.0f};
			GLfloat earth_mat_specular[] = {0.0f, 0.0f, 1.0f, 1.0f};
			GLfloat earth_mat_emission[] = {0.0f, 0.0f, 0.5f, 1.0f};
			GLfloat earth_mat_shininess = 80.0f;

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, earth_mat_ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, earth_mat_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, earth_mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, earth_mat_emission);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, earth_mat_shininess);

			glVertex3fv(PointC);
			glVertex3fv(PointB);
			glVertex3fv(PointD);

			setNormal(PointC, PointB, PointD);
		}

		//BAD
		//ColoredVertex(ColorG, PointB);
		//ColoredVertex(ColorR, PointA);
		//ColoredVertex(ColorY, PointD);
		{
			GLfloat earth_mat_ambient[] = {0.0f, 1.0f, 1.0f, 1.0f};
			GLfloat earth_mat_diffuse[] = {0.0f, 1.0f, 1.0f, 1.0f};
			GLfloat earth_mat_specular[] = {0.0f, 1.0f, 1.0f, 1.0f};
			GLfloat earth_mat_emission[] = {0.0f, 0.5f, 0.5f, 1.0f};
			GLfloat earth_mat_shininess = 80.0f;

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, earth_mat_ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, earth_mat_diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, earth_mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, earth_mat_emission);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, earth_mat_shininess);

			glVertex3fv(PointB);
			glVertex3fv(PointA);
			glVertex3fv(PointD);

			setNormal(PointB, PointA, PointD);
		}

		glEnd();
		glEndList();
		glEnable(GL_DEPTH_TEST);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�����Դ�� ����һ�ְ�ɫ�Ĺ�Դ
	{
		GLfloat sun_light_position[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_light_ambient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		GLfloat sun_light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		GLfloat sun_light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

		glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
		glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
	}
	
	
	glPushMatrix();
	glRotatef(angle, 1, 0.5, 0);
	glCallList(list);
	glPopMatrix();

	glutSwapBuffers();
}

void displayBlend(void)
{
	static int list = 0;
	if (list == 0)
	{
		list = glGenLists(1);
		glNewList(list, GL_COMPILE);

		glColor4f(1, 0, 0, 0.5);
		glRectf(-1, -1, 0.5, 0.5);
		glColor4f(0, 1, 0, 0.5);
		glRectf(-0.5, -0.5, 1, 1);

		glEndList();
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	//glBlendFunc(GL_ONE, GL_ZERO);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE);

	//glPushMatrix();
	glCallList(list);
	//glPopMatrix();

	glutSwapBuffers();

	glDeleteLists(list, 1);
	list = 0;
}