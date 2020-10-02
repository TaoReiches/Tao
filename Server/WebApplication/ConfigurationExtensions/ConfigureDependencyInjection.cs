using Microsoft.AspNetCore.Authorization;
using Microsoft.Extensions.DependencyInjection;
using TW.DAL.Storage.Redis;
using TW.DAL.Storage.Repositories;
using TW.Services.Signin;
using TW.Services.Users;
using WebApplication.Authorization;
using WebApplication.Authorization.Handlers;

namespace WebApplication.ConfigurationExtensions
{
    public static class ConfigureDependencyInjection
    {
        public static void AddTWDependenciesInjection(this IServiceCollection services)
        {
            services.AddScoped<ISigninService, SigninService>();
            services.AddScoped<IUserValidationService, UserValidationService>();
            services.AddScoped<IUserManagementService, UserManagementService>();

            // Authorization
            services.AddSingleton<IAuthorizationHandler, MinimalAccessRightsHandler>();
            services.AddSingleton<IAuthorizationHandler, SpecificAccessRightsHandler>();
            services.AddSingleton<IAuthorizationPolicyProvider, TWAuthorizationProvider>();

            services.AddRepositories();
        }

        public static void AddRepositories(this IServiceCollection services)
        {
            services.AddSingleton<IRedisClient, RedisClient>();
            services.AddScoped<ITokenRepository, TokenRepository>();
            services.AddScoped<IUserInfoRepository, UserInfoRepository>();
            services.AddScoped<IRedisLuaTest, RedisLuaTest>();
        }
    }
}
