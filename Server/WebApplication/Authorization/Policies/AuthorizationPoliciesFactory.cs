using System;
using Microsoft.AspNetCore.Authorization;
using TW.Api.Models.Common;
using WebApplication.Authorization.Requirements;

namespace WebApplication.Authorization.Policies
{
    internal static class AuthorizationPoliciesFactory
    {
        internal static AuthorizationPolicy TryCreateDynamicPolicy(string policyName, string policyParameter)
        {
            switch (policyName)
            {
                case nameof(EAuthorizationPolicies.MinimalAccessRight):
                    {
                        if (Enum.TryParse(policyParameter, out EUserRights userRights))
                        {
                            var policyBuilder = new AuthorizationPolicyBuilder();
                            policyBuilder.AddRequirements(new MinimalAccessRightsRequirement(userRights));
                            return policyBuilder.Build();
                        }
                        break;
                    }
                case nameof(EAuthorizationPolicies.OnlySpecificRight):
                    {
                        if (Enum.TryParse(policyParameter, out EUserRights userRights))
                        {
                            var policyBuilder = new AuthorizationPolicyBuilder();
                            policyBuilder.AddRequirements(new SpecificAccessRightsRequirement(userRights));
                            return policyBuilder.Build();
                        }
                        break;
                    }
            }

            return null;
        }
    }
}
