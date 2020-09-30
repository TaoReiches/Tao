${
    // Enable extension methods by adding using Typewriter.Extensions.*
    using Typewriter.Extensions.Types;

    Template(Settings settings)
    {
        settings.OutputFilenameFactory = file =>
        {
            var dirIndex = file.FullName.LastIndexOf(file.Name);
            var dir = file.FullName.Substring(0, dirIndex - 1);
            var folderIndex = dir.LastIndexOf("\\");
            var folder = dir.Substring(folderIndex);
            
            return  "..\\Generated\\" + folder + "\\" +  file.Name.Replace(".cs", ".ts");
        };
    }

    string GetDefaultValue(Property prop)
    {
        if (prop.Type.ClassName().Equals("string") && !prop.Type.IsEnumerable)
        {
            return "\"\"";
        }
        if (prop.Type.IsEnum)
        {
            return "-1";
        }
        if (prop.Type.IsNullable)
        {
            return "undefined";
        }
        return prop.Type.Default();
    }

    string GetNewClassPath(Type t)
    {
        return "TW.Api.Models/Generated/" + t.FullName.Substring("TW.Api.Models.".Length).Replace('.', '/');
    }

    string Imports(Class c)
    {
        IEnumerable<Type> types = c.Properties
            .Select(p => p.Type)
            .Where(t => !t.IsPrimitive)
            .Where(t => t.Name != c.Name)
            .Distinct();
            
        IEnumerable<Type> enums = c.Properties
            .Select(p => p.Type)
            .Where(t => t.IsEnum && t.FullName.Contains("TW.Api.Models"))
            .Where(t => t.Name != c.Name)
            .Distinct();

        string enumImports = string.Join(Environment.NewLine, enums.Select(t => $"import {{ {t.ClassName()} }} from '{GetNewClassPath(t)}';").Distinct());
        string classImports = string.Join(Environment.NewLine, types.Select(t => $"import {{ {t.ClassName()} }} from './{t.ClassName()}';").Distinct());
        return enumImports + ((enumImports.Length > 0 && classImports.Length > 0) ? "\n" : "") + classImports;
    }

    // append a ? next to any type that is nullable
    string TypeNullableFormatted(Property property) => property.Type.IsNullable ? $"?" : $"";

}
$Classes([LauncherExport])[
$Imports

export class $Name
{
    $Properties(prop => !prop.Attributes.Any(attr => attr.Name.Equals("JsonIgnore")))[
    public $Name$TypeNullableFormatted: $Type = $GetDefaultValue; ]
}]

$Enums([LauncherExport])[export enum $Name {
    $Values[
    $Name = $Value][,]
}]