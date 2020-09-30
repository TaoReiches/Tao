namespace TW.Api.Validation.Tools.Patterns
{
    internal class CharactersOnlyValidationPattern : IValidationPattern
    {
        public string RegexPattern => @"^[A-Za-zÀ-ȕ][A-Za-zÀ-ȕ\-_. ]*$";

        public string MessageOnPatternFail => "Only letters and '-', '_', ' ', '.' symbols are allowed";
    }
}
