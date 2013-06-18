#ifndef __LESSONTEN__H__
#define __LESSONTEN__H__

#include <gl\glut.h>
#include <stdio.h>
#include <stdlib.h>

void lessonTen_display(const char* bmp_filename);

void lessonTen_display_copy();

void readBMPFile(const char* bmp_filename);

void grab(int pWidth, int pHeight);

//检查一个整数是否为2的整数次方，如果是，返回1， 否则返回0
int power_of_two(int n);

GLuint load_texture(const char* filename);

void display_texture(GLint wood, GLint grass);

#endif