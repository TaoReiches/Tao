using System;
using Microsoft.AspNetCore.Authorization;
using TW.Api.Models.Common;
using WebApplication.Authorization.Policies;

namespace WebApplication.Authorization.Attributes
{
    [AttributeUsage(AttributeTargets.All, AllowMultiple = false)]
    internal sealed class AuthorizeSpecificAccessRightsAttribute : AuthorizeAttribute
    {
        private const string PolicyName = nameof(EAuthorizationPolicies.OnlySpecificRight);

        public AuthorizeSpecificAccessRightsAttribute(EUserRights accessRights) => Policy =
            $"{PolicyName}:{accessRights}";
    }
}
