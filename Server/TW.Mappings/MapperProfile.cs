using AutoMapper;
using TW.Mappings.Profiles;

namespace TW.Mappings
{
    public class MapperProfile : Profile
    {
        public MapperProfile()
        {
            UserInfoProfile.Construct(this);
        }
    }
}
