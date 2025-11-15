namespace WismutCLI;

internal class Program
{
	static void Main(string[] args)
	{
		CLI.Initialize();
		var options = CommandLineParser.Parse(args);
		CLI.ExecuteCommand(options);
	}
}
