/*
CS 657 PA1

Author: 
David Gabriel
2-21-19

This program is an extremely minimalist database management system, 
according to the programming assignment instructions. It lacks many 
key features, and is not recommended for use as a database manager. 

It is possible, with some intention, to input commands to this program
in such a way that may harm your system. It is recommended to not 
have super user permissions enabled while using this program. The program
will issue "rm" commands with operands determined from user input. Use
common sense. This program is safe when used with the provided testbench
given with the assignment.

There is absolutely NO warranty, not even for merchantability or suitability
for a particular purchase.

The author releases this software under GPL-3.0-only or GPL-3.0-or-later
license. 

*/
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <regex>
#include <string>
#include <dirent.h>
#include <errno.h>

using namespace std;
char scan_buffer;
std::string use_path;
char* use_path_complete;
std::string use_path_complete_str;

/*
mkdir- function to create a directory, which is equivalent in our relational database to
a new database.
Takes a string which is the directory name. Will create directory in "./" or return error 
*/

int mkdir(std::string dir_name)
{
        std::string dir_path_complete_temp = "./" + dir_name;
       	char* dir_path_complete = &dir_path_complete_temp[0u];
        int dir_err;

        DIR* dir = opendir(dir_path_complete);
        if (dir)
        {
            /* Directory exists. */
                closedir(dir);
        	printf("-- !Failed to create database ");
                std::cout << dir_name;
                printf(" because it already exists.\r\n");
        }
        else if (ENOENT == errno)
        {
                dir_err = mkdir(dir_path_complete, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        	printf("-- Database ");
                std::cout << dir_name;
                printf(" created.\r\n");
                /* Directory does not exist. */
        }
        else
        {
            /* opendir() failed for some other reason. */
        }

return 1;
}

/*
rmdir- remove directory. Takes in a directory name and searches within "./" 
to find directory. If directory exists, will delete. Else returns error to console.
*/

int rmdir(std::string dir_name)
{
        //format strings
        std::string dir_path_complete_temp = "./" + dir_name;
       	char* dir_path_complete = &dir_path_complete_temp[0u];
        int dir_err;

        DIR* dir = opendir(dir_path_complete);
        if (dir)
        {
            /* Directory exists. */
                closedir(dir);
                //remove direcory
               	std::string rm_cmd = "rm -r -f ./" + dir_name;
                //cout<< rm_cmd;
        	system(&rm_cmd[0U]);
                //console output
                printf("-- Database ");
                std::cout << dir_name;
                printf(" deleted.\r\n");

        }
        else if (ENOENT == errno)
        {
        	printf("--!Failed to delete ");
                std::cout << dir_name;
                printf(" because it does not exist.\r\n");
                /* Directory does not exist. */
        }
        else
        {
            /* opendir() failed for some other reason. */
        }
return 1;
}

/*
use- fuction to select path to a database. 
*/

int use(std::string use_name)
{
	use_path_complete_str = "./" + use_name;
	use_path_complete = &use_path_complete_str[0u];

	cout<< "-- Using database "<< use_path_complete_str << "." << std::endl;

	return 1;
}

/*
mktable- fuction to create a table within a database. The table is stored as a text file.
*/

int mktable(std::string table_name, std::string schema)
{
	//string format
	std::string table_path = use_path_complete_str + "/" + table_name;

	if (std::ifstream(table_path))
	{
		std::cout << "-- !Failed to create table "<< table_name << " because it already exists."  << std::endl;
		return false;
	}
	else
	{
	//create file
	std::cout << "-- Table " << table_name << " created."  << std::endl;
	std::ofstream myfile;
	myfile.open (table_path);
	myfile << schema;
	myfile.close();
	}
	return 1;
}


int rmtable(std::string table_name)
{
	//string format
	std::string table_path = use_path_complete_str + "/" + table_name;

	if (std::ifstream(table_path))
	{
		std::string rm_cmd = "rm -r -f ./" + table_path;
		std::cout << "-- Table "<< table_name << " deleted."  << std::endl;
        	system(&rm_cmd[0U]);

		return 1;
	}
	else
	{
	std::cout << "-- !Failed to delete " << table_name << " because it does not exist."  << std::endl;
	return 0;
	}

return 0;
}

int add_table(std::string table_name, std::string schema)
{

	//string format
	std::string table_path = use_path_complete_str + "/" + table_name;

	//write schema to file
	std::ofstream myfile;
	myfile.open (table_path);
	myfile << schema;
      	myfile.close();	

return 1;
/*
	myfile << "This is the first cell in the first column.\n";
	myfile << "a,b,c,\n";
	myfile << "c,s,v,\n";
	myfile << "1,2,3.456\n";
*/

}


int read_table(std::string table_name)
{
	//string format
	std::string table_path = use_path_complete_str + "/" + table_name;

	if (std::ifstream(table_path))
	{
	//if table exists


		std::ifstream myfile;
		myfile.open (table_path);
		std::string line;

//		std::cout << "-- Table "<< table_name << " deleted."  << std::endl;

		while (std::getline(myfile, line))
		{

			std::replace( line.begin(), line.end(), ',', '|'); 
			// replace all ',' to '|'

			std::cout << line << std::endl;
		}
	      	myfile.close();

//		std::cout << "-- Table "<< table_name << " deleted."  << std::endl;

		return 1;
	}
	else
	{

	std::cout << "-- !Failed to query " << table_name << " because it does not exist."  << std::endl;
	return 0;

	}

return 0;
}

//program entry
int main()
{
	bool exit_switch = 1;
	//main control loop
	while(exit_switch)
	{
		//interactive shell sdtio
		std::string line;
		std::cout << "dave-sql> ";
		getline (std::cin, line);
		std::string result;


                //regex detect command
		if(std::regex_match (line, std::regex("(CREATE DATABASE)(.*)" )))
		{
			std::string database_name = line.erase(0, 16);
			std::string::size_type semicolon = line.find(";");
                        database_name = database_name.erase(semicolon, 1);
		        mkdir(database_name);
		}

		if(std::regex_match (line, std::regex("(DROP DATABASE)(.*)" )))
		{
			std::string database_name = line.erase(0, 14);
			std::string::size_type semicolon = line.find(";");
                        database_name = database_name.erase(semicolon, 1);
			rmdir(database_name);
		}

                if(std::regex_match (line, std::regex("(USE)(.*)" )))
		{
			std::string use_name = line.erase(0, 4);
			std::string::size_type semicolon = line.find(";");
                        use_name = use_name.erase(semicolon, 1);
			use(use_name);
		}
		if(std::regex_match (line, std::regex("(CREATE TABLE)(.*)" )))
		{
			line = line.erase(0, 13);
			std::string::size_type semicolon = line.find(";");
                        std::string::size_type open_parens = line.find("(");
			//extract schema
			std::string schema = line.substr(open_parens+1, semicolon);
			semicolon = schema.find(";");
			schema = schema.erase((semicolon-1),2 );
			//extract table name
			std::string table_name = line.substr(0, open_parens-1);

			std::cout << table_name << std::endl;
			std::cout << schema  << std::endl;


			mktable(table_name, schema);
		}

		if(std::regex_match (line, std::regex("(DROP TABLE)(.*)" )))
		{
			std::string table_name = line.erase(0, 11);
			std::string::size_type semicolon = line.find(";");
                        table_name = table_name.erase(semicolon, 1);
			rmtable(table_name);
		}

		if(std::regex_match (line, std::regex("(ALTER TABLE)(.*)" )))
		{
			std::string table_name = line.erase(0, 12);
			std::string::size_type add = line.find("ADD");
			std::string::size_type semicolon = line.find(";");
			//extract schema
			std::string schema = line.substr(add+4,(semicolon-add));
			semicolon = schema.find(";");
			schema = schema.erase(semicolon,1);
			//extract table name
                        add = table_name.find("ADD");
			semicolon = table_name.find(";");
                        table_name = table_name.erase(add+1, semicolon-add+2);
			add_table(table_name, schema);
		}
		if(std::regex_match (line, std::regex("(SELECT)(.*)" )))
		{
			//std::cout << "selectfrom" << std::endl;
			line = line.erase(0, 14);
			std::string::size_type semicolon = line.find(";");
			std::string table_name = line.erase(semicolon,1);
			//extract table name;
			//std:cout << table_name << std::endl;
			read_table(table_name);
		}

		//exit switch
		if(line == "exit"){return 0;}
		line.clear();
	}
}
