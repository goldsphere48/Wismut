#pragma once

#include <string>
#include <map>

class CLI
{
public:
	static void Initialize();
	static void ExecuteCommand(const std::string& command, const std::map<std::string, std::string>& arguments);

private:
	CLI() = default;
};
