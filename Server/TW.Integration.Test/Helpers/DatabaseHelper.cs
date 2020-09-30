using TW.Api.Models.Common;
using TW.Common.Tools;
using TW.DAL.Models.Users;
using TW.DAL.Storage;

namespace TW.Integration.Test.Helpers
{
    public static class DatabaseHelper
    {
        private static TWContext _db;

        public static void InitializeDbForTests(TWContext db)
        {
            _db = db;
            CreateDefaultUser();
        }

        private static void CreateDefaultUser()
        {
            _db.UserInfoData.Add(new UserInfo
            {
                UserName = DefaultData.AdminName,
                Salt = DefaultData.AdminSalt,
                Password = CryptUtils.GetHashedPassword(DefaultData.AdminPassword, DefaultData.AdminSalt),
                Role = EUserRights.Administrator,
                EMail = DefaultData.AdminEmail
            });
            _db.SaveChanges();
        }
    }
}
