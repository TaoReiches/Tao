using System.Collections.Generic;
using System.Net;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;

namespace WebApplication.Validate
{
    public class ValidatorActionFilter : IActionFilter
    {
        public void OnActionExecuted(ActionExecutedContext context)
        {
        }

        public void OnActionExecuting(ActionExecutingContext context)
        {
            if (!context.ModelState.IsValid)
            {
                var errorMessage = new List<string>();
                foreach (var modelState in context.ModelState.Values)
                {
                    foreach(var error in modelState.Errors)
                    {
                        errorMessage.Add(error.ErrorMessage);
                    }
                }

                context.Result = new JsonResult(errorMessage)
                {
                    StatusCode = (int)HttpStatusCode.ExpectationFailed
                };
            }
        }
    }
}
