#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "defines.h"

#define NUMBER_OF_LOCALES 6
static const char* langs[] = {"enGB", "enUS", "deDE", "esES", "frFR", "ruRU" };
static int lang;

class System
{
public:
	System();
	~System();

	void SayHello( );
	void HandleArguments( int argc, char* argv[] );
	void ExtractDBCs( );

private:
	void ShowUsage( char* prg );
	vector<string> FindPatchMPQFiles( );
	
	bool CreateDir( const char* Path );
	bool FileExists( const char* FileName );
	const char *GetFileInDir( const char *dir );
#ifdef _WIN32
	HANDLE hFind;
	WIN32_FIND_DATA findFileInfo;
#elif __unix__
	DIR *dirp;
	struct dirent *dp;
#else
	//
#endif

	int nError;
	
	string Homepath;
	string WorkDir;
};

#endif // __SYSTEM_H__
