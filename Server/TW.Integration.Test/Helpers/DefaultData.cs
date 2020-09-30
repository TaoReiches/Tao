using System.Net.Http;
using TW.Common.Tools;

namespace TW.Integration.Test.Helpers
{
    public class DefaultData
    {
        public const string BaseUri = "http://localhost:8080/api";
        public const string AdminName = "IntegrationTestAdminName";
        public const string AdminPassword = "IntegrationTestAdminPassword";
        public const string AdminEmail = "Admin@tw.com";
        public static readonly string AdminSalt = CryptUtils.GenerateSalt();
        public static string Token;
        public static HttpClient Client;
        public static long UserId = 1;
    }
}
