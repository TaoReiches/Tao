using System.Security.Claims;
using NLog;

namespace TW.Common.Tools
{
    public static class UserUtils
    {
        private static ILogger Logger { get; } = LogManager.GetCurrentClassLogger();

        public static long GetUserId(ClaimsPrincipal claimsPrincipal)
        {
            if (long.TryParse(GetUserClaimValue(claimsPrincipal, "userId"), out long userId))
            {
                return userId;
            }

            Logger.Error("Could not parse userId from user claims");
            return -1;
        }

        public static long GetCompanyId(ClaimsPrincipal claimsPrincipal)
        {
            return long.Parse(GetUserClaimValue(claimsPrincipal, "companyId"));
        }

        private static string GetUserClaimValue(ClaimsPrincipal claimsPrincipal, string claimName)
        {
            var claimIdentity = claimsPrincipal.Identity as ClaimsIdentity;
            Claim claim = claimIdentity?.FindFirst(claimName);
            return claim != null ? claim.Value : string.Empty;
        }
    }
}
