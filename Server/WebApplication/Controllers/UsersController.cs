using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using Microsoft.AspNetCore.Mvc;
using Swashbuckle.AspNetCore.Annotations;
using TW.Api.Models.Common;
using TW.Api.Models.Users;
using TW.Services.Users;
using WebApplication.Authorization.Attributes;
using WebApplication.Validate;

namespace WebApplication.Controllers
{
    public class UsersController : BasicController
    {
        private readonly IUserManagementService _userManagementService;

        public UsersController(IUserManagementService userManagementService)
        {
            _userManagementService = userManagementService;
        }

        [HttpPut]
        public async Task<IActionResult> CreateNewUserAsync([FromBody] CreateUserRequestModel request)
        {
            var result = await _userManagementService.CreateUserAsync(request);

            return ResultToJson(result);
        }

        [HttpGet]
        [Route("{userIdentifier}")]
        [AuthorizeMinimalAccessRights(EUserRights.User)]
        [SwaggerResponse(StatusCodes.Status200OK, Type = typeof(UserInfoResultModel))]
        public async Task<IActionResult> GetUserInfoAsync([FromRoute] string userIdentifier)
        {
            var result = await _userManagementService.GetUserInfoAsync(userIdentifier);

            return ResultToJson(result);
        }
    }
}
