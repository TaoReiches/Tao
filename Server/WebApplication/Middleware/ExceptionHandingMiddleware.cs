using System;
using System.Net;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;
using NLog;

namespace WebApplication.Middleware
{
    public class ExceptionHandingMiddleware
    {
        private readonly RequestDelegate _next;
        private readonly ILogger _logger = LogManager.GetCurrentClassLogger();

        public ExceptionHandingMiddleware(RequestDelegate next)
        {
            _next = next;
        }

        public async Task Invoke(HttpContext context)
        {
            try
            {
                await _next(context);
            }
            catch (Exception ex)
            {
                _logger.Error(ex);

                context.Response.StatusCode = (int)HttpStatusCode.InternalServerError;
            }
        }
    }
}
