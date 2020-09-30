using System.Collections.Generic;
using Microsoft.AspNetCore.Builder;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.OpenApi.Models;

namespace WebApplication.ConfigurationExtensions
{
    public static class ConfigureSwaggerExtension
    {
        public static void AddTWSwagger(this IServiceCollection services)
        {
            services.AddSwaggerGen(sw =>
            {
                sw.SwaggerDoc("v1", new OpenApiInfo { Title = "TW API", Version = "v1" });

                //var xmlFile = $"{Assembly.GetExecutingAssembly().GetName().Name}.XML";
                //var xmlPath = Path.Combine(AppContext.BaseDirectory, xmlFile);
                //sw.IncludeXmlComments(xmlPath);

                //string modelsXmlPath = typeof(IResponse).Assembly.Location.Replace("dll", "xml");
                //sw.IncludeXmlComments(modelsXmlPath);

                //sw.SchemaGeneratorOptions.UseInlineDefinitionsForEnums = true;

                sw.AddSecurityDefinition("Bearer", new OpenApiSecurityScheme
                {
                    Description = "JWT Authorization header using Bearer scheme",
                    Name = "Authorization",
                    In = ParameterLocation.Header,
                    Type = SecuritySchemeType.ApiKey,
                    Scheme = "Bearer"
                });

                var requirement = new OpenApiSecurityRequirement
                {
                    {
                        new OpenApiSecurityScheme
                        {
                            Reference = new OpenApiReference
                            {
                                Type = ReferenceType.SecurityScheme,
                                Id = "Bearer"
                            },
                            Scheme = "oauth2",
                            Name = "Bearer",
                            In = ParameterLocation.Header
                        },
                        new List<string>()
                        }
                };
                sw.AddSecurityRequirement(requirement);

                //sw.RegisterModels();
            });
        }

        public static void UseTWSwagger(this IApplicationBuilder applicationBuilder)
        {
            //applicationBuilder.UseSwagger(c =>
            //{
            //    var basePath = "/api/";
            //    c.PreSerializeFilters.Add((swaggerDoc, httpReq) =>
            //    {
            //        var paths = new OpenApiPaths();
            //        foreach (var path in swaggerDoc.Paths)
            //        {
            //            paths.Add(path.Key.Replace(basePath, "/"), path.Value);
            //        }
            //        swaggerDoc.Paths = paths;
            //    });
            //});
            applicationBuilder.UseSwagger();
            applicationBuilder.UseSwaggerUI(c => c.SwaggerEndpoint("/swagger/v1/swagger.json", "TW API V1"));
        }
    }
}
