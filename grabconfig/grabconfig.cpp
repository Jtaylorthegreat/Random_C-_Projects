#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "grabconfig.h"

using namespace std;


string ASSIGNEDVALUE1, ASSIGNEDVALUE2, ASSIGNEDVALUE3, ASSIGNEDVALUE4;

//void grab_config(ifstream& in, configfile& out){
void grab_config(string filename, ifstream& in, configfile& out){
    //in.open(".agentconfig");
    in.open(filename);
    string line;
    while(!in.eof()){
        while(getline(in,line)){
            line.erase(remove_if(line.begin(), line.end(), ::isspace),
                                    line.end());
                if(line[0] == '#' || line.empty())
                    continue;
            auto delimiterPos = line.find("=");
            string name;
            string debugg;
            name = line.substr(0, delimiterPos);
            if(name == "confvalue1")
                out.loadedstructvalue1 = line.substr(delimiterPos +1);
            if(name == "confvalue2")
                out.loadedstructvalue2 = line.substr(delimiterPos +1);
            if(name == "confvalue3")
                out.loadedstructvalue3 = line.substr(delimiterPos +1);
            if(name == "confvalue4")
                out.loadedstructvalue4 = line.substr(delimiterPos +1);
            
        }
    }
}


void print_help(){
	printf("\n");
	printf("Usage: ./grabconfig configfile \n");
}



int main(int argc, char *argv[]){

    if (argc < 2){
    	print_help();
    	return -1;
    }
    if (argc > 2){
    	print_help();
    	return -1;
    }
    string suppliedfile(argv[1]);
	ifstream openconf1;
	configfile outp;
	grab_config(suppliedfile, openconf1, outp);
	openconf1.close();

    ASSIGNEDVALUE1 = outp.loadedstructvalue1;
    ASSIGNEDVALUE2 = outp.loadedstructvalue2;
    ASSIGNEDVALUE3 = outp.loadedstructvalue3;
    ASSIGNEDVALUE4 = outp.loadedstructvalue4;

    cout << "Loaded values: \n";
    cout << ASSIGNEDVALUE1 << "\n";
    cout << ASSIGNEDVALUE2 << "\n";
    cout << ASSIGNEDVALUE3 << "\n";
    cout << ASSIGNEDVALUE4 << "\n";
    return 0;

	}
