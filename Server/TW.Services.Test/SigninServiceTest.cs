using System;
using System.Collections.Generic;
using System.IdentityModel.Tokens.Jwt;
using System.Linq;
using System.Net;
using System.Security.Claims;
using System.Threading.Tasks;
using FluentAssertions;
using Microsoft.Extensions.Configuration;
using NSubstitute;
using NUnit.Framework;
using TW.Api.Models;
using TW.Api.Models.Common;
using TW.Api.Models.Signin;
using TW.Configuration;
using TW.DAL.Models.Tokens;
using TW.DAL.Models.Users;
using TW.DAL.Storage.Repositories;
using TW.Services.Signin;
using TW.Services.Users;

namespace TW.Services.Test
{
    [TestFixture]
    [Category("UnitTests")]
    public class SigninServiceTest
    {
        private const string DefaultUserName = "tw";
        private const long DefaultUserId = 9;
        private const EUserRights DefaultUserRole = EUserRights.Service;

        private ITokenRepository _tokenRepository;
        private IUserInfoRepository _userInfoRepository;
        private IUserValidationService _userValidationService;
        private IConfiguration _configuration;

        private SigninService _signinService;

        [SetUp]
        public void SetUp()
        {
            _configuration = Substitute.For<IConfiguration>();
            _userInfoRepository = Substitute.For<IUserInfoRepository>();
            _userValidationService = Substitute.For<IUserValidationService>();
            _tokenRepository = Substitute.For<ITokenRepository>();

            SetUpConfiguration();

            _signinService = new SigninService(
                _configuration,
                _tokenRepository,
                _userInfoRepository,
                _userValidationService);
        }

        [TestCase("t@w.a")]
        [TestCase("a")]
        public async Task ShouldReturnNotFoundIfNoUserWithSpecifiedIdentifier(string userIdentifier)
        {
            var request = new SigninRequestModel
            {
                UserIdentifier = userIdentifier
            };

            var res = await _signinService.GetSigninTokenAsync(request);

            res.Should().BeOfType<ErrorResponse>();
            res.ResponseCode.Should().Be(HttpStatusCode.NotFound);
        }

        [TestCase("t@w.a")]
        [TestCase("a")]
        public async Task ShouldReturnOKIfUserWasFound(string userIdentifier)
        {
            var request = new SigninRequestModel
            {
                UserIdentifier = userIdentifier
            };
            _userInfoRepository.GetUserInfoByIdentifierAsync(Arg.Any<string>()).Returns(new UserInfo()
            {
                UserName = DefaultUserName,
                Id = DefaultUserId,
                Role = DefaultUserRole
            });
            _userValidationService.IsPasswordValid(Arg.Any<UserInfo>(), Arg.Any<string>()).Returns(true);

            TokenModel tokenModel = new TokenModel();
            _tokenRepository.GetTokenAsync(Arg.Any<long>()).Returns(null as TokenModel);
            _tokenRepository.UpsertToken(Arg.Do<TokenModel>(model => tokenModel = model)).Returns(Task.CompletedTask);

            var res = await _signinService.GetSigninTokenAsync(request);
            var signinResult = res as SigninResultModel;
            var handler = new JwtSecurityTokenHandler();
            var generatedToken = handler.ReadJwtToken(signinResult.Token) as JwtSecurityToken;

            res.Should().BeOfType<SigninResultModel>();
            res.ResponseCode.Should().Be(HttpStatusCode.OK);

            generatedToken.Claims.FirstOrDefault(c => c.Type == "userId")?.Value.Should().Be(DefaultUserId.ToString());
            generatedToken.Claims.FirstOrDefault(c => c.Type == ClaimTypes.Role)?.Value.Should().Be(nameof(EUserRights.Service));

            tokenModel.RequesterId.Should().Be(DefaultUserId);
            tokenModel.RefreshToken.Should().BeEquivalentTo(signinResult.RefreshToken);
        }

