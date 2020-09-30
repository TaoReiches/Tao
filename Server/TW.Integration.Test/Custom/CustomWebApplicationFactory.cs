using System;
using Microsoft.AspNetCore;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Mvc.Testing;
using NLog.Web;

namespace TW.Integration.Test.Custom
{
    public class CustomWebApplicationFactory<TStartup>
        : WebApplicationFactory<TStartup> where TStartup : class
    {
        protected override IWebHostBuilder CreateWebHostBuilder() =>
            WebHost.CreateDefaultBuilder()
                .UseStartup<CustomStartUp>()
                .UseUrls("http://*:8080")
                .UseNLog()
                .UseShutdownTimeout(TimeSpan.FromSeconds(10));
    }
}
