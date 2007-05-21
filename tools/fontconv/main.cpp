#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

int next_p2(int a)
{
	int rval = 1;

	while (rval < a)
	{
		rval <<= 1;
	}
	return rval;
}

int make_dlist(FILE* output, FT_Face face, char ch)
{
	if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
	{
		return 1;
	}

	FT_Glyph glyph;
	if (FT_Get_Glyph(face->glyph, &glyph))
	{
		return 1;
	}

	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	FT_Bitmap& bitmap = bitmap_glyph->bitmap;
	
	int width = next_p2(bitmap.width);
	int height = next_p2(bitmap.rows);

	unsigned char* expanded_data = (unsigned char*)malloc(sizeof(unsigned char) *  2 * width * height);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			expanded_data[2 * (i + j * width)] = expanded_data[2 * (i + j * width) + 1] = 
				(i >= bitmap.width || j >= bitmap.rows) ?
				0 : bitmap.buffer[i + bitmap.width * j];
		}
	}

	unsigned int array_size = 2 * width * height;

	fprintf(output, "		GLubyte expanded_data%d[] = {", ch);
	for (unsigned int i = 0; i < array_size; i++)
	{
		if (i != 0)
		{
			fprintf(output, ", ");
		}
		fprintf(output, "0x%x", expanded_data[i]);
	}
	fprintf(output, "};\n");
	free(expanded_data);

	fprintf(output, "		make_dlist(%d, %d, %d, expanded_data%d, %d, %d, %d, %d, %d);\n",
							ch,
							width,
							height,
							ch,
							bitmap_glyph->left,
							bitmap_glyph->top-bitmap.rows,
							bitmap.width,
							bitmap.rows,
							face->glyph->advance.x);

	return 0;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Error: wrong number of parameters\n");
		printf("Usage: %s [font name] [font size]\n", argv[0]);
		return -1;
	}

	FT_Library library;
	if (FT_Init_FreeType(&library)) 
	{
		printf("Error initializing FreeType library.\n");
		return 1;
	}

	FT_Face face;

	unsigned int h;
	sscanf(argv[2], "%d", &h);
	char file_name[255];
	sprintf(file_name, "%s.ttf", argv[1]);

	if (FT_New_Face(library, file_name, 0, &face))
	{
		printf("Error opening font file: '%s'.\n", file_name);
		return -1;
	}

	FT_Set_Char_Size(face, h << 6, h << 6, 96, 96);

	char output_file_name[255];
	char class_name[255];
	sprintf(output_file_name, "Font%s%d.h", argv[1], h);
	sprintf(class_name, "Font%s%d", argv[1], h);

	FILE* output = fopen(output_file_name, "w");

	fprintf(output, "#ifndef __OPENGL_FONT_%s_%d\n", argv[1], h);
	fprintf(output, "#define __OPENGL_FONT_%s_%d\n", argv[1], h);
	fprintf(output, "#include \"Font.h\"\n");
	fprintf(output, "class %s : public Font\n{\npublic:\n", class_name);
	fprintf(output, "	%s(){}\n", class_name);
	fprintf(output, "	virtual ~%s(){}\n", class_name);
	fprintf(output, "protected:\n");
	fprintf(output, "	virtual void load_textures()\n");
	fprintf(output, "	{\n");
	fprintf(output, "		_h = %d;\n", h);

	for(unsigned char i = 0; i < 128; i++)
	{
		make_dlist(output, face, i);
	}
	
	fprintf(output, "	}\n");
	fprintf(output, "};\n#endif\n\n");
	fclose(output);

	FT_Done_Face(face);

	FT_Done_FreeType(library);

	return 0;
}