        [Test]
        public async Task ShouldReturnNullWhenRefreshTokenDoesNotMatch()
        {
            _tokenRepository.GetTokensDataByUserIdAsync(DefaultUserId).Returns(new List<TokenModel> 
            {
                new TokenModel
                {
                    RequesterId = DefaultUserId,
                    RefreshToken = "qwer"
                }
            });

            var token = new JwtSecurityToken(claims: new[]
            {
                new Claim("userId", DefaultUserId.ToString())
            });
            var jwt = new JwtSecurityTokenHandler().WriteToken(token);
            var res = await _signinService.RefreshTokenAsync(jwt, "asd11");

            res.Should().BeNull();
        }

        [TestCase]
        public async Task ShouldReturnNullWhenRefreshTokenExpired()
        {
            var jwt = new JwtSecurityToken(claims: new[]
            {
                new Claim("userId", DefaultUserId.ToString())
            });
            var token = new JwtSecurityTokenHandler().WriteToken(jwt);

            var jwtRefreshToken = new JwtSecurityToken(claims: new[]
                {
                    new Claim("userId", DefaultUserId.ToString())
                },
                expires: DateTime.UtcNow);
            var refreshToken = new JwtSecurityTokenHandler().WriteToken(jwtRefreshToken);
            _tokenRepository.GetTokensDataByUserIdAsync(DefaultUserId).Returns(
                new List<TokenModel>
                {
                    new TokenModel
                    {
                        RequesterId = DefaultUserId,
                        RefreshToken = refreshToken,
                        UserInfo = new UserInfo()
                    }
                });

            var res = await _signinService.RefreshTokenAsync(token, refreshToken);

            res.Should().BeNull();
        }

        [TestCase]
        public async Task ShouldReturnValidTokenWhenRefreshTokenMatched()
        {
            var userInfo = new UserInfo
            {
                Id = DefaultUserId,
                UserName = DefaultUserName,
                Role = DefaultUserRole
            };

            var jwt = new JwtSecurityToken(claims: new[]
            {
                new Claim("userId", DefaultUserId.ToString())
            });
            var token = new JwtSecurityTokenHandler().WriteToken(jwt);

            var jwtRefreshToken = new JwtSecurityToken(claims: new[]
                {
                    new Claim("userId", DefaultUserId.ToString())
                },
                expires: DateTime.UtcNow.AddDays(1));
            var refreshToken = new JwtSecurityTokenHandler().WriteToken(jwtRefreshToken);

            _tokenRepository.GetTokensDataByUserIdAsync(DefaultUserId).Returns(new List<TokenModel>
            {
                new TokenModel
                {
                    RequesterId = DefaultUserId,
                    RefreshToken = refreshToken,
                    UserInfo = userInfo
                }
            });
            var res = await _signinService.RefreshTokenAsync(token, refreshToken);

            var handler = new JwtSecurityTokenHandler();
            var generatedToken = handler.ReadJwtToken(res);
            generatedToken.Claims.FirstOrDefault(c => c.Type == "userId")?.Value.Should().Be(DefaultUserId.ToString());
            generatedToken.Claims.FirstOrDefault(c => c.Type == ClaimTypes.Role)?.Value.Should().Be(nameof(EUserRights.Service));
        }

        private void SetUpConfiguration()
        {
            var config = new TokensGeneratorConfiguration
            {
                Audience = "tw",
                Issuer = "tw",
                EncryptKey = "TestEncryptKeyWithLengthLimitation",
                TokenExpirationMinutes = 1,
                RefreshTokenExpirationMinutes = 1
            };

            _configuration = new ConfigurationBuilder().AddInMemoryCollection(new Dictionary<string, string>
            {
                {"Jwt:" + nameof(config.Audience), config.Audience },
                {"Jwt:" + nameof(config.Issuer), config.Issuer },
                {"Jwt:" + nameof(config.EncryptKey), config.EncryptKey }
            }).Build();
        }
    }
}
