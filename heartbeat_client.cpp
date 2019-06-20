#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fstream>
using namespace std;

int main()
{
	string agentconfig = ".agentconfig";
	ifstream fin(agentconfig.c_str());
		if(!fin) {
			cout << "Agent config file not found" << ".\n";
			cout << "Initialzing setup" << ".\n";
			string interactiveinput;
			ofstream settingfile(".agentconfig");
			cout << "please enter the heartbeat server IP address ";
			getline (cin, interactiveinput);
			settingfile << interactiveinput << endl;
			cout << "please enter the heartbeat server port ";
			getline (cin, interactiveinput);
			settingfile << interactiveinput << endl;
			cout << "Please enter unique 8 character key ";
			getline (cin, interactiveinput);
			settingfile << interactiveinput << endl;
			settingfile.close();
		}
	
	while (true) {	
		
		int csocket = socket(AF_INET, SOCK_STREAM, 0);
		if (csocket == -1) {
			cerr << "Can't create socket! Exiting" << endl;
			return -1;
		}
		
	ifstream infile;
	string IP, PORT, KEY;
		infile.open(".agentconfig");
		getline(infile, IP, '\n');
		getline(infile, PORT, '\n');
		getline(infile, KEY, '\n');
		infile.close();
		
		int servPORT = stoi(PORT);
		sockaddr_in tech;
		tech.sin_family = AF_INET;
		tech.sin_port = htons(servPORT);
		inet_pton(AF_INET, IP.c_str(), &tech.sin_addr);
		
	int conreq = connect(csocket, (sockaddr*)&tech, sizeof(tech));
	if (conreq < 0 ){
		cerr << "error connecting" << endl;
		return -1;
	}
	char buf [4096];
	int sendmessage = send(csocket, KEY.c_str(), KEY.size(), 0);
	if (sendmessage < 0){
		cerr << "Error sending" << endl;
		return -1;
	}
		/*keeping socket open*/
		//close(csocket);
    sleep(30);
	}
	
	

}
