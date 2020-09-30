using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc.Testing;
using TW.Integration.Test.Custom;
using TW.Integration.Test.Helpers;
using TW.Integration.Test.IntegrationTests;
using Xunit;

namespace TW.Integration.Test
{
    public class IntegrationTest : IClassFixture<CustomWebApplicationFactory<CustomStartUp>>
    {
        public IntegrationTest(CustomWebApplicationFactory<CustomStartUp> factory)
        {
            DefaultData.Client = factory.CreateClient(new WebApplicationFactoryClientOptions
            {
                AllowAutoRedirect = false
            });
        }

        [Fact]
        [Trait("IntegrationTests", "")]
        public async Task IntegrationTestEntries()
        {
            // first must login
            await SigninControllerTest.ShouldReturnOKWhenSigninAsync();

            // then get user indentifier
            await UsersControllerTest.ShouldReturnOKWhenGetUserInfoAsync();

            // the other tests
            await new UsersControllerTest().RunTests();
        }
    }
}
