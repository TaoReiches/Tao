using System.Threading.Tasks;
using TW.Api.Models;
using TW.Api.Models.Users;

namespace TW.Services.Users
{
    public interface IUserManagementService
    {
        Task<IResponse> CreateUserAsync(CreateUserRequestModel request);

        Task<IResponse> GetUserInfoAsync(string userIdentifier);
    }
}
