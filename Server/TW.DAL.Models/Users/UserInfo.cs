using System.ComponentModel.DataAnnotations.Schema;
using TW.Api.Models.Common;

namespace TW.DAL.Models.Users
{
    [Table(nameof(UserInfo))]
    public class UserInfo : MySqlEntity
    {
        public string UserName { get; set; }

        public string EMail { get; set; }

        public string Password { get; set; }

        public EUserRights Role { get; set; }

        public string Salt { get; set; }
    }
}
