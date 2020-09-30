using System.Net;
using System.Net.Http;
using System.Threading.Tasks;
using FluentAssertions;
using TW.Api.Models.Users;
using TW.Integration.Test.Helpers;

namespace TW.Integration.Test.IntegrationTests
{
    public class UsersControllerTest : IIntegrationTestRunner
    {
        public async Task RunTests()
        {
            await ShouldReturnOKWhenCreateNewUserAsync();
        }

        public static async Task ShouldReturnOKWhenGetUserInfoAsync()
        {
            var response = await HttpHelper.SendRequest(HttpMethod.Get, $"/Users/{DefaultData.AdminName}");

            response.StatusCode.Should().Be(HttpStatusCode.OK);

            var result = HttpHelper.DeserializeResponse<UserInfoResultModel>(response);
            DefaultData.UserId = result.UserId;
        }

        private async Task ShouldReturnOKWhenCreateNewUserAsync()
        {
            var requestContent = new CreateUserRequestModel
            {
                UserName = "NewUser",
                Password = "NewPassword",
                EMail = "t@w.c"
            };

            var response = await HttpHelper.SendRequest(HttpMethod.Put, "/Users/", requestContent);

            response.StatusCode.Should().Be(HttpStatusCode.OK);
        }
    }
}
