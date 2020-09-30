using FluentValidation.TestHelper;
using NUnit.Framework;
using TW.Api.Validation.Users;

namespace TW.Api.Validation.Test.Users
{
    [TestFixture]
    [Category("UnitTests")]
    public class CreateUserRequestModelValidatorTest
    {
        private CreateUserRequestModelValidator _validations;

        [SetUp]
        public void SetUp()
        {
            _validations = new CreateUserRequestModelValidator();
        }

        [Test]
        public void ShouldHaveErrorIfPasswordIsEmpty()
        {
            _validations.ShouldHaveValidationErrorFor(u => u.Password, string.Empty);
            _validations.ShouldHaveValidationErrorFor(u => u.Password, value: null, ruleSet: null);
        }

        [Test]
        public void ShouldNotHavaErrorIfPasswordExists()
        {
            _validations.ShouldNotHaveValidationErrorFor(u => u.Password, "a");
        }

        [Test]
        public void ShouldHaveErrorIfUserNameIsInvalid()
        {
            _validations.ShouldHaveValidationErrorFor(u => u.UserName, new string('a', 13));
            _validations.ShouldHaveValidationErrorFor(u => u.UserName, "+asd");
            _validations.ShouldHaveValidationErrorFor(u => u.UserName, ")(**");
        }

        [Test]
        public void ShouldNotHaveErrorIfUserNameIsValid()
        {
            _validations.ShouldNotHaveValidationErrorFor(u => u.UserName, "As");
            _validations.ShouldNotHaveValidationErrorFor(u => u.UserName, new string('a', 12));
            _validations.ShouldNotHaveValidationErrorFor(u => u.UserName, "a._-");
        }
    }
}
