namespace TW.Api.Validation.Tools.Patterns
{
    class AlphaNumericalValidationPattern : IValidationPattern
    {
        public string RegexPattern => @"^[A-Za-z0-9\-_.]+$";

        public string MessageOnPatternFail => "Only letters, numbers and '-', '_', '.' symbols are allowed";
    }
}
