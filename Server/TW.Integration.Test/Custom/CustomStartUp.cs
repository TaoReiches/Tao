using System;
using AutoMapper;
using FluentValidation.AspNetCore;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using TW.Api.Validation;
using TW.DAL.Storage;
using TW.Integration.Test.Helpers;
using TW.Mappings;
using WebApplication.ConfigurationExtensions;
using WebApplication.Middleware;
using WebApplication.Validate;

namespace TW.Integration.Test.Custom
{
    public class CustomStartUp
    {
        private const bool useInmemoryDatabase = true;

        public CustomStartUp(IConfiguration configuration)
        {
            Configuration = configuration;
        }

        public IConfiguration Configuration { get; }

        // This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddTWSwagger();

            services.AddAutoMapper(typeof(MapperProfile));

            services.AddMvc(opt =>
            {
                opt.EnableEndpointRouting = false;
                opt.Filters.Add(typeof(ValidatorActionFilter));
            })
            .SetCompatibilityVersion(CompatibilityVersion.Version_3_0)
            .AddFluentValidation(fv =>
            {
                fv.RegisterValidatorsFromAssemblyContaining<IApiValidator>();
                //fv.RegisterValidatorsFromAssembly(Assembly.LoadFrom("TW.Api.Models"));
                fv.RunDefaultMvcValidationAfterFluentValidationExecutes = false;
            });

            services.AddTWAuthentification(Configuration);

#pragma warning disable 162
            if (useInmemoryDatabase)
            {
                services.AddDbContext<TWContext>(opt =>
                opt.UseInMemoryDatabase("InMemoryDatabase"));
            }
            else
            {
                services.AddDbContext<TWContext>(opt =>
                    opt.UseMySql(
                        Configuration.GetConnectionString("MySqlConnection"),
                        builderOptions => builderOptions.MigrationsAssembly("TW.DAL.Storage")));
            }
#pragma warning restore 162

            services.AddTWDependenciesInjection();

            services.AddHttpContextAccessor();
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env)
        {
            app.UseMiddleware<ExceptionHandingMiddleware>();


            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseAuthentication();
            app.UseTWSwagger();

            app.UseMvc(routes => routes.MapRoute("default", "/api/{controller}/"));

            using var serviceScope = app.ApplicationServices.GetRequiredService<IServiceScopeFactory>().CreateScope();
            using var context = serviceScope.ServiceProvider.GetService<TWContext>();
#pragma warning disable 162
            if (useInmemoryDatabase)
            {
                context.Database.EnsureCreated();
                try
                {
                    DatabaseHelper.InitializeDbForTests(context);
                }
                catch (Exception ex)
                {
                    throw (ex);
                }
            }
            else
            {
                context.Database.Migrate();
            }
#pragma warning restore 162
        }
    }
}
