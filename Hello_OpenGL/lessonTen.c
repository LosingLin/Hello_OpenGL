#include "lessonTen.h"

static int BytesPerPixel = 3;  //24位BMP每个像素有3个字节

static GLubyte* pixelData = 0;

static GLint width, height; //opengl的GLint是32位的， 用于存储图片的大小

void lessonTen_display(const char* bmp_filename)
{

	static int i = 0;
	
	glClear(GL_COLOR_BUFFER_BIT);

	if (pixelData == 0)
	{
		readBMPFile(bmp_filename);
	}

	glDrawPixels(width, height, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixelData);

	glutSwapBuffers();

	
	if(i == 0)
	{
		grab(450, 450);
		i = 1;
	}

}

void readBMPFile(const char* bmp_filename)
{
	int lineLength, totalLength;

	FILE *pFile = fopen(bmp_filename, "rb");

	fseek(pFile, 0x0012, SEEK_SET);  
	fread(&width, sizeof(width), 1, pFile);
	fseek(pFile, 0x0016, SEEK_SET);
	fread(&height, sizeof(height), 1, pFile);

	printf("width: %d, height: %d \n", width, height);

	//BMP每行像素数据的长度会补齐到4的倍数
	lineLength = width * BytesPerPixel;
	while(lineLength % 4 != 0)
	{
		++ lineLength;
	}
	totalLength = lineLength * height;

	printf("total: %d Bytes \n", totalLength);

	pixelData = (GLubyte*)malloc(totalLength);

	if (pixelData == 0) exit(0);

	fseek(pFile, 54, SEEK_SET); //跳过前面的文件头
	
	fread(pixelData, totalLength, 1, pFile);

	fclose(pFile);
}

#define BMP_Header_Length 54

void grab(int pWidth, int pHeight)
{
	FILE *originFile = fopen("lanlan02.bmp", "rb");
	FILE *grabFile = fopen("grab.bmp", "wb");

	GLubyte *pPixelData;
	GLubyte BMP_Header[BMP_Header_Length];

	GLint lineBytes;
	GLint pixelDataLength;

	lineBytes = pWidth * BytesPerPixel;

	while(lineBytes % 4 != 0)
	{
		++ lineBytes;
	}

	pixelDataLength = lineBytes * pHeight;

	pPixelData = (GLubyte *)malloc(pixelDataLength);
	if (pPixelData == 0) exit(0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glReadPixels(0, 0, pWidth, pHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

	fread(BMP_Header, sizeof(BMP_Header), 1, originFile);
	fwrite(BMP_Header, sizeof(BMP_Header), 1, grabFile);

	fseek(grabFile, 0x0012, SEEK_SET);
	fwrite(&pWidth, sizeof(pWidth), 1, grabFile);
	fseek(grabFile, 0x0016, SEEK_SET);
	fwrite(&pHeight, sizeof(pHeight), 1, grabFile);

	fseek(grabFile, 0, SEEK_END);
	fwrite(pPixelData, pixelDataLength, 1, grabFile);

	fclose(originFile);
	fclose(grabFile);

	free(pPixelData);
}

void lessonTen_display_copy()
{
	static int grabed = 0;
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(0.5f, 1.0f);
	glEnd();

	glPixelZoom(0.5f, 0.5f);

	glRasterPos2i(-1, -1);

	glCopyPixels(300, 300, 300, 300, GL_COLOR);

	glutSwapBuffers();

	if (grabed == 0)
	{
		grab(600, 600);
		grabed = 1;
	}
}

int power_of_two(int n)
{
	if (n <= 0) return 0;
	return (n & (n-1)) == 0;
}

GLuint load_texture(const char* filename)
{
	GLint width, height, total_bytes;
	GLubyte *pixels = 0;
	GLuint last_texture_ID, texture_ID = 0;

	FILE *pFile = fopen(filename, "rb");
	if (pFile == 0) return 0;

	fseek(pFile, 0x0012, SEEK_SET);
	fread(&width, sizeof(width), 1, pFile);
	fseek(pFile, 0x0016, SEEK_SET);
	fread(&height, sizeof(height), 1, pFile);

	fseek(pFile, BMP_Header_Length, SEEK_SET);

	{
		GLint line_bytes = width * 3;
		while(line_bytes % 4 != 0)
		{
			++ line_bytes;
		}
		total_bytes = line_bytes * height;
	}

	pixels = (GLubyte*)malloc(total_bytes);

	if (pixels == 0)
	{
		fclose(pFile);
		return 0;
	}

	if (fread(pixels, total_bytes, 1, pFile) <= 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	//OPENGL支持的纹理有限制...
	{
		GLint max;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);

		if(!power_of_two(width) || !power_of_two(height) || width > max || height > max)
		{
			const GLint new_width = 256;
			const GLint new_height = 256;
			GLint new_line_bytes, new_total_bytes;
			GLubyte *new_pixels = 0;

			new_line_bytes = new_width * 3;
			while(new_line_bytes % 4 != 0) ++new_line_bytes;
			new_total_bytes = new_line_bytes * new_height;

			new_pixels = (GLubyte*)malloc(new_total_bytes);
			if (new_pixels == 0)
			{
				free(pixels);
				fclose(pFile);
				return 0;
			}

			gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels,
				new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

			free(pixels);

			pixels = new_pixels;
			width = new_width;
			height = new_height;
		}
	}

	glGenTextures(1, &texture_ID);

	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);

	glBindTexture(GL_TEXTURE_2D, texture_ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT,
		GL_UNSIGNED_BYTE, pixels);

	glBindTexture(GL_TEXTURE_2D, last_texture_ID);

	free(pixels);
	return texture_ID;
}

void display_texture(GLint wood, GLint grass)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, 1, 1, 21);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1, 5, 5, 0, 0, 0, 0, 0, 1);

	glBindTexture(GL_TEXTURE_2D, grass);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, -8.0f, 0.0f);
		glTexCoord2f(0.0f, 5.0f); glVertex3f(-8.0f, 8.0f, 0.0f);
		glTexCoord2f(5.0f, 5.0f); glVertex3f(8.0f, 8.0f, 0.0f);
		glTexCoord2f(5.0f, 0.0f); glVertex3f(8.0f, -8.0f, 0.0f);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, wood);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, -3.0f, 1.5f);
		glTexCoord2f(5.0f, 1.0f); glVertex3f(6.0f, -3.0f, 1.5f);
		glTexCoord2f(5.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
	glEnd();

	glRotatef(-90, 0, 0, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.0f, -3.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.0f, -3.0f, 1.5f);
		glTexCoord2f(5.0f, 1.0f); glVertex3f(6.0f, -3.0f, 1.5f);
		glTexCoord2f(5.0f, 0.0f); glVertex3f(6.0f, -3.0f, 0.0f);
	glEnd();

	glutSwapBuffers();

	{
		static int i = 0;
		if(i == 0)
		{
			grab(600, 600);
			i = 1;
		}
	}
}