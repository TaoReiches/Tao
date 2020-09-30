using Microsoft.AspNetCore.Authorization;
using TW.Api.Models.Common;

namespace WebApplication.Authorization.Requirements
{
    public class MinimalAccessRightsRequirement : IAuthorizationRequirement
    {
        public EUserRights MinimalRightsLevel { get; set; }

        public MinimalAccessRightsRequirement(EUserRights userRights)
        {
            MinimalRightsLevel = userRights;
        }
    }
}
