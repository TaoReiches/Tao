using System;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
using NLog.Web;

namespace WebApplication
{
    public class Program
    {
        public static void Main(string[] args)
        {
            AdjustNLogTarget();

            CreateWebHostBuilder(args).Build().Run();
        }

        public static IWebHostBuilder CreateWebHostBuilder(string[] args) =>
            WebHost.CreateDefaultBuilder(args)
                .UseStartup<Startup>()
                .UseUrls("http://*:8080")
                .UseNLog()
                .UseShutdownTimeout(TimeSpan.FromSeconds(10));

        private static void AdjustNLogTarget()
        {
            var configuration = NLog.LogManager.Configuration;
#if DEBUG
            NLog.LogManager.GetCurrentClassLogger().Info("DisableNLogAzureTarget");
            configuration.RemoveTarget("Azure");
#else
            NLog.LogManager.GetCurrentClassLogger().Info("DisableNLogFileTarget");
            configuration.RemoveTarget("file");
#endif
            NLog.LogManager.Configuration = configuration;
        }
    }
}
