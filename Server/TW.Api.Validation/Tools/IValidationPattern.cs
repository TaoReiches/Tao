namespace TW.Api.Validation.Tools
{
    internal interface IValidationPattern
    {
        string RegexPattern { get; }

        string MessageOnPatternFail { get; }
    }
}
