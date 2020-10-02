using System.Threading.Tasks;
using StackExchange.Redis;
using TW.Api.Models.Signin;

namespace TW.DAL.Storage.Redis
{
    public interface IRedisLuaTest
    {
        Task<SigninRequestModel> OnUserSignin(SigninRequestModel request);
    }

    public class RedisLuaTest : IRedisLuaTest
    {
        protected IDatabase Collection { get; }
        protected IServer RedisServer { get; }

        public RedisLuaTest(IRedisClient redisClient)
        {
            Collection = redisClient.GetCollection();
            RedisServer = redisClient.GetServer();
        }

        public async Task<SigninRequestModel> OnUserSignin(SigninRequestModel request)
        {
            return request;
        }
    }
}
