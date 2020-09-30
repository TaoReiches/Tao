using Microsoft.AspNetCore.Authorization;
using TW.Api.Models.Common;

namespace WebApplication.Authorization.Requirements
{
    public class SpecificAccessRightsRequirement : IAuthorizationRequirement
    {
        public EUserRights AllowedAccessRightsLevel { get; set; }

        public SpecificAccessRightsRequirement(EUserRights userRights)
        {
            AllowedAccessRightsLevel = userRights;
        }
    }
}
