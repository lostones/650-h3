#include "query_funcs.h"



void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{

	ostringstream ss;

	work *W;
	W = new work(*C);
	char str[] = "', '";

	ss << "INSERT INTO player VALUES ( DEFAULT, '"  << team_id << str << jersey_num << str << first_name << str << last_name << str << mpg << str << ppg << str << rpg << str << apg << str << spg << str << bpg << "');";
	
	W->exec(ss.str());
	W->commit();
	
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{

	ostringstream ss;

	work *W;
	W = new work(*C);
	char str[] = "', '";

	ss << "INSERT INTO team VALUES ( DEFAULT, '" << name << str << state_id << str << color_id << str << wins << str << losses << "');";

	W->exec(ss.str());
	W->commit();

}


void add_state(connection *C, string name)
{

	ostringstream ss;

	work *W;
	W = new work(*C);
	char str[] = "', '";

	ss << "INSERT INTO state VALUES ( DEFAULT, '" << name << "');";

	W->exec(ss.str());
	W->commit();
}


void add_color(connection *C, string name)
{
	
	ostringstream ss;
	work *W;
	W = new work(*C);
	char str[] = "', '";

	ss << "INSERT INTO color VALUES ( DEFAULT, '" << name << "');";

	W->exec(ss.str());
	W->commit();

}


void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
	//Approach for this problem; min/max used to evaluate are masked by bit from use_input
	
	ostringstream ss;

	int use_array[7] = {use_mpg, use_ppg, use_rpg, use_apg, use_spg, use_bpg, 0};
	double min_array[6] = {min_mpg, min_ppg, min_rpg, min_apg, min_spg, min_bpg};
	double max_array[6] = {max_mpg, max_ppg, max_rpg, max_apg, max_spg, max_bpg};
	const char * attr_array[6] = {"mpg", "ppg", "rpg", "apg", "spg", "bpg"};
	
	double min_temp, max_temp;
	 
	//cout << i << endl;
	ss << "SELECT * FROM PLAYER";

	for(int k = 0; k < 6; k++){
		if(k == 0 and use_array[k]== 1){
			ss << " WHERE ";
		}
		if(use_array[k] == 1){
			ss << "( " << attr_array[k] << " >= " << min_array[k] << " AND " << attr_array[k] << " <= " << max_array[k] << " )";
		}

		if(k != 5 and use_array[k+1] != 0){
			ss << " AND";
		}
		
	}
	
	ss << ";";
	
	
	/* Create a non-transactional object. */
	nontransaction *N;
	N = new nontransaction(*C);
   
      
    	/* Execute SQL query */
    	result *R;
	R = new result( N->exec( ss.str() ));
	cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;	

	for (result::const_iterator c = R->begin(); c != R->end(); ++c) {
      		cout << c[0].as<int>() << " " << c[1].as<int>() << " " << c[2].as<int>() << " " << c[3].as<string>() << " " << c[4].as<string>() << " " << c[5].as<int>() << " " << c[6].as<int>() << " " << c[7].as<int>() << " " << c[8].as<int>() << " " << c[9].as<float>() << " " << c[10].as<float>() << endl;
	    }
	N->commit();

}


void query2(connection *C, string team_color)
{
	ostringstream ss;

	ss << "SELECT TEAM.NAME FROM COLOR, TEAM WHERE COLOR.NAME = '" << team_color << "' AND COLOR.COLOR_ID = TEAM.COLOR_ID;";
	/* Create a non-transactional object. */
	nontransaction *N;
	N = new nontransaction(*C);
   
      
    	/* Execute SQL query */
    	result *R;
	R = new result( N->exec( ss.str() ));
	cout << "Name" << endl;
	for (result::const_iterator c = R->begin(); c != R->end(); ++c) {
      		cout << c[0].as<string>() << " " << endl;
	    }
	N->commit();
	
}


void query3(connection *C, string team_name)
{
	ostringstream ss;

	ss << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME " \
	"FROM PLAYER, TEAM " \
	"WHERE TEAM.NAME = '" << team_name << "' AND PLAYER.TEAM_ID = TEAM.TEAM_ID " \
	"ORDER BY PPG DESC;";

	/* Create a non-transactional object. */
	nontransaction *N;
	N = new nontransaction(*C);
   
      
    	/* Execute SQL query */
    	result *R;
	R = new result( N->exec( ss.str() ));
	cout << "FIRST_NAME LAST_NAME" << endl;
	for (result::const_iterator c = R->begin(); c != R->end(); ++c) {
      		cout << c[0].as<string>() << " " << c[1].as<string>() << endl;
	    }
	N->commit();
	
}


void query4(connection *C, string team_state, string team_color)
{
	ostringstream ss;

	ss << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, PLAYER.UNIFORM_NUM " \
	"FROM PLAYER, TEAM, STATE, COLOR " \
	"WHERE STATE.NAME = '" << team_state << "' AND COLOR.NAME = '" << team_color << "' "\
	"AND TEAM.STATE_ID = STATE.STATE_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID AND PLAYER.TEAM_ID = TEAM.TEAM_ID;";
	
	/* Create a non-transactional object. */
	nontransaction *N;
	N = new nontransaction(*C);
   
      
    	/* Execute SQL query */
    	result *R;
	R = new result( N->exec( ss.str() ));

	cout << "FIRST_NAME LAST_NAME UNIFORM_NUM" << endl;

	for (result::const_iterator c = R->begin(); c != R->end(); ++c) {
      		cout << c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<string>() << endl;
	    }
	N->commit();
}


void query5(connection *C, int num_wins)
{
	ostringstream ss;

	ss << "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS FROM PLAYER, TEAM WHERE " \
	" TEAM.WINS > " << num_wins << " AND TEAM.TEAM_ID = PLAYER.TEAM_ID;";

	/* Create a non-transactional object. */
	nontransaction *N;
	N = new nontransaction(*C);
   
      
    	/* Execute SQL query */
    	result *R;
	R = new result( N->exec( ss.str() ));

	cout << "FIRST_NAME LAST_NAME TEAM_NAME WINS" << endl;

	for (result::const_iterator c = R->begin(); c != R->end(); ++c) {
      		cout << c[0].as<string>() << " " << c[1].as<string>() << " " << c[2].as<string>() << " " << c[3].as<int>() << endl;
	    }
	N->commit();
}
