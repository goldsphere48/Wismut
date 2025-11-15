namespace WismutCLI;

[AttributeUsage(AttributeTargets.Method)]
public class WismutCommandAttribute(string name) : Attribute
{
	public string Name { get; } = name;
}
