#ifndef NL_CTEXTURELOOKUP_H
#define NL_CTEXTURELOOKUP_H

#include <nel/misc/bitmap.h>
#include <nel/misc/rgba.h>
#include <string>

class CTextureLookup {
private:
	NLMISC::CBitmap bitmap;
public:

	CTextureLookup(std::string filename);
	~CTextureLookup();

	NLMISC::CRGBAF lookup(unsigned int row, float val);

};

#endif // NL_CTEXTURELOOKUP_H