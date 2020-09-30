using TW.Common.Tools;
using TW.DAL.Models.Users;

namespace TW.Services.Users
{
    public class UserValidationService : IUserValidationService
    {
        public bool IsPasswordValid(UserInfo userInfo, string password)
        {
            var saltedAndHashedPassword = CryptUtils.GetHashedPassword(password, userInfo.Salt);

            return string.Equals(saltedAndHashedPassword, userInfo.Password);
        }
    }
}
