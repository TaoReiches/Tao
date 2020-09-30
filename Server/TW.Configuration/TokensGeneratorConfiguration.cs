namespace TW.Configuration
{
    public class TokensGeneratorConfiguration
    {
        public string Issuer { get; set; }

        public string Audience { get; set; }

        public string EncryptKey { get; set; }

        public uint TokenExpirationMinutes { get; set; }

        public uint RefreshTokenExpirationMinutes { get; set; }
    }
}
