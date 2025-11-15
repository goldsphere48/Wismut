using System.Reflection;

namespace WismutCLI;

public static class CLI
{
	private static readonly Dictionary<string, MethodInfo> commands = new();

	public static void Initialize()
	{
		foreach (var methodInfo in typeof(Commands).GetMethods(BindingFlags.Static | BindingFlags.Public))
		{
			var attribute = methodInfo.GetCustomAttributes(typeof(WismutCommandAttribute), false).FirstOrDefault();
			if (attribute is WismutCommandAttribute wismutActionAttribute)
			{
				commands.Add(wismutActionAttribute.Name, methodInfo);
			}
		}
	}

	public static void ExecuteCommand(CommandParameters options)
	{
		if (string.IsNullOrEmpty(options.Command))
		{
			Console.WriteLine("Unspecified command");
			return;
		}

		if (!commands.TryGetValue(options.Command, out var method))
		{
			Console.WriteLine($"Unknown command {options.Command}");
			return;
		}

		var @params = new List<object>(options.Arguments.Count);
		foreach (var parameter in method.GetParameters())
		{
			var attribute = parameter.GetCustomAttributes(typeof(WismutCommandParamAttribute)).FirstOrDefault();
			if (attribute is WismutCommandParamAttribute wismutActionParamAttribute)
			{
				if (!options.Arguments.TryGetValue(wismutActionParamAttribute.Name, out var value) && wismutActionParamAttribute.Required)
				{
					Console.WriteLine($"Parameter {wismutActionParamAttribute.Name} for command {options.Command} required");
					return;
				}

				@params.Add(value ?? "");
			}
			else
				Console.WriteLine("Each argument of action should be marked as WismutCommandParam");
		}

		method.Invoke(null, @params.ToArray());
	}
}
