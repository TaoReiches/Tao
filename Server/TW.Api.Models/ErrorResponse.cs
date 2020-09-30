using System.Net;

namespace TW.Api.Models
{
    public class ErrorResponse : BasicResponse
    {
        public ErrorResponse(string message, HttpStatusCode httpStatusCode) : base(message, httpStatusCode)
        {
        }
    }
}
