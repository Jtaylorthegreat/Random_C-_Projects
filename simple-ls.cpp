#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <algorithm>

using namespace std;


typedef std::vector<std::string> stringvec;
 
void read_dirs_in_directory(const std::string& dirname, stringvec& vec)
{
	unsigned char isFolder =0x4;
	DIR* direc = opendir(dirname.c_str());
    struct dirent * dp;
    while ((dp = readdir(direc)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && dp->d_type == isFolder)
        	vec.push_back(dp->d_name);
    }
    closedir(direc);
}

void read_files_in_directory(const std::string& dirname, stringvec& vec)
{
	unsigned char isFile =0x8;
	DIR* direc = opendir(dirname.c_str());
    struct dirent * dp;
    while ((dp = readdir(direc)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0 && dp->d_type == isFile)
        	vec.push_back(dp->d_name);
    }
    closedir(direc);
}

void read_all_in_directory(const std::string& dirname, stringvec& vec)
{
	DIR* direc = opendir(dirname.c_str());
    struct dirent * dp;
    while ((dp = readdir(direc)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        	vec.push_back(dp->d_name);
    }
    closedir(direc);
}

void print_help(){
		printf("\n");
        printf("                             Simple Ls                                      \n");
        printf("\n");
        printf("                   Usage: ./simple-ls [option] location                     \n");
        printf("options:\n");
        printf("\n");
        printf(" -h                                                           show this help\n");
        printf(" -f                                                   show files in location\n");
        printf(" -d                                             show directories in location\n");
        printf(" -a                        show all (both files and directories) in location\n");
        printf("\n");
}

int main(int argc, char *argv[]) {
	if (argc < 3){
		print_help();
		return -1;
	}
	if (argc > 3){
		print_help();
		return -1;
	}
	string suppliedoption(argv[1]);
    if (suppliedoption == "-h") {
    	print_help();
    	return 0;
    }
    if (suppliedoption == "-f") {
    	vector<string> vect;
		int numoffiles =0;
		const string passdir(argv[2]);
		read_files_in_directory(passdir.c_str(), vect);
		numoffiles = vect.size();
		sort(vect.begin(), vect.end());
		cout << "num of files in " << passdir << ": " << numoffiles << endl;
		if (numoffiles > 0 ) {
			for (int i = 0; i < numoffiles; i++){
				cout << vect[i] << endl;
			}
		}
		return 0;
    }
    if (suppliedoption == "-d") {
    	vector<string> vect;
		int numoffiles =0;
		const string passdir(argv[2]);
		read_dirs_in_directory(passdir.c_str(), vect);
		numoffiles = vect.size();
		sort(vect.begin(), vect.end());
		cout << "num of directories in " << passdir << ": " << numoffiles << endl;
		if (numoffiles > 0 ) {
			for (int i = 0; i < numoffiles; i++){
				cout << vect[i] << endl;
			}
		}
		return 0;
    }
    if (suppliedoption == "-a") {
    	vector<string> vect;
		int numoffiles =0;
		const string passdir(argv[2]);
		read_all_in_directory(passdir.c_str(), vect);
		numoffiles = vect.size();
		sort(vect.begin(), vect.end());
		cout << "num of items in " << passdir << ": " << numoffiles << endl;
		if (numoffiles > 0 ) {
			for (int i = 0; i < numoffiles; i++){
				cout << vect[i] << endl;
			}
		}
		return 0;
    }



	
	return 0;
}
