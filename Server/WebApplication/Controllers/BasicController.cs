using System.Net;
using System.Net.Mime;
using Microsoft.AspNetCore.Mvc;
using Newtonsoft.Json;
using NLog;
using TW.Api.Models;
using TW.Common.Tools;

namespace WebApplication.Controllers
{
    [Route(DefaultRoute)]
    [ApiController]
    [Produces(MediaTypeNames.Application.Json)]
    public abstract class BasicController : Controller
    {
        protected const string DefaultRoute = "/api/[controller]";

        protected ILogger Logger { get; } = LogManager.GetCurrentClassLogger();

        protected ContentResult StatusCodeToActionResult(HttpStatusCode statusCode)
        {
            return new ContentResult { StatusCode = (int)statusCode };
        }

        protected JsonResult ResultToJson(IResponse result)
        {
            return new JsonResult(result)
                //new JsonSerializerSettings { NullValueHandling = NullValueHandling.Ignore })
            {
                StatusCode = (int)result.ResponseCode
            };
        }

        protected long GetUserId()
        {
            return UserUtils.GetUserId(HttpContext.User);
        }

        protected long GetUserCompanyId()
        {
            return UserUtils.GetCompanyId(HttpContext.User);
        }
    }
}
