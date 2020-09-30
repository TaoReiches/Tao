using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using FluentAssertions;
using TW.Api.Models.Signin;
using TW.Integration.Test.Helpers;

namespace TW.Integration.Test.IntegrationTests
{
    public class SigninControllerTest : IIntegrationTestRunner
    {
        public Task RunTests()
        {
            return Task.CompletedTask;
        }

        public static async Task ShouldReturnOKWhenSigninAsync()
        {
            var requestContent = new SigninRequestModel
            {
                UserIdentifier = DefaultData.AdminName,
                Password = DefaultData.AdminPassword
            };

            var response = await HttpHelper.SendRequest(HttpMethod.Put, "/Signin", requestContent, false);

            response.StatusCode.Should().Be(HttpStatusCode.OK);

            var tokenResult = HttpHelper.DeserializeResponse<SigninResultModel>(response);
            DefaultData.Token = tokenResult.Token;
        }
    }
}
