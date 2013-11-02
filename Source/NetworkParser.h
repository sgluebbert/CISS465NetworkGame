#ifndef NETWORKPARSER_H
#define NETWORKPARSER_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>

class NetworkParser
{

public:

	NetworkParser()
	{
		std::string line;
        std::ifstream config_file("../Source/conf/network.conf");
        if (config_file.is_open())
        {
        	while (std::getline(config_file, line))
        	{
        		if (line == "\n" || line == "" || line[0] == '#') 
        		{
        			continue;
        		}

        		line = RemoveWhiteSpace(line);
        		
    			std::string key;
    			std::string value;

    			int i = 0;
    			while (line[i] != '=')
    			{
    				key += line[i];
    				i++;
    			}

    			i++; // skip '=' character

    			while (i < line.size())
    			{
    				value += line[i];
    				i++;
    			}

    			map[key] = value;
        	}

        }

        config_file.close();
	}

	std::string RemoveWhiteSpace(std::string text)
	{
		std::string ret;
		for (int i = 0; i < text.size(); i++)
		{
			if (text[i] != ' ' && text[i] != '\t')
				ret += text[i];
		}

		return ret;
	}

	std::string GetNetworkType()
	{
		return map["NETWORK"];
	}

	const char * GetServerHost()
	{
		if (map["SERVER_HOST"] == "NULL")
			return NULL;

		return map["SERVER_HOST"].c_str();
	}

	const char * GetClientHost()
	{
		if (map["CLIENT_HOST"] == "NULL")
			return NULL;

		return map["CLIENT_HOST"].c_str();
	}

	Uint16 GetServerPort()
	{
		return atoi(map["SERVER_PORT"].c_str());
	}

	Uint16 GetClientPort()
	{
		return atoi(map["CLIENT_PORT"].c_str());
	}

private:
	std::map<std::string, std::string> map;	
};

#endif