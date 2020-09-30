using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using TW.DAL.Models.Tokens;
using NLog;
using System.Linq;
using System;

namespace TW.DAL.Storage.Repositories
{
    public interface ITokenRepository
    {
        Task<IList<TokenModel>> GetTokensDataByUserIdAsync(long userId);

        Task<TokenModel> GetTokenAsync(long userId);

        Task UpsertToken(TokenModel token);
    }

    public class TokenRepository : EntityFrameworkRepository<TokenModel>, IRepository<TokenModel>, IPersistentRepository, ITokenRepository
    {
        private readonly DbSet<TokenModel> _tokenData;
        private readonly ILogger _logger = LogManager.GetCurrentClassLogger();

        public TokenRepository(TWContext context) : base(context, context.TokenData)
        {
            _tokenData = context.TokenData;
        }

        public async Task<IList<TokenModel>> GetTokensDataByUserIdAsync(long userId)
        {
            try
            {
                return await _tokenData.AsNoTracking().Where(
                    t => t.RequesterId == userId).Include(t => t.UserInfo).ToListAsync();
            }
            catch (InvalidOperationException)
            {
                _logger.Error($"Cannot get token data due to no record with user id: {userId} in the database.");
                throw;
            }
        }

        public async Task<TokenModel> GetTokenAsync(long userId)
        {
            return await GetByPredicateAsync(t => t.RequesterId == userId);
        }

        public async Task UpsertToken(TokenModel token)
        {
            await UpsertAsync(token);
            await CommitChangesAsync();
        }
    }
}
