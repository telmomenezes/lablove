/*
 * LOVE Lab
 * Copyright (C) 2007 Telmo Menezes.
 * telmo@telmomenezes.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the version 2 of the GNU General Public License 
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ImageSDLOpenGL.h"

#include <png.h>

namespace layer
{

ImageSDLOpenGL::ImageSDLOpenGL()
{
}

ImageSDLOpenGL::~ImageSDLOpenGL()
{
}

bool ImageSDLOpenGL::loadPNG(std::string filePath)
{
	FILE *infile;
	png_structp pngPtr;
	png_infop infoPtr;

	unsigned char *imageData;
	char sig[8];

	int bitDepth;
	int colorType;

	unsigned long width;
	unsigned long height;
	unsigned int rowbytes;

	imageData = NULL;
	int i;
	png_bytepp rowPointers = NULL;

	infile = fopen(filePath.c_str(), "rb");
	if (!infile)
	{
		return false;
	}

	fread(sig, 1, 8, infile);

	if (!png_check_sig((unsigned char*)sig, 8))
	{
		fclose(infile);
		//ERROR: wrong file format
		return false;
	}
 
	pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngPtr)
	{
		fclose(infile);
		//ERROR: out of memory!
		return false;
	}
 
	infoPtr = png_create_info_struct(pngPtr);
	if (!infoPtr)
	{
		png_destroy_read_struct(&pngPtr, (png_infopp)NULL, (png_infopp)NULL);
		fclose(infile);
		//ERROR: out of memory!
		return false;
	}
   
  
	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
		fclose(infile);
		return false;
	}

	png_init_io(pngPtr, infile);
   
   	png_set_sig_bytes(pngPtr, 8);

	png_read_info(pngPtr, infoPtr);

	png_get_IHDR(pngPtr,
			infoPtr,
			&width,
			&height,
			&bitDepth,
			&colorType,
			NULL,
			NULL,
			NULL);

	mWidth = width;
	mHeight = height;

	mTextureWidth = nextPowerOfTwo(mWidth);
	mTextureHeight = nextPowerOfTwo(mHeight);
   
	if (bitDepth > 8)
	{
		png_set_strip_16(pngPtr);
	}
	if (colorType == PNG_COLOR_TYPE_GRAY
		|| colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		png_set_gray_to_rgb(pngPtr);
	}
	if (colorType == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(pngPtr);
	}

   	png_read_update_info(pngPtr, infoPtr);
	png_get_IHDR(pngPtr,
			infoPtr,
			&width,
			&height,
			&bitDepth,
			&colorType,
			NULL,
			NULL,
			NULL);

	unsigned int bpp = 3;
	GLint internalTextureFormat = GL_RGB;
	GLenum textureFormat = GL_RGB;
	if (colorType == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		bpp = 4;
		internalTextureFormat = GL_RGBA;
		textureFormat = GL_RGBA;
	}

	rowbytes = png_get_rowbytes(pngPtr, infoPtr);

	if ((imageData = (unsigned char*)malloc(rowbytes * height)) == NULL)
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
		return false;
	}

	if ((rowPointers = (png_bytepp)malloc(height * sizeof(png_bytep))) == NULL)
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
		free(imageData);
		imageData = NULL;
		return false;
	}

	for (i = 0;  i < height;  ++i)
	{
		rowPointers[i] = imageData + i * rowbytes;
	}

	png_read_image(pngPtr, rowPointers);

	free(rowPointers);

	png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
	fclose(infile);

	

	unsigned char* textureData = (unsigned char*)malloc(mTextureWidth * mTextureHeight * bpp);

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < rowbytes; x++)
		{
			textureData[(mTextureWidth * bpp * y) + x] = imageData[(rowbytes * y) + x];
		}
	}

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexImage2D(GL_TEXTURE_2D,
			0,
			internalTextureFormat,
			mTextureWidth,
			mTextureHeight,
			0,
			textureFormat,
			GL_UNSIGNED_BYTE,
			textureData);

	delete imageData;
	delete textureData;

	return true;
}

void ImageSDLOpenGL::draw(float x, float y)
{
	float origX1 = 0.0f;
	float origY1 = 0.0f;
	float origX2 = ((float)mWidth) / ((float)mTextureWidth);
	float origY2 = ((float)mHeight) / ((float)mTextureHeight);

	float x2 = x + mWidth;
	float y2 = y + mHeight;

	glBindTexture(GL_TEXTURE_2D, mTexture);
	glEnable(GL_TEXTURE_2D);

	glBegin(GL_QUADS);

	glTexCoord2f(origX1, origY1);
	glVertex3f(x, y, 0.0f);

	glTexCoord2f(origX2, origY1);
	glVertex3f(x2, y, 0.0f);

	glTexCoord2f(origX2, origY2);
	glVertex3f(x2, y2, 0.0f);

	glTexCoord2f(origX1, origY2);
	glVertex3f(x, y2, 0.0f);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

}

