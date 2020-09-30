using Newtonsoft.Json;
using TW.Api.Models.Common;

namespace TW.Api.Models.Signin
{
    [JsonObject]
    [LauncherExport]
    public class SigninResultModel : BasicResponse
    {
        [JsonRequired]
        public string Token { get; set; }

        [JsonRequired]
        public string RefreshToken { get; set; }
    }
}
