using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Swashbuckle.AspNetCore.Annotations;
using TW.Api.Models.Signin;
using TW.Services.Signin;

namespace WebApplication.Controllers
{
    public class SigninController : BasicController
    {
        private readonly ISigninService _signinService;

        public SigninController(ISigninService signinService)
        {
            _signinService = signinService;
        }

        [HttpPut]
        [AllowAnonymous]
        [SwaggerResponse(StatusCodes.Status200OK, Type=typeof(SigninResultModel))]
        public async Task<IActionResult> SigninAsync([FromBody] SigninRequestModel signinRequest)
        {
            var token = await _signinService.GetSigninTokenAsync(signinRequest);

            return ResultToJson(token);
        }
    }
}