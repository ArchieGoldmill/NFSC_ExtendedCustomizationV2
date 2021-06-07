using System.IO;


namespace Localizator
{
	class Program
	{
		// I'm just too lazy to do this manually
		static void Main(string[] args)
		{
			// Might remove this hardcode later)
			string path = @"E:\Games\Need for Speed - Carbon\_mods\ExtendedCustomization\Install\Binary\Localization";

			var files = new string[] {"Danish", "Dutch", "Finnish", "French", "German",
				"Italian", "Labels", "Largest", "Mexican", "Polish", "Russian", "Spanish", "Swedish"};
			var mainFile = "English";

			var text = File.ReadAllText(Path.Combine(path, mainFile + ".end"));
			foreach (var file in files)
			{
				var res = text.Replace(mainFile.ToUpper() + "_", file.ToUpper() + "_");
				File.WriteAllText(Path.Combine(path, file + ".end"), res);
			}
		}
	}
}
