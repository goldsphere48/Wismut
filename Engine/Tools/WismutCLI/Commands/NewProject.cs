namespace WismutCLI;

public static partial class Commands
{
	[WismutCommand("new-project")]
	public static void NewProject(
		[WismutCommandParam("wismut-path", required: true)] string wismutRoot,
		[WismutCommandParam("template")] string template,
		[WismutCommandParam("location")] string outputPath,
		[WismutCommandParam("name")] string name
	)
	{
		if (string.IsNullOrEmpty(template))
			template = "Default";

		if (string.IsNullOrEmpty(outputPath))
			template = "./";

		var templatePath = Path.Combine(wismutRoot, "Engine", "Templates", template);

		Console.WriteLine($"Selected template: {template}\n" +
		                  $"Creating project... ");

		string relativePathToWismut = Path.TrimEndingDirectorySeparator(Path.GetRelativePath(outputPath, wismutRoot));
		CopyDirectoryContentsAndReplaceArgs(templatePath, outputPath, name, relativePathToWismut);

		Console.WriteLine("Done.");
	}

	private static bool CopyDirectoryContentsAndReplaceArgs(string sourceDir, string destinationDir, string projectName, string pathToWismut)
	{
		var dir = new DirectoryInfo(sourceDir);

		if (!dir.Exists)
		{
			Console.WriteLine($"Can't find source path: {sourceDir}");
			return false;
		}

		Directory.CreateDirectory(destinationDir);

		var files = dir.GetFiles();
		foreach (FileInfo file in files)
		{
			string tempPath = Path.Combine(destinationDir, file.Name);
			string content = File.ReadAllText(file.FullName);
			content = content.Replace("{name}", projectName);
			content = content.Replace("{path_to_wismut}", pathToWismut);
			File.WriteAllText(tempPath, content);
		}

		var subDirs = dir.GetDirectories();
		foreach (DirectoryInfo subDir in subDirs)
		{
			string tempPath = Path.Combine(destinationDir, subDir.Name);
			CopyDirectoryContentsAndReplaceArgs(subDir.FullName, tempPath, projectName, pathToWismut);
		}

		return true;
	}
}
