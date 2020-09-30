using System;
using System.Collections.Generic;
using System.Linq.Expressions;
using System.Threading.Tasks;

namespace TW.DAL.Storage
{
    public interface IPersistentRepository
    {
        Task CommitChangesAsync();
    }

    public interface IRepository<TEntity>
    {
        Task<TEntity> GetByPredicateAsync(Expression<Func<TEntity, bool>> predicate);

        Task<ICollection<TEntity>> GetListByPredicateAsync(Expression<Func<TEntity, bool>> predicate);

        Task UpsertAsync(TEntity entity);

        Task InsertAsync(TEntity entity);

        Task RemoveAsync(TEntity entity);

        Task RemoveRangeByPredicateAsync(Expression<Func<TEntity, bool>> filter);

        Task<TEntity> FindByPredicateAndDeleteAsync(Expression<Func<TEntity, bool>> predicate);
    }
}
