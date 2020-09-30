using System;
using Microsoft.AspNetCore.Authorization;
using TW.Api.Models.Common;
using WebApplication.Authorization.Policies;

namespace WebApplication.Authorization.Attributes
{
    [AttributeUsage(AttributeTargets.All, AllowMultiple = false)]
    internal sealed class AuthorizeMinimalAccessRightsAttribute : AuthorizeAttribute
    {
        private const string PolicyName = nameof(EAuthorizationPolicies.MinimalAccessRight);

        public AuthorizeMinimalAccessRightsAttribute(EUserRights minimalLevel) => Policy =
            $"{PolicyName}:{minimalLevel}";
    }
}
