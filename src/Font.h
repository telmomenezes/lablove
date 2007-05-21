#ifndef __OPENGL_FONT_H
#define __OPENGL_FONT_H

#include <GL/gl.h>
#include <GL/glu.h>

#include <vector>
#include <string>

using namespace std;

class Font
{
public:
	Font();
	virtual ~Font();

	bool init();
	void print(float x, float y, const char *fmt, ...);

protected:
	virtual void load_textures()=0;
	bool make_dlist(char ch,
			int width,
			int height,
			GLubyte* expanded_data,
			int glyph_left,
			int glyph_top_bitmap_rows,
			int bitmap_width,
			int bitmap_rows,
			int glyph_advance_x);
	void push_screen_coordinate_matrix();
	void pop_projection_matrix();

	float _h;
	GLuint* _textures;
	GLuint _list_base;
};
#endif

