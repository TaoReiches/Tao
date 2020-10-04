using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Newtonsoft.Json;
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
            //// initialize test data
            //var rand = new Random();
            //for( var i = 0; i < 20; ++i)
            //{
            //    await Collection.HashSetAsync($"user:{i}", "username", $"testuser-{i}");
            //    await Collection.HashSetAsync($"user:{i}", "password", $"testpassword-{i*100}");
            //    await Collection.HashSetAsync($"user:{i}", "state", rand.Next(0,100) > 50 ? 1 : 2);
            //}

            var script = @"
            local name = @name
            local password = @password
            local hashname = 'user:'..name
            redis.call('HSET', hashname, 'username', name)
            redis.call('HSET', hashname, 'password', password)
            redis.call('HSET', hashname, 'state', 2)
            local keys = redis.call('KEYS', 'user:*')
            for i, key in ipairs(keys) do
                local state = redis.call('HGET', key, 'state')
                if state == '1' then
                    return redis.call('HGETALL', key)
                end
            end
            return nil
            ";

            //script = @"
            //local previous = redis.call('HEXISTS', 'none', 'none');
            //if previous == 0 then
            //    return 6
            //else
            //    return 8
            //end
            //";

            //script = @"
            //return redis.call('HGETALL', 'none');
            //";

            var prepared = LuaScript.Prepare(script);
            var loaded = await prepared.LoadAsync(RedisServer);

            var list = new List<long> { 888, 999, 666 };
            var str = JsonConvert.SerializeObject(list);

            long testlong = 9898988;
            bool testbool = false;

            var result = await loaded.EvaluateAsync(Collection, new { name = testEnum.test2.ToString(), password = DateTime.UtcNow.ToString() });
            //var dic = result.ToDictionary();
            //foreach(var res in dic)
            //{
            //    switch(res.Key)
            //    {
            //        case "username":
            //            {
            //                string a = res.Value.ToString();
            //                break;
            //            }
            //        case "password":
            //            long b = (long)res.Value;
            //            break;
            //    }
            //}

            return request;
        }
    }

    enum testEnum
    {
        test1,
        test2
    }
}
