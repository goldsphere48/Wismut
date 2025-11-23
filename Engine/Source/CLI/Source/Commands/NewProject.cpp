#include "Commands/NewProject.h"
#include "CommandRegistry.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace Commands
{
	static std::string ToLower(std::string str) {
		std::ranges::transform(str, str.begin(), [](unsigned char c) { return std::tolower(c); });
		return str;
	}

	static void ReplaceAll(std::string& str, const std::string& from, const std::string& to)
	{
		if (from.empty()) return;
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos)
		{
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
	}

	static void CopyDirectoryContentsAndReplaceArgs(
		const fs::path& sourceDir,
		const fs::path& destinationDir,
		const std::map<std::string, std::string>& replacements
	)
	{
		if (!fs::exists(sourceDir))
		{
			std::cout << "Can't find source path: " << sourceDir << "\n";
			return;
		}

		fs::create_directories(destinationDir);

		for (const auto& entry : fs::directory_iterator(sourceDir))
		{
			const auto& sourcePath = entry.path();
			const auto destinationPath = destinationDir / sourcePath.filename();

			if (entry.is_regular_file())
			{
				std::string content;
				std::ifstream inFile(sourcePath, std::ios::binary);
				if (inFile.is_open())
				{
					inFile.seekg(0, std::ios::end);
					content.resize(inFile.tellg());
					inFile.seekg(0, std::ios::beg);
					inFile.read(&content[0], content.size());
					inFile.close();
				}

				for (const auto& [placeholder, value] : replacements)
				{
					ReplaceAll(content, placeholder, value);
				}

				std::ofstream outFile(destinationPath, std::ios::binary);
				if (outFile.is_open())
				{
					outFile << content;
					outFile.close();
				}
			}
			else if (entry.is_directory())
			{
				CopyDirectoryContentsAndReplaceArgs(sourcePath, destinationPath, replacements);
			}
		}
	}

	void NewProject(const std::map<std::string, std::string>& arguments)
	{
		auto it = arguments.find("wismut-path");
		if (it == arguments.end() || it->second.empty())
		{
			std::cout << "Parameter wismut-path is required\n";
			return;
		}
		std::string wismutRoot = it->second;

		std::string template_name = "Default";
		if ((it = arguments.find("template")) != arguments.end() && !it->second.empty())
			template_name = it->second;

		std::string outputPath = "./";
		if ((it = arguments.find("location")) != arguments.end() && !it->second.empty())
			outputPath = it->second;

		std::string projectName = "MyProject";
		if ((it = arguments.find("name")) != arguments.end() && !it->second.empty())
			projectName = it->second;

		std::string company = "DefaultCompany";
		if ((it = arguments.find("company")) != arguments.end() && !it->second.empty())
			company = it->second;

		fs::path templatePath = fs::path(wismutRoot) / "Engine" / "Templates" / template_name;
		fs::path relativePathToWismut = fs::relative(wismutRoot, outputPath);

		std::cout << "Selected template: " << template_name << "\n"
			<< "Creating project '" << projectName << "' at " << outputPath << "... \n";

		std::map<std::string, std::string> replacements;

		replacements["{name}"] = projectName;
		replacements["{path_to_wismut}"] = relativePathToWismut.string();

		replacements["{name_android_package}"] = ToLower(projectName);
		replacements["{company_android_package}"] = ToLower(company);

		CopyDirectoryContentsAndReplaceArgs(templatePath, outputPath, replacements);

		std::cout << "Done.\n";
	}
}
