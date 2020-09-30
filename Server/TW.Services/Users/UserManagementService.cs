using System.Net;
using System.Threading.Tasks;
using AutoMapper;
using TW.Api.Models;
using TW.Api.Models.Users;
using TW.DAL.Models.Users;
using TW.DAL.Storage.Repositories;

namespace TW.Services.Users
{
    public class UserManagementService : IUserManagementService
    {
        private readonly IUserInfoRepository _userInfoRepository;
        private readonly IMapper _mapper;

        public UserManagementService(
            IUserInfoRepository userInfoRepository,
            IMapper mapper)
        {
            _userInfoRepository = userInfoRepository;
            _mapper = mapper;
        }

        public async Task<IResponse> CreateUserAsync(CreateUserRequestModel request)
        {
            // Check the username, it must be unique.
            var existUser = await _userInfoRepository.GetUserInfoByIdentifierAsync(request.UserName);
            if (existUser != null)
            {
                return new ErrorResponse("Username has been used.", HttpStatusCode.Conflict);
            }

            var newUser = _mapper.Map<UserInfo>(request);
            await _userInfoRepository.CreateNewUserAsync(newUser);

            return new SuccessResponse(HttpStatusCode.Created);
        }

        public async Task<IResponse> GetUserInfoAsync(string userIdentifier)
        {
            var userInfo = await _userInfoRepository.GetUserInfoByIdentifierAsync(userIdentifier);
            if (userInfo == null)
            {
                return new ErrorResponse("User does not found", HttpStatusCode.NotFound);
            }

            return _mapper.Map<UserInfoResultModel>(userInfo);
        }
    }
}
