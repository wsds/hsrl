#ifndef FT2_H_
#define FT2_H_
#include <ft2build.h>
#include FT_FREETYPE_H

namespace hsrl {

class FreeType2 {
private:
	FreeType2();
	FT_Library library;
	FT_Face face;
public:
	bool ready;
	static FreeType2* getInstance() {
		static FreeType2 instance;
		return &instance;
	}
	FT_Bitmap * genCharBitmap(FT_ULong ch);
	bool isReady() const{return ready;};
};
}

#endif

