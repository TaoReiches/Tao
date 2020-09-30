using System.Text.RegularExpressions;
using FluentValidation.Validators;

namespace TW.Api.Validation.Tools
{
    internal class CustomRegexFieldValidator : PropertyValidator
    {
        private readonly IValidationPattern _validationPattern;

        public CustomRegexFieldValidator(IValidationPattern pattern, string fieldName) :
            base($"{fieldName} - {pattern.MessageOnPatternFail}")
        {
            _validationPattern = pattern;
        }

        protected override bool IsValid(PropertyValidatorContext context)
        {
            if (context.PropertyValue is string propertyToValidate &&
                !string.IsNullOrEmpty(propertyToValidate))
            {
                return Regex.IsMatch(propertyToValidate, _validationPattern.RegexPattern);
            }

            return false;
        }
    }
}
