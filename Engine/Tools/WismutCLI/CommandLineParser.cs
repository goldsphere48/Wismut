namespace WismutCLI
{
	public class CommandParameters
	{
		public string Command = "";
		public Dictionary<string, string> Arguments = new();
	}

	public static class CommandLineParser
	{
		public static CommandParameters Parse(string[] args)
		{
			var options = new CommandParameters();
			foreach (var arg in args)
			{
				if (!arg.StartsWith("--"))
					options.Command = arg;
				else
				{
					string[] strings = arg.Split("=");
					string key = strings[0].Substring(2, strings[0].Length - 2);
					string value = "";
					if (strings.Length > 1)
						value = strings[1];
					options.Arguments.Add(key, value);
				}
			}

			return options;
		}
	}
}
