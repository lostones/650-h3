#include <iostream>
#include <pqxx/pqxx>
#include <cstring>
#include "exerciser.h"
#include <fstream>
using namespace std;
using namespace pqxx;


const int MAX_CHARS_PER_LINE = 512;
const int MAX_TOKENS_PER_LINE = 12;
const char* const DELIMITER = " ";


int transferFile(char * name, work *W);

int main (int argc, char *argv[]) 
{

 //

  //Allocate & initialize a Postgres connection object
  connection *C;
  //create a transactional object
  work *W;



	try{
	  //Establish a connection to the database
	  //Parameters: database name, user name, user password
	  C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
	    if (C->is_open()) {
	      cout << "Opened database successfully: " << C->dbname() << endl;
	    } else {
	      cout << "Can't open database" << endl;
	      return 1;
	     }
	  } catch (const std::exception &e){
	    cerr << e.what() << std::endl;
	    return 1;
	  }


//TODO: IMPLEMENT QUERY FUNCTIONS
	char * dropTables;

dropTables = (char *) "DROP TABLE IF EXISTS PLAYER, TEAM, STATE, COLOR;";

	char * crPlayer;
	char * crTeam;
	char * crState;
	char * crColor;
  
crPlayer = (char *) "CREATE TABLE PLAYER(" 
	"PLAYER_ID 	SERIAL	PRIMARY KEY NOT NULL," 
	"TEAM_ID	INT	DEFAULT -1 NOT NULL ," //REFERENCES TEAM(TEAM_ID) ," 
	"UNIFORM_NUM	INT	DEFAULT -1 NOT NULL," 
	"FIRST_NAME	TEXT	DEFAULT 'DEF' NOT NULL," 
	"LAST_NAME	TEXT	DEFAULT 'DEF' NOT NULL," 
	"MPG		INT	DEFAULT -1 NOT NULL," 
	"PPG		INT	DEFAULT -1 NOT NULL," 
	"RPG		INT	DEFAULT -1 NOT NULL," 
	"APG		INT	DEFAULT -1 NOT NULL," 
	"SPG		REAL	DEFAULT -1.0 NOT NULL," 
	"BPG		REAL	DEFAULT -1.0 NOT NULL);";

crTeam = (char *) "CREATE TABLE TEAM(" 
	"TEAM_ID	SERIAL PRIMARY KEY NOT NULL," 
	"NAME		TEXT	DEFAULT 'DEF' NOT NULL," 
	"STATE_ID	INT	DEFAULT -1 NOT NULL ," //REFERENCES STATE (STATE_ID)," 
	"COLOR_ID	INT	DEFAULT -1 NOT NULL ," //REFERENCES COLOR (COLOR_ID),"
	"WINS		INT	DEFAULT -1 NOT NULL,"
	"LOSSES		INT	DEFAULT -1 NOT NULL);";

crState = (char *) "CREATE TABLE STATE("
	"STATE_ID	SERIAL PRIMARY KEY NOT NULL,"
	"NAME		TEXT	DEFAULT 'DEF' NOT NULL);";

crColor = (char *) "CREATE TABLE COLOR("
	"COLOR_ID	SERIAL PRIMARY KEY NOT NULL,"
	"NAME		TEXT	DEFAULT 'DEF' NOT NULL);";


//cout << "C is: " << C << endl;
	W = new work(*C);


//Drop all pre-existing tables
	W->exec(dropTables);

	//cout << "successfully dropped pre-existing tables!" << endl;

//cout << "table: removed successfully" << endl;
//Create fresh tables, commit at end of set of creations
	
	W->exec(crState);
	W->exec(crColor);
	W->exec(crTeam);
	W->exec(crPlayer);

	W->commit();

  //cout << "table create successfully" << endl;

// Begin populating the tables
// Step 1 read the player text file 
	const char * fileArray[4] = {"state.txt", "color.txt", "team.txt", "player.txt"};
	int i;
	for(i = 0; i < 4; i++){
	
		W = new work(*C);
		transferFile((char *)fileArray[i], W);
		W->commit();
	}  
	

  exercise(C);


  //Close database connection
  C->disconnect();

  return 0;
}

int transferFile(char * name, work *W){
	
	size_t strsize;
	char  tbname[7] = {};
	char  sqlInsert[256];
	char * set;
	ifstream fin;
	fin.open(name); // open a file
 	if (!fin.good()) 
    	  return 1; // exit if file not found
	
  	strsize = strcspn(name, ".");
	
	strncpy(tbname, name, strsize);
	//cout << "file: " << name << " is parsed into: " << tbname << endl;
 	while(!fin.eof()){

		 // read an entire line into memory
	    char buf[MAX_CHARS_PER_LINE];
	    fin.getline(buf, MAX_CHARS_PER_LINE);
	    
	    // parse the line into blank-delimited tokens
	    int n = 0; // a for-loop index
	    
	    // array to store memory addresses of the tokens in buf
	    const char* token[MAX_TOKENS_PER_LINE] = {}; // initialize to 0
	    
	    // parse the line
	    token[0] = strtok(buf, DELIMITER); // first token
	    if (token[0]) // zero if line is blank
	    {
	      for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
	      {
		token[n] = strtok(0, DELIMITER); // subsequent tokens
		if (!token[n]) break; // no more tokens
	      }
	    }
	
    	// process (print) the tokens
		if(n == 11){
		   sprintf(sqlInsert, "INSERT INTO %s VALUES ( DEFAULT, %s, %s, '%s', '%s', %s, %s, %s, %s, %s, %s);", tbname, token[1], token[2], token[3], token[4], token[5], token[6], token[7], token[8], token[9], token[10]);
		   W->exec(sqlInsert);
  		} else if( n == 6){
		   sprintf(sqlInsert, "INSERT INTO %s VALUES (DEFAULT, '%s', %s, %s,%s, %s);", tbname, token[1], token[2], token[3], token[4], token[5]);
		   W->exec(sqlInsert);
		} else if(n == 2){
		   sprintf(sqlInsert, "INSERT INTO %s VALUES (DEFAULT, '%s');", tbname, token[1]);
		   W->exec(sqlInsert);
		}
 	}	
}

