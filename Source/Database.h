#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string>
#include <string.h>

using namespace std;

const static char * ip = "127.0.0.1";
const static char * usr = "starclash";
const static char * pass = "starclash";
const static char * db =  "Starclash";

static int query_state;

class Database
{
	MYSQL mysql, *connection;
	MYSQL_RES *result;
	MYSQL_ROW row;

public:

	bool connect()
	{
		mysql_init(&mysql);
	 
		connection = mysql_real_connect(&mysql, ip, usr, pass, db, 0, NULL, 0);
	 
		if (connection==NULL)
		{
			cout<<mysql_error(&mysql)<<endl;
			return false;
		}
		else
		return true;
	}

	//////////////////////////////////////////////////////////////////////
	int check_player(char* username, char* password)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			//return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT 1 FROM Player where username = ('%s') and password = ('%s')", username, password);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			return get_player_id_with_username(username);
		}

		else
		{
			std::cout << 0 << std::endl;
			return 0;
			
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
		return true;
	}


	int get_player_id_with_username(char* username)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT player_id FROM Player where username = ('%s')", username);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
		return true;
	}



	//////////////////////////////////////////////////////////////////////
	bool insert_player(int player_id, char* username, char* password)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "INSERT INTO Player set player_id = ('%d'), username = ('%s'), password = ('%s')", player_id, username, password);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool insert_team_1(int team_1_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "INSERT INTO Team_1 set team_1_id = ('%d')", team_1_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool insert_team_2(int team_2_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "INSERT INTO Team_2 set team_2_id = ('%d')", team_2_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool insert_lobby(int lobby_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "INSERT INTO Lobby set lobby_id = ('%d')", lobby_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}
	///////////////////////////////////////////////////////////////////////



	bool delete_player(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "DELETE FROM Player WHERE player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool delete_team_1(int team_1_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "DELETE FROM Team_1 WHERE team_1_id = ('%d')", team_1_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool delete_team_2(int team_2_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "DELETE FROM Team_2 WHERE team_2_id = ('%d')", team_2_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool delete_lobby(int lobby_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "DELETE FROM Lobby WHERE lobby_id = ('%d')", lobby_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}



	////////////////////////////////////////////////////////////////////////

	bool update_player_id(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET player_id = ('%d') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_username(int player_id, char* username)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET username = ('%s') WHERE player_id = ('%d')", username, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_password(int player_id, char* password)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET password = ('%s') WHERE player_id = ('%d')", password, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_fname(int player_id, char* fname)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET fname = ('%s') WHERE player_id = ('%d')", fname, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_email(int player_id, char* email)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET email = ('%s') WHERE player_id = ('%d')", email, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_lname(int player_id, char* name)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET lname = ('%s') WHERE player_id = ('%d')", name, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_total_kills(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET total_kills = ('%d') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool add_total_kills(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET total_kills = total_kills + %d WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_shots_fired(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET shots_fired = ('%d') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool add_shots_fired(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET shots_fired = shots_fired + %d WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_shots_hit(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET shots_hit = ('%d') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool add_shots_hit(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET shots_hit = shots_hit + %d WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_total_deaths(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET total_deaths = ('%d') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool add_total_deaths(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET total_deaths = total_deaths + %d WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_kill_death_ratio(int player_id, double num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET kill_death_ratio = ('%f') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_accuracy(int player_id, double num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET accuracy = ('%f') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_wins(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET wins = ('%d') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool add_wins(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET wins = wins + %d WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_losses(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET losses = ('%d') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool add_losses(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET losses = losses + %d WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_experience_points(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET experience_points = ('%d') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool add_experience_points(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET experience_points = experience_points + %d WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_captures(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET captures = ('%d') WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool add_captures(int player_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET captures = captures + %d WHERE player_id = ('%d')", num, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_rank(int player_id, char* rank)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET rank = ('%s') WHERE player_id = ('%d')", rank, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool add_rank(int player_id, char* rank)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET rank = rank + %s WHERE player_id = ('%d')", rank, player_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_team_1_id(int team_1_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Team_1 SET team_1_id = ('%d') WHERE team_1_id = ('%d')", num, team_1_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}


	bool update_team_1_num_players(int team_1_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Team_1 SET num_players = ('%d') WHERE team_1_id = ('%d')", num, team_1_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_team_1_color(int team_1_id, char* color)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET color = ('%s') WHERE team_1_id = ('%d')", color, team_1_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_team_1_lobby_id(int team_1_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Team_1 SET lobby_id = ('%d') WHERE team_1_id = ('%d')", num, team_1_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}




	bool update_team_2_id(int team_2_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Team_2 SET team_2_id = ('%d') WHERE team_2_id = ('%d')", num, team_2_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}


	bool update_team_2_num_players(int team_2_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Team_2 SET num_players = ('%d') WHERE team_2_id = ('%d')", num, team_2_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_team_2_color(int team_2_id, char* color)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Player SET color = ('%s') WHERE team_2_id = ('%d')", color, team_2_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_team_2_lobby_id(int team_2_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Team_2 SET lobby_id = ('%d') WHERE team_2_id = ('%d')", num, team_2_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_lobby_id(int lobby_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Lobby SET lobby_id = ('%d') WHERE lobby_id = ('%d')", num, lobby_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_lobby_team_1_id(int team_1_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Lobby SET team_1_id = ('%d') WHERE lobby_id = ('%d')", num, team_1_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_lobby_team_2_id(int team_2_id, int num)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Lobby SET team_2_id = ('%d') WHERE lobby_id = ('%d')", num, team_2_id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}

	bool update_lobby_num_players(int num_players, int id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return false;
		}

		char statement[512];
		sprintf(statement, "UPDATE Lobby SET num_players = ('%d') WHERE lobby_id = ('%d')", num_players, id);

		mysql_query(connection, statement);
		mysql_query(connection, "COMMIT");
		return true;
	}


	///////////////////////////////////////////////////////////////////////




	///////////////////////////////////////////////////////////////////////
	int get_player_id(const char *username)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT player_id FROM Player where username = ('%s')", username);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	char* get_username(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			//return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT username FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			return row[0];
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	char* get_password(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			//return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT password FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			return row[0];
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	char* get_fname(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			//return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT fname FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			return row[0];
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	char* get_lname(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			//return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT lname FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			return row[0];
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	char* get_email(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			//return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT email FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			return row[0];
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}


	int get_total_kills(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT total_kills FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_shots_fired(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT shots_fired FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_shots_hit(int player_id)
	{
		if (query_state !=0) 
		{
			// cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT shots_hit FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_total_deaths(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT total_deaths FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_kill_death_ratio(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT kill_death_ratio FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_accuracy(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT accuracy FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_wins(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT wins FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_losses(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT losses FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_experience_points(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT experience_points FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_captures(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT captures FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	char* get_rank(int player_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			//return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT rank FROM Player where player_id = ('%d')", player_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			return row[0];
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}



	int get_team_1_id(int team_1_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT team_1_id FROM Team_1 where team_1_id = ('%d')", team_1_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_team_1_num_players(int team_1_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT team_1_num_players FROM Team_1 where team_1_id = ('%d')", team_1_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	char* get_team_1_color(int team_1_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			//return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT color FROM Team_1 where team_1_id = ('%d')", team_1_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			return row[0];
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_team_1_lobby_id(int team_1_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT team_1_lobby_id FROM Team_1 where team_1_id = ('%d')", team_1_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}





	int get_team_2_id(int team_2_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT team_2_id FROM Team_2 where team_2_id = ('%d')", team_2_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_team_2_num_players(int team_2_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT team_2_num_players FROM Team_2 where team_2_id = ('%d')", team_2_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	char* get_team_2_color(int team_2_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			//return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT color FROM Team_2 where team_2_id = ('%d')", team_2_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			return row[0];
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_team_2_lobby_id(int team_2_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT team_2_lobby_id FROM Team_2 where team_2_id = ('%d')", team_2_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}






	int get_lobby_id(int lobby_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT lobby_id FROM Lobby where lobby_id = ('%d')", lobby_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_lobby_team_1_id(int lobby_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT team_1_id FROM Lobby where lobby_id = ('%d')", lobby_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_lobby_team_2_id(int lobby_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT team_2_id FROM Lobby where lobby_id = ('%d')", lobby_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}

	int get_lobby_num_players(int lobby_id)
	{
		if (query_state !=0) 
		{
			cout << mysql_error(connection) << endl;
			return -1;
		}

		char statement[512];
		sprintf(statement, "SELECT num_players FROM Lobby where lobby_id = ('%d')", lobby_id);

		mysql_query(connection, statement);
		
		result = mysql_store_result(connection);

		if(row = mysql_fetch_row(result))
		{
			// std::cout << row[0] << std::endl;
			int k = atoi(row[0]);
			return k;
		}
		
		mysql_query(connection, "COMMIT");
		mysql_free_result(result);
	}
	///////////////////////////////////////////////////////////////////////




	void close()
	{
		mysql_close(&mysql);
	}


};

#endif

