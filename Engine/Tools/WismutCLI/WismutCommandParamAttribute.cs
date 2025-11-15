namespace WismutCLI;

[AttributeUsage(AttributeTargets.Parameter, AllowMultiple = true)]
public class WismutCommandParamAttribute(string name, bool required = false) : Attribute
{
	public string Name { get; } = name;
	public bool Required { get; } = required;
}
