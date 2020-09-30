using System.Collections.Generic;
using System.Threading.Tasks;
using TW.DAL.Models.Users;

namespace TW.DAL.Storage.Repositories
{
    public interface IUserInfoRepository
    {
        Task<UserInfo> GetUserInfoByIdentifierAsync(string userIdentifier);

        Task<UserInfo> GetUserInfoByIdAsync(long userId);

        Task<ICollection<UserInfo>> SearchUsersAsync(string query);

        Task CreateNewUserAsync(UserInfo userInfo);

        Task UpdateUserInfoAsync(UserInfo userInfo);
    }

    public class UserInfoRepository : EntityFrameworkRepository<UserInfo>, IUserInfoRepository, IPersistentRepository
    {
        public UserInfoRepository(TWContext context) : base(context, context.UserInfoData)
        {
        }

        public async Task CreateNewUserAsync(UserInfo userInfo)
        {
            await InsertAsync(userInfo);
            await CommitChangesAsync();
        }

        public async Task<UserInfo> GetUserInfoByIdAsync(long userId)
        {
            return await GetByPredicateAsync(u => u.Id == userId);
        }

        public async Task<UserInfo> GetUserInfoByIdentifierAsync(string userIdentifier)
        {
            return await GetByPredicateAsync(u =>
            string.Equals(u.UserName, userIdentifier) ||
            string.Equals(u.EMail, userIdentifier));
        }

        public async Task<ICollection<UserInfo>> SearchUsersAsync(string query)
        {
            return await GetListByPredicateAsync(u =>
            u.UserName.Contains(query));
        }

        public async Task UpdateUserInfoAsync(UserInfo userInfo)
        {
            await UpsertAsync(userInfo);
            await CommitChangesAsync();
        }
    }
}
