#ifndef NL_NELOID_EXCEPTIONS_H
#define NL_NELOID_EXCEPTIONS_H

#include <string>


#include <nel/misc/types_nl.h>

#include <nel/misc/i_xml.h>

class ENoLevelLoaded : public NLMISC::Exception {
public:
	ENoLevelLoaded() : NLMISC::Exception("No level is currently loaded.") { }
	ENoLevelLoaded(const std::string &str) : NLMISC::Exception( str ) {}
};

class ENoSuchLevel : public NLMISC::Exception {
public:
	ENoSuchLevel() : NLMISC::Exception("No level exists to be loaded.") { }
	ENoSuchLevel(const std::string &str) : NLMISC::Exception( str ) {}
};

#endif // NL_NELOID_EXCEPTIONS_H