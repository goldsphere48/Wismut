#include "CLI.h"
#include "CommandRegistry.h"
#include "Commands/NewProject.h"
#include <iostream>

void CLI::Initialize()
{
	auto newProjectHandler = [](const std::map<std::string, std::string>& args) {
		Commands::NewProject(args);
	};

	CommandRegistry::Instance().RegisterCommand("new-project", newProjectHandler);
	CommandRegistry::Instance().RegisterCommandParameter("new-project", "wismut-path", true);
	CommandRegistry::Instance().RegisterCommandParameter("new-project", "template", false);
	CommandRegistry::Instance().RegisterCommandParameter("new-project", "location", false);
	CommandRegistry::Instance().RegisterCommandParameter("new-project", "name", false);
	CommandRegistry::Instance().RegisterCommandParameter("new-project", "company", false);
}

void CLI::ExecuteCommand(const std::string& command, const std::map<std::string, std::string>& arguments)
{
	if (command.empty())
	{
		std::cout << "Unspecified command\n";
		return;
	}

	const CommandMetadata* metadata = CommandRegistry::Instance().GetCommand(command);
	if (!metadata)
	{
		std::cout << "Unknown command: " << command << "\n";
		return;
	}

	for (const auto& [paramName, param] : metadata->Parameters)
	{
		if (param.Required && !arguments.contains(paramName))
		{
			std::cout << "Parameter " << paramName << " for command " << command << " is required\n";
			return;
		}
	}

	metadata->Handler(arguments);
}
