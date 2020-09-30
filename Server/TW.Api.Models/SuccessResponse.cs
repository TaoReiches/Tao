using System.Net;

namespace TW.Api.Models
{
    public class SuccessResponse : BasicResponse
    {
        public SuccessResponse(HttpStatusCode httpStatusCode = HttpStatusCode.OK) : base(httpStatusCode)
        {
        }

        public SuccessResponse(string message, HttpStatusCode httpStatusCode) : base(message, httpStatusCode)
        {
        }
    }
}
