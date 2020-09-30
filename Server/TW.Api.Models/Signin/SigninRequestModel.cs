using Newtonsoft.Json;
using TW.Api.Models.Common;

namespace TW.Api.Models.Signin
{
    [JsonObject]
    [LauncherExport]
    public class SigninRequestModel
    {
        [JsonRequired]
        public string UserIdentifier { get; set; }

        [JsonRequired]
        public string Password { get; set; }
    }
}
