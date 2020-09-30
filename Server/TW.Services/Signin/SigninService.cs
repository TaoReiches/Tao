using System;
using System.Globalization;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Net;
using System.Security.Claims;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Extensions.Configuration;
using Microsoft.IdentityModel.Tokens;
using TW.Api.Models;
using TW.Api.Models.Common;
using TW.Api.Models.Signin;
using TW.Configuration;
using TW.DAL.Models.Tokens;
using TW.DAL.Storage.Repositories;
using TW.Services.Users;

namespace TW.Services.Signin
{
    public class SigninService : ISigninService
    {
        private readonly ITokenRepository _tokenRepository;
        private readonly IUserInfoRepository _userInfoRepository;
        private readonly IUserValidationService _userValidationService;
        private readonly TokensGeneratorConfiguration _configuration;

        public SigninService(
            IConfiguration configuration,
            ITokenRepository tokenRepository,
            IUserInfoRepository userInfoRepository,
            IUserValidationService userValidationService)
        {
            _tokenRepository = tokenRepository;
            _userInfoRepository = userInfoRepository;
            _userValidationService = userValidationService;
            _configuration = configuration.GetSection("Jwt").Get<TokensGeneratorConfiguration>();
        }

        public async Task<IResponse> GetSigninTokenAsync(SigninRequestModel signinRequest)
        {
            var userInfo = await _userInfoRepository.GetUserInfoByIdentifierAsync(signinRequest.UserIdentifier);
            if(userInfo == null)
            {
                return new ErrorResponse("Specified users not exist!", HttpStatusCode.NotFound);
            }

            if (!_userValidationService.IsPasswordValid(userInfo, signinRequest.Password))
            {
                return new ErrorResponse("Password error!", HttpStatusCode.Forbidden);
            }

            var claims = CreateUserClaims(userInfo.UserName, userInfo.Id, userInfo.Role);
            var token = await BuildTokenAsync(claims);
            var refreshToken = await BuildTokenAsync(claims, DateTime.UtcNow.AddMinutes(_configuration.RefreshTokenExpirationMinutes));

            var tokenModel = await _tokenRepository.GetTokenAsync(userInfo.Id) ??
                new TokenModel
                {
                    RequesterId = userInfo.Id
                };
            tokenModel.RefreshToken = refreshToken;

            await _tokenRepository.UpsertToken(tokenModel);

            return new SigninResultModel
            {
                Token = token,
                RefreshToken = refreshToken
            };
        }

        public async Task<string> RefreshTokenAsync(string token, string refreshToken)
        {
            if (string.IsNullOrEmpty(token) || string.IsNullOrEmpty(refreshToken))
            {
                return null;
            }

            var handle = new JwtSecurityTokenHandler();
            JwtSecurityToken jwtSecurityToken;
            try
            {
                jwtSecurityToken = handle.ReadJwtToken(token);
            }
            catch (ArgumentException)
            {
                return null;
            }

            string userIdString = jwtSecurityToken.Claims.FirstOrDefault(c => c.Type == "userId")?.Value;
            if(!long.TryParse(userIdString, out long userId))
            {
                return null;
            }

            var tokenModel = (await _tokenRepository.GetTokensDataByUserIdAsync(userId)).FirstOrDefault(
                tm => string.Equals(tm.RefreshToken, refreshToken));
            if (tokenModel == null)
            {
                return null;
            }

            var jwtRefreshToken = handle.ReadJwtToken(refreshToken);
            if (jwtRefreshToken.ValidTo < DateTime.Now)
            {
                return null;
            }

            var claims = CreateUserClaims(tokenModel.UserInfo.UserName, tokenModel.UserInfo.Id, tokenModel.UserInfo.Role);
            return await BuildTokenAsync(claims);
        }

        private Claim[] CreateUserClaims(string userName, long id, EUserRights role)
        {
            return new[]
            {
                new Claim(JwtRegisteredClaimNames.Jti, Guid.NewGuid().ToString()),
                new Claim(ClaimTypes.Name, userName),
                new Claim(ClaimTypes.Role, role.ToString()),
                new Claim("userId", id.ToString(CultureInfo.InvariantCulture))
            };
        }

        private async Task<string> BuildTokenAsync(Claim[] claims, DateTime tokenLifeTime)
        {
            var key = new SymmetricSecurityKey(Encoding.UTF8.GetBytes(_configuration.EncryptKey));
            var creds = new SigningCredentials(key, SecurityAlgorithms.HmacSha256);

            var token = new JwtSecurityToken(
                _configuration.Issuer,
                _configuration.Audience,
                claims,
                null,
                tokenLifeTime,
                creds);

            await Task.CompletedTask;

            return new JwtSecurityTokenHandler().WriteToken(token);
        }

        private async Task<string> BuildTokenAsync(Claim[] claims)
        {
            return await BuildTokenAsync(claims, DateTime.UtcNow.AddMinutes(_configuration.TokenExpirationMinutes));
        }
    }
}
