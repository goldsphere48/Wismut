#include "CommandRegistry.h"

CommandRegistry& CommandRegistry::Instance()
{
	static CommandRegistry instance;
	return instance;
}

void CommandRegistry::RegisterCommand(
	const std::string& name,
	const std::function<void(const std::map<std::string, std::string>&)>& handler
)
{
	CommandMetadata metadata;
	metadata.Name = name;
	metadata.Handler = handler;
	commands[name] = metadata;
}

void CommandRegistry::RegisterCommandParameter(
	const std::string& commandName,
	const std::string& paramName,
	bool required
)
{
	auto it = commands.find(commandName);
	if (it != commands.end())
	{
		CommandParameter param;
		param.Name = paramName;
		param.Required = required;
		it->second.Parameters[paramName] = param;
	}
}

const CommandMetadata* CommandRegistry::GetCommand(const std::string& name) const
{
	auto it = commands.find(name);
	if (it != commands.end())
		return &it->second;
	return nullptr;
}

const std::map<std::string, CommandMetadata>& CommandRegistry::GetAllCommands() const
{
	return commands;
}
