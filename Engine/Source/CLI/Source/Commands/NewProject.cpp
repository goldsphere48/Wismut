#include "Commands/NewProject.h"
#include "CommandRegistry.h"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace Commands
{
	static void CopyDirectoryContentsAndReplaceArgs(
		const fs::path& sourceDir,
		const fs::path& destinationDir,
		const std::string& projectName,
		const std::string& pathToWismut)
	{
		if (!fs::exists(sourceDir))
		{
			std::cout << "Can't find source path: " << sourceDir << "\n";
			return;
		}

		fs::create_directories(destinationDir);

		// Copy files
		for (const auto& entry : fs::directory_iterator(sourceDir))
		{
			if (entry.is_regular_file())
			{
				std::string content;
				std::ifstream file(entry.path());
				if (file.is_open())
				{
					content = std::string((std::istreambuf_iterator<char>(file)),
										std::istreambuf_iterator<char>());
					file.close();
				}

				// Replace placeholders
				size_t pos = 0;
				while ((pos = content.find("{name}", pos)) != std::string::npos)
				{
					content.replace(pos, 6, projectName);
					pos += projectName.length();
				}

				pos = 0;
				while ((pos = content.find("{path_to_wismut}", pos)) != std::string::npos)
				{
					content.replace(pos, 16, pathToWismut);
					pos += pathToWismut.length();
				}

				fs::path destFile = destinationDir / entry.path().filename();
				std::ofstream outFile(destFile);
				if (outFile.is_open())
				{
					outFile << content;
					outFile.close();
				}
			}
		}

		// Recursively copy subdirectories
		for (const auto& entry : fs::directory_iterator(sourceDir))
		{
			if (entry.is_directory())
			{
				fs::path destDir = destinationDir / entry.path().filename();
				CopyDirectoryContentsAndReplaceArgs(entry.path(), destDir, projectName, pathToWismut);
			}
		}
	}

	void NewProject(const std::map<std::string, std::string>& arguments)
	{
		// Get parameters
		auto it = arguments.find("wismut-path");
		if (it == arguments.end() || it->second.empty())
		{
			std::cout << "Parameter wismut-path is required\n";
			return;
		}
		std::string wismutRoot = it->second;

		std::string template_name = "Default";
		it = arguments.find("template");
		if (it != arguments.end() && !it->second.empty())
			template_name = it->second;

		std::string outputPath = "./";
		it = arguments.find("location");
		if (it != arguments.end() && !it->second.empty())
			outputPath = it->second;

		std::string projectName = "MyProject";
		it = arguments.find("name");
		if (it != arguments.end() && !it->second.empty())
			projectName = it->second;

		// Build template path
		fs::path templatePath = fs::path(wismutRoot) / "Engine" / "Templates" / template_name;

		std::cout << "Selected template: " << template_name << "\n"
				  << "Creating project... \n";

		// Calculate relative path to Wismut
		fs::path relativePathToWismut = fs::relative(wismutRoot, outputPath);
		std::string relativePathStr = relativePathToWismut.string();

		CopyDirectoryContentsAndReplaceArgs(templatePath, outputPath, projectName, relativePathStr);

		std::cout << "Done.\n";
	}
}
