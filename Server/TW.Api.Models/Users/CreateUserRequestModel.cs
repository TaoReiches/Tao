using Newtonsoft.Json;
using TW.Api.Models.Common;

namespace TW.Api.Models.Users
{
    [JsonObject]
    [LauncherExport]
    public class CreateUserRequestModel
    {
        [JsonRequired]
        public string UserName { get; set; }

        [JsonRequired]
        public string Password { get; set; }

        [JsonRequired]
        public string EMail { get; set; }
    }
}
