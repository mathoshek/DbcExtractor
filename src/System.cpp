#include "System.h"

System::System()
{
	nError = ERROR_SUCCESS;
	lang = -1;
}

System::~System()
{
	//
}

void System::SayHello()
{
	printf("=================================================\n");
	printf("==============Welcome to DbcExtractor============\n");
	printf("===================by Mathoshek==================\n");
	printf("=================================================\n\n");
}

void System::HandleArguments( int argc, char* argv[] )
{
	Homepath = argv[0];
	Homepath = Homepath.substr( 0, Homepath.find_last_of( "/" ) + 1 );

	for( int32 i = 1; i < argc; i++ )
	{
		if( argv[i][0] == '-' )
		{
			switch( argv[i][1] )
			{
			case 'd':
				if(i + 1 < argc)
				{
					WorkDir = argv[i + 1];
					i++;
				}
				else
					ShowUsage(argv[0]);
				break;
			default:
				ShowUsage( argv[0] );
			}
		}
		else
			ShowUsage( argv[0] );
	}
	
	if( WorkDir.empty() == true )
		WorkDir = "./";
	else if( WorkDir[WorkDir.size( ) - 1] != '/')
		WorkDir += "/";
	
	for(int32 i = 0; i <= NUMBER_OF_LOCALES; i++)
	{
		char tmp1[512];
		sprintf(tmp1, "%sData/%s/locale-%s.MPQ", WorkDir.c_str(), langs[i], langs[i]);
		if(FileExists(tmp1) == true)
		{
			lang = i;
			printf("Detected locale: %s\n", langs[lang]);
			break;
		}
	}
	if (lang == -1)
	{
		printf("No locale found!!!\n");
		exit(1);
	}
}

void System::ShowUsage(char* prg)
{
	printf(
		"Usage:\n"\
		"%s -[var] [value]\n"\
		"-d set the wow folder (OPTIONAL - default: ./)\n"\
		"Example: %s -d \"D:/WoW Cata/\"\n", prg, prg);
	exit(1);
}

void System::ExtractDBCs()
{
	HANDLE hMpq = NULL;
	vector<string> PatchMpqFiles;
	PatchMpqFiles = FindPatchMPQFiles();

	string localepath = WorkDir;
	localepath += "Data/";
	localepath += langs[lang];
	localepath += "/locale-";
	localepath += langs[lang];
	localepath += ".MPQ";
	string DbcFolder = Homepath;
	DbcFolder += "dbc/";
	CreateDir( DbcFolder.c_str( ) );

	// Open the primary MPQ
	printf("Opening %s ...\n", localepath.c_str());
	if(!SFileOpenArchive(localepath.c_str(), 0, MPQ_OPEN_READ_ONLY, &hMpq))
		printf("Failed to open the archive %s: Error:%d\n", localepath.c_str(), GetLastError());

	// Add all patches
	if(nError == ERROR_SUCCESS)
	{
		for(vector<string>::iterator iter = PatchMpqFiles.begin(); iter != PatchMpqFiles.end(); ++iter)
		{
			printf("Adding patch %s ...\n", iter->c_str());
			if(!SFileOpenPatchArchive(hMpq, iter->c_str(), NULL, 0))
				printf("Failed to add patch %s: %d ... \n", iter->c_str(), GetLastError());
		}
	}

	string component = "component.wow-";
	component += langs[lang];
	component += ".txt";
	SFileExtractFile(hMpq, component.c_str( ), (DbcFolder + component).c_str( ), SFILE_OPEN_PATCHED_FILE);
	
	int cnt = 0;
	SFILE_FIND_DATA sf;
	HANDLE hFind;
	bool bResult = true;

	printf("Extracting DBC Files%*d", 6, cnt);
	hFind = SFileFindFirstFile(hMpq, "DBFilesClient\\*", &sf, NULL);
	while(hFind && bResult)
	{
		string fname = DbcFolder + (sf.cFileName + 14);
		SFileExtractFile(hMpq, sf.cFileName, fname.c_str(), SFILE_OPEN_PATCHED_FILE);
		printf("\rExtracting DBC Files%*d", 6, cnt);
		bResult = SFileFindNextFile(hFind, &sf);
		cnt++;
	}
	printf("\rFinished extracting %d files\n", cnt);
	
	if(hMpq != NULL)
		SFileCloseArchive(hMpq);
}

vector<string> System::FindPatchMPQFiles()
{
	vector<string> PatchMpqFiles;
	string path = WorkDir;
	path += "Data/";
	path += langs[lang];
	path += "/";
	
	const char *p = GetFileInDir( path.c_str( ) );
	while( p != NULL )
	{
		if( strstr( p, ".MPQ" ) )
			if( strstr(p, "wow-update-" ) )
			{
				if( strstr( p, "oldworld" ) )
					continue;

				PatchMpqFiles.push_back( path + p );
			}
		p = GetFileInDir( NULL );
	}
	return PatchMpqFiles;
}

bool System::CreateDir( const char* Path )
{
#ifdef _WIN32
	if( CreateDirectory( Path, NULL ) == TRUE )
		return true;
#elif __unix__
	if( mkdir( Path, 0777 ) == 0 )
		return true;
#else
	//
#endif
	return false;
}

bool System::FileExists( const char* FileName )
{
	FILE *f = fopen( FileName, "rb" );
	if( f != NULL )
	{
		fclose( f );
		return true;
	}
	return false;
}

const char* System::GetFileInDir( const char *dir )
{
#ifdef _WIN32
	if( dir != NULL )
	{
		char directory[256];
		strcpy( directory, dir );
		strcat( directory, "*" );
		hFind = FindFirstFile( directory, &findFileInfo );
		if( hFind == INVALID_HANDLE_VALUE )
			return NULL;
	}
	BOOL test = FindNextFile( hFind, &findFileInfo );
	while( test == TRUE && ( findFileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
		test = FindNextFile( hFind, &findFileInfo );
	if( test == FALSE )
	{
		FindClose( hFind );
		return NULL;
	}
	return findFileInfo.cFileName;
#elif __unix__
	if( dir != NULL )
	{
		dirp = opendir( dir );
		if( dirp == NULL )
			return NULL;
	}
	dp = readdir( dirp );
	while( dp != NULL && dp->d_type != DT_REG )
		dp = readdir( dirp );
	if( dp == NULL )
	{
		closedir( dirp );
		return NULL;
	}
	return dp->d_name;
#else
	return NULL;
#endif
}

