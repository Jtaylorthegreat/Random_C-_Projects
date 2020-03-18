#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <random>
#include <functional> 
#include <algorithm> 
#include <chrono>
#include <termios.h>

#include "mysql_connection.h"
#include "mysql_driver.h"
#include "mysql_error.h"
#include <mysql.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

using namespace std;
using namespace sql;


typedef vector<char> char_array;
 
char_array charset()
{
    return char_array( 
	{'0','1','2','3','4',
	'5','6','7','8','9',
	'A','B','C','D','E','F',
	'G','H','I','J','K',
	'L','M','N','O','P',
	'Q','R','S','T','U',
	'V','W','X','Y','Z',
	'a','b','c','d','e','f',
	'g','h','i','j','k',
	'l','m','n','o','p',
	'q','r','s','t','u',
	'v','w','x','y','z'
	});
};    
 
string random_string( size_t length, function<char(void)> rand_char )
{
    std::string str(length,0);
    std::generate_n( str.begin(), length, rand_char );
    return str;
}

void hideinput(){
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}
void reenableinput(){
    termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

int main(int argc, char *argv[]) {
	if (argc < 4) {
        	cout << "Usage: ./stress-db user sqlserver runtime" << endl;
        	cout << "Example to run for 3 minutes:" << endl;
        	cout << "Example: ./stress-db user1 localhost 3" << endl;
		   	return -1;
   	}
	
	if (argc > 4) {
        	cout << "Usage: ./stress-db user  sqlserver runtime" << endl;
        	cout << "Example to run for 3 minutes:" << endl;
        	cout << "Example: ./stress-db user1 localhost 3" << endl;
        	return -1;
   	}

	string dbuser(argv[1]);
	string dbserver(argv[2]);
	string runtimeinput(argv[3]);
	string dbname = "stressdatabase";
	int runtime;
	try {
	    runtime = stoi(runtimeinput);
	}
	catch (std::invalid_argument) {
		cout << "Invalid time value submitted" << endl;
		return -1;
	}

	string dbpass;
	cout << "Enter Password" << endl;
	hideinput();
	getline (cin, dbpass);
	reenableinput();

	const auto ch_set = charset();
	default_random_engine rng(std::random_device{}()); 
    uniform_int_distribution<> dist(0, ch_set.size()-1);
    auto randchar = [ ch_set,&dist,&rng ](){return ch_set[ dist(rng) ];};
    auto namelength = 50;
    auto emaillength = 60;

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		driver = get_driver_instance();
		con = driver->connect(dbserver.c_str(), dbuser.c_str(), dbpass.c_str());
		
  		stmt = con->createStatement();
  		stmt->execute("DROP DATABASE IF EXISTS stressdatabase");
  		stmt->execute("CREATE DATABASE stressdatabase");
  		stmt->execute("CREATE TABLE stressdatabase.stresstable (userloginname varchar(50), emailaddress varchar(60), uniquekey char(36), usertime TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
       	delete stmt;
  		delete con;
	}  catch (sql::SQLException &e) {
           cout << "# ERR: " << e.what() << endl;
           cout << "Mysql error code: " << e.getErrorCode() << endl;
           cout <<  "SQLState: " << e.getSQLState() << endl;
           return -1;
        }

	cout << "Database & Table successfully created" << endl;
	cout << "Running Stress Test for " << runtimeinput << " minutes" << endl;


	auto finish = chrono::system_clock::now() +chrono::minutes(runtime);

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		driver = get_driver_instance();
		con = driver->connect(dbserver.c_str(), dbuser.c_str(), dbpass.c_str());
		con->setSchema(dbname.c_str());
		con-> setAutoCommit(0);
  		stmt = con->createStatement();


  		stringstream ssrandomname;
		stringstream ssrandomemail;
    	string randomname, randomemail;
	    	
    	ssrandomname << random_string(namelength,randchar);
    	ssrandomemail << random_string(emaillength,randchar);
    	randomname = ssrandomname.str();
    	randomemail = ssrandomemail.str();


  		stringstream formatsqlupdate;
  		formatsqlupdate << "INSERT INTO stresstable(userloginname, emailaddress, uniquekey, usertime) VALUES ('" << randomname <<  "', '" << randomemail << "', uuid(), CURRENT_TIMESTAMP)";
	    string updatestatement = formatsqlupdate.str();

		do {

			    stmt->execute("START TRANSACTION;");
	            stmt->execute(updatestatement.c_str());
	            stmt->execute("COMMIT;");
	       	
	        
		} while (chrono::system_clock::now() < finish);

	    delete stmt;
	  	delete con;
	}  catch (sql::SQLException &e) {
	    cout << "# ERR: " << e.what() << endl;
	    cout << "Mysql error code: " << e.getErrorCode() << endl;
	    cout <<  "SQLState: " << e.getSQLState() << endl;
	    return -1;
	}

		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;
			driver = get_driver_instance();
			con = driver->connect(dbserver.c_str(), dbuser.c_str(), dbpass.c_str());
			con->setSchema(dbname.c_str());

	  		stmt = con->createStatement();
			res = stmt->executeQuery("select avg(math.timespersecond) as averagenumber from (select usertime,COUNT(usertime) as timespersecond from stresstable GROUP BY usertime ORDER BY timespersecond DESC) math");
	  			while (res->next()){
	  				string averagecommits = string(res->getString("averagenumber"));
	  				cout << "Average commits per second: " << averagecommits << endl;
	  			}
	  		delete res;
	  		res = stmt->executeQuery("select count(*) from stresstable");
	  			while (res->next()){
	  				string totalcommits = string(res->getString("count(*)"));
	  				cout << "Total commits performed: " << totalcommits << endl;
	  			}
	  		delete res;
	       	delete stmt;
	  		delete con;
		}  catch (sql::SQLException &e) {
	           cout << "# ERR: " << e.what() << endl;
	           cout << "Mysql error code: " << e.getErrorCode() << endl;
	           cout <<  "SQLState: " << e.getSQLState() << endl;
	           return -1;
	        }

	    try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			driver = get_driver_instance();
			con = driver->connect(dbserver.c_str(), dbuser.c_str(), dbpass.c_str());
			stmt = con->createStatement();
			stmt->execute("DROP DATABASE stressdatabase");
		   	delete stmt;
			delete con;
		}  catch (sql::SQLException &e) {
		    cout << "# ERR: " << e.what() << endl;
		    cout << "Mysql error code: " << e.getErrorCode() << endl;
		    cout <<  "SQLState: " << e.getSQLState() << endl;
	        cout << "Error removing database: stressdatabase" << endl;
	        return -1;
	    }
	cout << "Successfully removed database for testing" << endl;



  	return 0;

}
