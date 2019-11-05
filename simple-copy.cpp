/*Algorithm for breaking up file referenced from the original author here:
https://gist.github.com/andschwa/d120be76ba8ebd66cf50*/ 

#include <string.h>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <climits>

using namespace std;

void print_help(){

        printf("\n");
        printf("________________________________________________________________________ \n");
        printf("|                                                                      | \n");
        printf("|                  Simple file copy (less that 2gb)                    | \n");
        printf("|                                                                      | \n");
        printf("|                  Usage: ./simple-copy file destination               | \n");
        printf("|                  Example: ./simple-copy myfile /home/testuser/       | \n");
        printf("|                                                                      | \n");
        printf("|  ------------------------------------------------------------------  | \n");
        printf("|                  Few caveats:                                        | \n");
        printf("|                                                                      | \n");
        printf("|                  Do not use full path for file                       | \n");
        printf("|                  for destination a trailing / must be included       | \n");
        printf("|______________________________________________________________________| \n");


}


size_t whats_my_size(string filepass) {
    ifstream fcheck(filepass.c_str(), ios::binary);
        //check added to main below is redundant:
        /*if(!fcheck){
            cout << " ERROR --  Unable to open file for transfer: " << filepass.c_str() << ".\n";
            return 0;
        }*/
    struct stat passedfile;
    if(stat(filepass.c_str(), &passedfile) != 0) {
        return 0;
    }
    return passedfile.st_size;   
}

void simple_copy_file_req(string sourcefile, string destdirectory, string copyfilefullpath){
    if (mkdir(destdirectory.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1){
        if( errno == EEXIST ) {
            goto runNormalCopy;
        }
        else {
            cout << "Unable to create directory: " << destdirectory.c_str() << ".\n";
            exit(0);
        }
    }
    else {
        cout << "Directory created: " << destdirectory.c_str() << ".\n";
    }
    runNormalCopy:
    ifstream canopensourcef(sourcefile.c_str(), ios::binary);
        if(!canopensourcef){
            cout << " Unable to open sourcefile " << sourcefile.c_str() << " for copy to directory: " << copyfilefullpath.c_str() << ".\n";
            exit(0);
        }
    ofstream canopendestf;
    canopendestf.open(copyfilefullpath.c_str(), ios::out | ios::binary);
        if(!canopendestf){
            cout << " Unable to write to directory " << sourcefile.c_str() << "for copy of sourcefile: " << copyfilefullpath.c_str() << ".\n";
            exit(0);
        }
    size_t wfilesize = whats_my_size(sourcefile.c_str());
        if (wfilesize > INT_MAX){
            cout << " Source file " << sourcefile.c_str() << "too large for copy to directory: " << copyfilefullpath.c_str() << ".\n";
            exit(0);
        }

    size_t slabsize = 4 * 1024;
    size_t totalslabs = wfilesize / slabsize;
    size_t finalslabsize = wfilesize % slabsize;
    if (finalslabsize !=0){
        ++totalslabs;
    }
    else {
        finalslabsize = slabsize;
    }
    for (size_t slab = 0; slab < totalslabs; slab++){
        size_t currentslabsize =
        slab == totalslabs - 1
        ? finalslabsize
        : slabsize;

        int currentslabmovesize = static_cast<int>(currentslabsize);

        vector<char> slabbindata(currentslabsize);
        canopensourcef.read(&slabbindata[0], currentslabsize);
        char readingbuffer[currentslabmovesize];
        copy(slabbindata.begin(), slabbindata.end(), readingbuffer);
        
        canopendestf.write(&slabbindata[0], currentslabsize);
        


        }
        canopensourcef.close();
        canopendestf.close();
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
    string suppliedfile(argv[1]);
    string supplieddestination(argv[2]);
    stringstream ssfullpath;
    ssfullpath << supplieddestination.c_str() << suppliedfile.c_str();
    string passpath = ssfullpath.str();
    simple_copy_file_req(suppliedfile.c_str(), supplieddestination.c_str(), passpath.c_str());
    cout << "New file created: " << passpath << "\n";
    return 0;

}
