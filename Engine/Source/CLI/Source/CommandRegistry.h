#pragma once

#include <string>
#include <map>
#include <functional>

struct CommandParameter
{
	std::string Name;
	bool Required;
};

struct CommandMetadata
{
	std::string Name;
	std::function<void(const std::map<std::string, std::string>&)> Handler;
	std::map<std::string, CommandParameter> Parameters;
};

class CommandRegistry
{
public:
	static CommandRegistry& Instance();

	void RegisterCommand(const std::string& name, const std::function<void(const std::map<std::string, std::string>&)>& handler);
	void RegisterCommandParameter(const std::string& commandName, const std::string& paramName, bool required);

	const CommandMetadata* GetCommand(const std::string& name) const;
	const std::map<std::string, CommandMetadata>& GetAllCommands() const;

private:
	CommandRegistry() = default;
	std::map<std::string, CommandMetadata> commands;
};

#define REGISTER_COMMAND(name, handler) \
	static struct CommandRegistrar_##name { \
		CommandRegistrar_##name() { \
			CommandRegistry::Instance().RegisterCommand(#name, handler); \
		} \
	} registrar_##name;
