using System;
using System.Security.Claims;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using TW.Api.Models.Common;
using WebApplication.Authorization.Requirements;

namespace WebApplication.Authorization.Handlers
{
    public class MinimalAccessRightsHandler : AuthorizationHandler<MinimalAccessRightsRequirement>
    {
        protected override Task HandleRequirementAsync(AuthorizationHandlerContext context, MinimalAccessRightsRequirement requirement)
        {
            if (!context.User.HasClaim(u => u.Type == ClaimTypes.Role))
            {
                return Task.CompletedTask;
            }

            var userRightsString = context.User.FindFirst(u => u.Type == ClaimTypes.Role).Value;
            if (!Enum.TryParse(userRightsString, out EUserRights userRights))
            {
                return Task.CompletedTask;
            }

            if (userRights < requirement.MinimalRightsLevel)
            {
                return Task.CompletedTask;
            }

            context.Succeed(requirement);
            return Task.CompletedTask;
        }
    }
}
