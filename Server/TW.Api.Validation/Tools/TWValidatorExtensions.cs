using FluentValidation;
using FluentValidation.Internal;
using TW.Api.Validation.Tools.Patterns;

namespace TW.Api.Validation.Tools
{
    public static class TWValidatorExtensions
    {
        private const string DefaultFieldName = "UnknownFieldName";

        public static IRuleBuilderOptions<T, string> AlphaNumericalField<T>(this IRuleBuilderInitial<T, string> ruleBuilder)
        {
            return ValidatorSetter<T, AlphaNumericalValidationPattern>(ruleBuilder);
        }

        public static IRuleBuilderOptions<T, string> CharactersOnly<T>(this IRuleBuilderInitial<T, string> ruleBuilder)
        {
            return ValidatorSetter<T, CharactersOnlyValidationPattern>(ruleBuilder);
        }

        private static IRuleBuilderOptions<T, string> ValidatorSetter<T, K>(
            IRuleBuilderInitial<T,string> ruleBuilder,
            string fieldName = DefaultFieldName)
            where K : IValidationPattern, new()
        {
            if (ruleBuilder is RuleBuilder<T, string> rb)
            {
                fieldName = rb.Rule.PropertyName;
            }

            return ruleBuilder.SetValidator(new CustomRegexFieldValidator(new K(), fieldName));
        }
    }
}
