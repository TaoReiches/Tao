using System.ComponentModel.DataAnnotations.Schema;
using TW.DAL.Models.Users;

namespace TW.DAL.Models.Tokens
{
    [Table(nameof(TokenModel))]
    public class TokenModel : MySqlEntity
    {
        [ForeignKey(nameof(UserInfo))]
        public long RequesterId { get; set; }

        public string RefreshToken { get; set; }

        public virtual UserInfo UserInfo { get; set; }
    }
}
