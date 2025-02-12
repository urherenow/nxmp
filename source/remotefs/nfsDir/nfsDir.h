#ifndef NXMP_NFSDIR_H
#define NXMP_NFSDIR_H

#include <string>
#include <vector>
#include <stdio.h>
#include <cstring>
#include "localfiles.h"
#include "playlist.h"

class nfsDir{
public:
	nfsDir(std::string _url,Playlist * _playlist);
	
	void DirList(std::string path,bool showHidden,const std::vector<std::string> &extensions);
	std::vector<FS::FileEntry> getCurrList();
	
	void backDir();
	std::string getUrl();
	std::string getCurrPath();
	std::string getBasePath();
	
	bool *checked(int pos);
	void clearChecked();
	
	int sortOrder=0;
	
private:
	std::vector<FS::FileEntry> currentlist;
	std::string url;
	std::string basepath = "";
	std::string currentpath = "";
	Playlist *playlist;
	
};

#endif