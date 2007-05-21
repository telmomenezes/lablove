#ifdef __LOVE_GRAPHICS

#include "Font.h"
#include <stdarg.h>

Font::Font()
{
	_h = -1.0f;
	_textures = NULL;
	_list_base = 0;
}

Font::~Font()
{
	if (_h >= 0)
	{
		glDeleteLists(_list_base, 128);
		glDeleteTextures(128, _textures);
		delete [] _textures;
		_h = -1.0f;
	}
}

bool Font::make_dlist(char ch,
			int width,
			int height,
			GLubyte* expanded_data,
			int glyph_left,
			int glyph_top_bitmap_rows,
			int bitmap_width,
			int bitmap_rows,
			int glyph_advance_x)
{
	glBindTexture(GL_TEXTURE_2D, _textures[ch]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, 
			GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

	glNewList(_list_base + ch, GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D, _textures[ch]);

	glPushMatrix();

	glTranslatef(glyph_left, 0, 0);

	glTranslatef(0, glyph_top_bitmap_rows, 0);

	float x = (float)bitmap_width / (float)width,
	y = (float)bitmap_rows / (float)height;

	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex2f(0, bitmap_rows);
	glTexCoord2d(0, y);
	glVertex2f(0, 0);
	glTexCoord2d(x, y);
	glVertex2f(bitmap_width, 0);
	glTexCoord2d(x, 0);
	glVertex2f(bitmap_width, bitmap_rows);
	glEnd();
	glPopMatrix();
	glTranslatef(glyph_advance_x >> 6, 0, 0);

	glEndList();

	return true;
}

bool Font::init()
{
	_textures = new GLuint[128];

	_list_base = glGenLists(128);
	glGenTextures(128, _textures);

	load_textures();

	return true;
}

void Font::push_screen_coordinate_matrix()
{
	glPushAttrib(GL_TRANSFORM_BIT);
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(viewport[0], viewport[2], viewport[1], viewport[3], -1.0, 1.0);
	glPopAttrib();
}

void Font::pop_projection_matrix()
{
	glPushAttrib(GL_TRANSFORM_BIT);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void Font::print(float x, float y, const char *fmt, ...)
{
	glPushMatrix();
	glLoadIdentity();

	push_screen_coordinate_matrix();                                   
        
	GLuint font = _list_base;

	float h = _h / .63f;                                                 
	char text[256];
	va_list	ap;

	if (fmt == NULL)
	{
		*text=0;
	}
	else
	{
		va_start(ap, fmt);
		vsprintf(text, fmt, ap);
		va_end(ap);
	}

	const char *start_line = text;
	vector<string> lines;
	const char *c = text;
	for(; *c; c++)
	{
		if(*c=='\n')
		{
			string line;
			for (const char *n = start_line; n < c; n++)
			{
				line.append(1, *n);
			}
			lines.push_back(line);
			start_line = c + 1;
		}
	}
	if(start_line)
	{
		string line;
		for(const char *n = start_line; n < c; n++)
		{
			line.append(1, *n);
		}
		lines.push_back(line);
	}

	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT  | GL_ENABLE_BIT | GL_TRANSFORM_BIT); 
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);      

	glListBase(font);
	float modelview_matrix[16];     
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	for(int i = 0; i < lines.size(); i++)
	{
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(x, y - h * i, 0);
		glMultMatrixf(modelview_matrix);

		glCallLists(lines[i].length(), GL_UNSIGNED_BYTE, lines[i].c_str());

		glPopMatrix();
	}

	glPopAttrib();          

	pop_projection_matrix();

	glPopMatrix();
}

#endif

