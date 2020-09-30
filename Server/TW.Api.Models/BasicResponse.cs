using System.Net;
using Newtonsoft.Json;

namespace TW.Api.Models
{
    public abstract class BasicResponse : IResponse
    {
        protected BasicResponse()
        {
        }

        protected BasicResponse(HttpStatusCode httpStatusCode)
        {
            ResponseCode = httpStatusCode;
        }

        protected BasicResponse(string message, HttpStatusCode httpStatusCode)
        {
            ResponseCode = httpStatusCode;
            Message = message;
        }

        [JsonIgnore]
        public virtual HttpStatusCode ResponseCode { get; set; } = HttpStatusCode.OK;

        public virtual string Message { get; set; }
    }
}
