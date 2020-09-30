using AutoMapper;
using TW.Api.Models.Common;
using TW.Api.Models.Users;
using TW.Common.Tools;
using TW.DAL.Models.Users;

namespace TW.Mappings.Profiles
{
    internal static class UserInfoProfile
    {
        internal static void Construct(Profile currentProfile)
        {
            string _salt = string.Empty;

            currentProfile.CreateMap<CreateUserRequestModel, UserInfo>()
                .BeforeMap((_, __) => _salt = CryptUtils.GenerateSalt())
                .ForMember(dest => dest.Salt, opt => opt.MapFrom(_ => _salt))
                .ForMember(dest => dest.Role, opt => opt.MapFrom(_ => EUserRights.User))
                .ForMember(dest => dest.Password, opt => opt.MapFrom(src => CryptUtils.GetHashedPassword(src.Password, _salt)));

            currentProfile.CreateMap<UserInfo, UserInfoResultModel>()
                .ForMember(dest => dest.UserId, opt => opt.MapFrom(src => src.Id))
                .ForMember(dest => dest.UserName, opt => opt.MapFrom(src => src.UserName))
                .ForMember(dest => dest.Email, opt => opt.MapFrom(src => src.EMail));
        }
    }
}
