#include "FreeType2.h"
#include <android/log.h>
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,"freetype2",__VA_ARGS__)

namespace hsrl {
FreeType2::FreeType2() {
	ready = false;
	FT_Error error;
	error = FT_Init_FreeType(&library);
	if (error) {
		FT_Done_FreeType(library);
		return;
	}
	error = FT_New_Face(library, "/system/fonts/DroidSansFallback.ttf", 0,
			&face);
	if (error == FT_Err_Unknown_File_Format) {
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		return;
	} else if (error) {
		FT_Done_Face(face);
		FT_Done_FreeType(library);
		return;
	}
	FT_Select_Charmap(face, FT_ENCODING_UNICODE);

	error = FT_Set_Char_Size(face, 0, 16 * 64, 300, 300);

	if (error) {
		return;
	}

	ready = true;
}
FT_Bitmap* FreeType2::genCharBitmap(FT_ULong ch) {
	if(!ready){
		return NULL;
	}
	FT_Error error = 0;
	FT_Bitmap* bitmap = NULL;
	FT_UInt glyph_index = FT_Get_Char_Index(face, ch);
	error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
	if (error) {
		return bitmap;
	}

	error = FT_Render_Glyph(face->glyph, ft_render_mode_normal);
	if (error) {
		return bitmap;
	}
	bitmap = &(face->glyph->bitmap);
	return bitmap;
}
}
