using System;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using TW.Api.Models.Common;
using WebApplication.Authorization.Requirements;

namespace WebApplication.Authorization.Handlers
{
    public class SpecificAccessRightsHandler : AuthorizationHandler<SpecificAccessRightsRequirement>
    {
        protected override Task HandleRequirementAsync(AuthorizationHandlerContext context, SpecificAccessRightsRequirement requirement)
        {
            if (!context.User.HasClaim(u => u.Type == ClaimTypes.Role))
            {
                return Task.CompletedTask;
            }

            var userRightsString = context.User.FindFirst(u => u.Type == ClaimTypes.Role).Value;

            if (Enum.TryParse(userRightsString, out EUserRights userRights))
            {
                return Task.CompletedTask;
            }

            if (userRights != requirement.AllowedAccessRightsLevel && 
                userRights != EUserRights.Administrator)
            {
                return Task.CompletedTask;
            }

            context.Succeed(requirement);
            return Task.CompletedTask;
        }
    }
}
