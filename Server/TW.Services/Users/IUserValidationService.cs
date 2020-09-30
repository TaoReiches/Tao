using TW.DAL.Models.Users;

namespace TW.Services.Users
{
    public interface IUserValidationService
    {
        bool IsPasswordValid(UserInfo userInfo, string password);
    }
}
