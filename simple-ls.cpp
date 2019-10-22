#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

typedef std::vector<std::string> stringvec;
 
void read_directory(const std::string& dirname, stringvec& vec)
{
    DIR* direc = opendir(dirname.c_str());
    struct dirent * dp;
    while ((dp = readdir(direc)) != NULL) {
        //vec.push_back(dp->d_name);
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        	vec.push_back(dp->d_name);
    }
    closedir(direc);
}

int main(int argc, char *argv[]) {
	if (argc < 2){
		cout << "Usage: ./simple-ls directory" << endl;
		return -1;
	}
	vector<string> vect;
	int numoffiles;
	const string passdir(argv[1]);
	read_directory(passdir.c_str(), vect);
	numoffiles = vect.size();
	cout << "num of items in " << passdir << ": " << numoffiles << endl;
	for (int i = 0; i < numoffiles; i++){
		cout << vect[i] << endl;
	}
	return 0;
}
