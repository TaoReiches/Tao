using System.Net;

namespace TW.Api.Models
{
    public interface IResponse
    {
        HttpStatusCode ResponseCode { get; set; }

        string Message { get; set; }
    }
}
