using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace TW.Integration.Test.Helpers
{
    public class HttpHelper
    {
        public static async Task<HttpResponseMessage> SendRequest(
            HttpMethod httpMethod,
            string webApi,
            object content = null,
            bool authorization = true)
        {
            var request = new HttpRequestMessage(httpMethod, $"{DefaultData.BaseUri}{webApi}");

            if (authorization)
            {
                request.Headers.Add("Authorization", $"Bearer {DefaultData.Token}");
            }

            if (content != null)
            {
                request.Content = new StringContent(JsonConvert.SerializeObject(content), Encoding.UTF8, "application/json");
            }

            return await DefaultData.Client.SendAsync(request);
        }

        public static T DeserializeResponse<T>(HttpResponseMessage response)
        {
            var responseString = response.Content.ReadAsStringAsync();
            return JsonConvert.DeserializeObject<T>(responseString.Result);
        }
    }
}
