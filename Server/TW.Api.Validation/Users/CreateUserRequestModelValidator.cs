using FluentValidation;
using TW.Api.Models.Users;
using TW.Api.Validation.Tools;

namespace TW.Api.Validation.Users
{
    public class CreateUserRequestModelValidator : AbstractValidator<CreateUserRequestModel>
    {
        public CreateUserRequestModelValidator()
        {
            RuleFor(u => u.UserName).AlphaNumericalField().NotEmpty().MaximumLength(12).WithMessage(
                "User name is must be no longer than 12 symbols");
            RuleFor(u => u.Password).NotEmpty().WithMessage("Password can not empty");
            RuleFor(u => u.EMail).EmailAddress().NotEmpty().WithMessage("Please enter the right email");
        }
    }
}
