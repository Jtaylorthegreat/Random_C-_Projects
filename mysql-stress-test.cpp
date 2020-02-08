#include <iostream>
#include <string.h>
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


int main(int argc, char *argv[]) {
	if (argc < 6) {
        	cout << "Usage: ./stress-db database user password sqlserver runtime" << endl;
        	cout << "Example to run for 3 minutes:" << endl;
        	cout << "Example: ./stress-db testdatabase user1 password123 localhost 3" << endl;
        	return -1;
   	}
	
	if (argc > 6) {
        	cout << "Usage: ./stress-db database user password sqlserver runtime" << endl;
        	cout << "Example to run for 3 minutes:" << endl;
        	cout << "Example: ./stress-db testdatabase user1 password123 localhost 3" << endl;
        	return -1;
   	}


	string dbname(argv[1]);
    string dbuser(argv[2]);
    string dbpass(argv[3]);
	string dbserver(argv[4]);
	string runtimeinput(argv[5]);
	int runtime;
	runtime = stoi(runtimeinput);

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
		//sql::ResultSet *res;
		//sql::PreparedStatement *pstmt;  
		driver = get_driver_instance();
		con = driver->connect(dbserver.c_str(), dbuser.c_str(), dbpass.c_str());
		con->setSchema(dbname.c_str());

  		stmt = con->createStatement();
  		stmt->execute("DROP TABLE IF EXISTS stresstable");
  		stmt->execute("CREATE TABLE stresstable (userloginname varchar(50), emailaddress varchar(60), uniquekey char(36), usertime TIMESTAMP DEFAULT CURRENT_TIMESTAMP)");
       	delete stmt;
  		delete con;
	}  catch (sql::SQLException &e) {
           cout << "# ERR: " << e.what() << endl;
           cout << "Mysql error code: " << e.getErrorCode() << endl;
           cout <<  "SQLState: " << e.getSQLState() << endl;
           return -1;
        }

	cout << "Table successfully created" << endl;
	cout << "running Stress Test for " << runtimeinput << " minutes" << endl;
	//auto finish = chrono::system_clock::now() + 3min;
	//auto finish = chrono::system_clock::now() + minutes{3};
	typedef chrono::duration<int, ratio<60,1> > minutes;
	//auto finish = chrono::system_clock::now() +chrono::minutes(1);
	auto finish = chrono::system_clock::now() +chrono::minutes(runtime);
	do {

		stringstream ssrandomname;
		stringstream ssrandomemail;
    	string randomname, randomemail;
    	
    	ssrandomname << random_string(namelength,randchar);
    	ssrandomemail << random_string(emaillength,randchar);
    	randomname = ssrandomname.str();
    	randomemail = ssrandomemail.str();

		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			//sql::ResultSet *res;
			driver = get_driver_instance();
			con = driver->connect(dbserver.c_str(), dbuser.c_str(), dbpass.c_str());
			con->setSchema(dbname.c_str());

	  		stmt = con->createStatement();
	  		stringstream formatsqlupdate;
	  		formatsqlupdate << "INSERT INTO stresstable(userloginname, emailaddress, uniquekey, usertime) VALUES ('" << randomname <<  "', '" << randomemail << "', uuid(), CURRENT_TIMESTAMP)";
            string updatestatement = formatsqlupdate.str();
            stmt->execute("START TRANSACTION;");
            stmt->execute(updatestatement.c_str());
            stmt->execute("COMMIT;");
	       	delete stmt;
	  		delete con;
		}  catch (sql::SQLException &e) {
	           cout << "# ERR: " << e.what() << endl;
	           cout << "Mysql error code: " << e.getErrorCode() << endl;
	           cout <<  "SQLState: " << e.getSQLState() << endl;
	           return -1;
	        }
	        
	} while (chrono::system_clock::now() < finish);

		try {
			sql::Driver *driver;
			sql::Connection *con;
			sql::Statement *stmt;
			sql::ResultSet *res;
			//sql::PreparedStatement *pstmt;  
			driver = get_driver_instance();
			con = driver->connect(dbserver.c_str(), dbuser.c_str(), dbpass.c_str());
			con->setSchema(dbname.c_str());

	  		stmt = con->createStatement();
			res = stmt->executeQuery("select avg(math.timespersecond) as averagenumber from (select usertime,COUNT(usertime) as timespersecond from stresstable GROUP BY usertime ORDER BY timespersecond DESC) math");
	  			while (res->next()){
	  				string averagecommits = string(res->getString("averagenumber"));
	  				//cout << "Average commits per second: " << res->getString(2) << endl;
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


  	return 0;

}
