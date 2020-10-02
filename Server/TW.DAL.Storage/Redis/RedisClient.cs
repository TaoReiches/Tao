
using System;
using Microsoft.Extensions.Configuration;
using StackExchange.Redis;

namespace TW.DAL.Storage.Redis
{
    public interface IRedisClient
    {
        IDatabase GetCollection();

        IServer GetServer();
    }

    public class RedisClient : IRedisClient
    {
        private readonly IDatabase _database;
        private readonly IServer _server;

        public RedisClient(IConfiguration configuration)
        {
            var lazyConnection = new Lazy<ConnectionMultiplexer>(() =>
            {
                string cacheConnection = configuration.GetConnectionString("RedisConnection");
                return ConnectionMultiplexer.Connect(cacheConnection);
            });
            _database = lazyConnection.Value.GetDatabase();
            _server = lazyConnection.Value.GetServer(_database.IdentifyEndpoint());
        }

        public IDatabase GetCollection()
        {
            return _database;
        }

        public IServer GetServer()
        {
            return _server;
        }
    }
}
