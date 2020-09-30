using System.Threading.Tasks;
using TW.Api.Models;
using TW.Api.Models.Signin;

namespace TW.Services.Signin
{
    public interface ISigninService
    {
        Task<IResponse> GetSigninTokenAsync(SigninRequestModel signinRequest);

        Task<string> RefreshTokenAsync(string token, string refreshToken);
    }
}
