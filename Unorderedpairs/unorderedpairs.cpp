/* Copyright 2020 Justin Taylor */

   /*This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Justin Taylor <taylor.justin199@gmail.com> */
/* Inspired by "counting interations" example on page 46 of Cracking the coding interview 6th Edition demonstrating O(N²) runtimes */


  
  #include <iostream>
	#include <string>
	#include <string.h>
	#include <array>

	using namespace std;


	void printunorderedPairs(int dataset) {  
		int exporti;
		int exportj;
		for (int i = 0; i < dataset; i++) {  
			for (int j =  i + 1; j < dataset; j++) { 
				cout << i << " , " << j << "\n";
				exporti = i;
				exportj = j;
			}  
		}  
		cout << " " << endl;
		cout << "final set: " << endl;
		cout << exporti << " , " << exportj << "\n";
	}


	int main(int argc, char *argv[]){
		if (argc < 2) {
			cout << "Usage: ./unorderedpairs count" << endl;
			return -1;
		}
		if (argc > 2) {
			cout << "Usage: ./unorderedpairs count" << endl;
			return -1;
		}
		string inputnumber(argv[1]);
		int setnumber;
		try {
			setnumber = stoi(inputnumber);
		}
		catch (invalid_argument){
			cout << "Invalid number submitted" << endl;
			return -1;
		}
		printunorderedPairs(setnumber);
		return 0;
	}
