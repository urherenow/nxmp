#include "localFs.h"


localFs::localFs(std::string _path,Playlist * _playlist){
	currentpath = _path;
	playlist = _playlist;
}

localFs::~localFs(){
	
}

std::string localFs::getCurrentPath(){
	return currentpath;
}

std::vector<FS::FileEntry> localFs::getCurrList(){
	return currentlist;
}

void localFs::clearChecked(){
	for(int i=0;i<currentlist.size();i++){
		currentlist[i].checked = false;
	}
}

bool *localFs::checked(int pos){
	return &currentlist[pos].checked;
}

void localFs::DirList(const std::string &path,bool showHidden,const std::vector<std::string> &extensions) {

		currentpath = path;
		currentlist.clear();
		struct dirent *ent;
		DIR *dir;

		if (!path.empty()) {
			if ((dir = opendir(path.c_str())) != nullptr) {
				while ((ent = readdir(dir)) != nullptr) {
					if ((path == "/" || strlen(ent->d_name) == 1) && ent->d_name[0] == '.') {
						continue;
					}
					if ((path == "/" || strlen(ent->d_name) == 2) && ent->d_name[0] == '.' && ent->d_name[1] == '.') {
						continue;
					}
					if (!showHidden && ent->d_name[0] == '.') {
						if (strlen(ent->d_name) != 2 && ent->d_name[1] != '.') {
							continue;
						}
					}

					FS::FileEntry file;
					file.name = ent->d_name;
					file.path = FS::removeLastSlash(path) + "/" + file.name;
					file.checked = playlist->isPresent(file,file.path);
					
					
					
					struct stat st{0};
					if (stat(file.path.c_str(), &st) == 0) {
						file.size = (size_t) st.st_size;
						file.type = S_ISDIR(st.st_mode) ? FS::FileEntryType::Directory : FS::FileEntryType::File;
						file.is_valid = 0;
						file.created = (time_t)st.st_ctime;
						file.modified = (time_t)st.st_mtime;
						file.accessed = (time_t)st.st_atime;
					}
					
					
					if(file.type == FS::FileEntryType::File){
						bool isMediafile = false;
						for (auto &ext : extensions) {
							if (Utility::endsWith(file.name, ext, false)) {
								isMediafile = true;
							}
						}
						if(isMediafile){
							currentlist.push_back(file);
						}
					}else if(file.type == FS::FileEntryType::Directory){
						currentlist.push_back(file);
					}

				}
			
				closedir(dir);
				if(sortOrder == FS::FS_NAME_ASCENDINGORDER){
					std::sort(currentlist.begin(), currentlist.end(), FS::SortNameAsc);
				}
				if(sortOrder == FS::FS_NAME_DESCENDINGORDER){
					std::sort(currentlist.begin(), currentlist.end(), FS::SortNameDesc);
				}
				/*
				if(sortOrder == FS::FS_DATE_ASCENDINGORDER){
					std::sort(currentlist.begin(), currentlist.end(), FS::SortDateAsc);
				}
				if(sortOrder == FS::FS_DATE_DESCENDINGORDER){
					std::sort(currentlist.begin(), currentlist.end(), FS::SortDateDesc);
				}
				*/
				if(sortOrder == FS::FS_SIZE_ASCENDINGORDER){
					std::sort(currentlist.begin(), currentlist.end(), FS::SortSizeAsc);
				}
				if(sortOrder == FS::FS_SIZE_DESCENDINGORDER){
					std::sort(currentlist.begin(), currentlist.end(), FS::SortSizeDesc);
				}
				
			}
		}

	}
	
	void localFs::backPath(){
		currentpath = currentpath.substr(0, currentpath.find_last_of("\\/"));
		if(currentpath == "")currentpath = "/";

	}
	
