#include "CTextureLookup.h"
#include <nel/3d/texture_file.h>

CTextureLookup::CTextureLookup(std::string filename) {
	NL3D::CTextureFile::buildBitmapFromFile(bitmap, filename, false);
}

CTextureLookup::~CTextureLookup() { }

NLMISC::CRGBAF CTextureLookup::lookup(unsigned int row, float val) {
	float rowF = (float)row / (float)bitmap.getHeight();
	return bitmap.getColor(val, rowF);
}