#include "CLI.h"
#include "CommandLineParser.h"

int main(int argc, char* argv[])
{
	CLI::Initialize();
	CommandParameters params = CommandLineParser::Parse(argc, argv);
	CLI::ExecuteCommand(params.Сommand, params.Arguments);
	return 0;
}
