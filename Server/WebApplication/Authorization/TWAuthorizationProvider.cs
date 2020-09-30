using System.Threading.Tasks;
using Microsoft.AspNetCore.Authorization;
using Microsoft.Extensions.Options;
using WebApplication.Authorization.Policies;

namespace WebApplication.Authorization
{
    public class TWAuthorizationProvider : IAuthorizationPolicyProvider
    {
        private DefaultAuthorizationPolicyProvider PolicyProvider { get; }

        public TWAuthorizationProvider(IOptions<AuthorizationOptions> options)
        {
            PolicyProvider = new DefaultAuthorizationPolicyProvider(options);
        }

        public Task<AuthorizationPolicy> GetDefaultPolicyAsync() =>
            Task.FromResult(new AuthorizationPolicyBuilder().RequireAuthenticatedUser().Build());

        public Task<AuthorizationPolicy> GetFallbackPolicyAsync() =>
            Task.FromResult(new AuthorizationPolicyBuilder().Build());

        public Task<AuthorizationPolicy> GetPolicyAsync(string policyDescription)
        {
            //var (policyName, policyParam) = policyDescription.Split(":");
            var policySplitString = policyDescription.Split(":");
            if (policySplitString.Length != 2)
            {
                return GetDefaultPolicyAsync();
            }
            var policyName = policySplitString[0];
            var policyParam = policySplitString[1];

            if (policyParam == null)
            {
                return PolicyProvider.GetPolicyAsync(policyName);
            }

            var policy = AuthorizationPoliciesFactory.TryCreateDynamicPolicy(policyName, policyParam);
            if (policy == null)
            {
                return GetDefaultPolicyAsync();
            }

            return Task.FromResult(policy);
        }
    }
}
