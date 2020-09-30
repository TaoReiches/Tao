using System;
using System.Security.Cryptography;
using System.Text;

namespace TW.Common.Tools
{
    public static class CryptUtils
    {
        private const ushort SaltLength = 32;
        private static readonly char[] _invitationTokenAllowedChars =
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890".ToCharArray();

        public static string GetHashedPassword(string password, string salt)
        {
            var encoding = new UTF8Encoding();
            var passwordBytes = encoding.GetBytes(password);
            var saltBytes = encoding.GetBytes(salt);

            using (var hmac = new HMACSHA256(saltBytes))
            {
                var hashedPassword = hmac.ComputeHash(passwordBytes);

                var saltedPassword = new byte[hashedPassword.Length + saltBytes.Length];
                Buffer.BlockCopy(hashedPassword, 0, saltedPassword, 0, hashedPassword.Length);
                Buffer.BlockCopy(saltBytes, 0, saltedPassword, hashedPassword.Length, saltBytes.Length);

                return Convert.ToBase64String(hmac.ComputeHash(saltedPassword));
            }
        }

        public static string GenerateSalt()
        {
            using (var random = new RNGCryptoServiceProvider())
            {
                var salt = new byte[SaltLength];

                random.GetNonZeroBytes(salt);

                return Convert.ToBase64String(salt);
            }
        }

        public static string GenerateToken(int length)
        {
            if (length < 1 || length > 128)
            {
                throw new ArgumentException(nameof(length));
            }

            using (var rng = RandomNumberGenerator.Create())
            {
                var byteBuffer = new byte[length];

                rng.GetBytes(byteBuffer);

                var characterBuffer = new char[length];

                for (var iter = 0; iter < length; ++iter)
                {
                    var i = byteBuffer[iter] % _invitationTokenAllowedChars.Length;

                    characterBuffer[iter] = _invitationTokenAllowedChars[i];
                }

                return new string(characterBuffer);
            }
        }
    }
}
