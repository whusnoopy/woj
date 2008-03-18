#ifndef _FLOOD_SERVER_DATA_FILEDATA_H__
#define _FLOOD_SERVER_DATA_FILEDATA_H__

#include <string>
using namespace std;

typedef struct _FILE_DATA_{
	string filename;
	char * buf;
}FileData;

#endif /*FILEDATA_H_*/
