namespace TW.Api.Models.Users
{
    public class UserInfoResultModel : BasicResponse
    {
        public long UserId { get; set; }

        public string UserName { get; set; }

        public string Email { get; set; }
    }
}
