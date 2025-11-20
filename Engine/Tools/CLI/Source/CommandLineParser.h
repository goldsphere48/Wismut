#pragma once

#include <string>
#include <map>

struct CommandParameters
{
	std::string Сommand;
	std::map<std::string, std::string> Arguments;
};

class CommandLineParser
{
public:
	static CommandParameters Parse(int argc, char* argv[]);
};
