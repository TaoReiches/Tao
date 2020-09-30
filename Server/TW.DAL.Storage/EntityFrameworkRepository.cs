using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Linq.Expressions;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using TW.DAL.Models;

namespace TW.DAL.Storage
{
    public abstract class EntityFrameworkRepository<TEntity> : IRepository<TEntity>, IPersistentRepository where TEntity : MySqlEntity
    {
        private readonly DbContext _context;
        protected virtual DbSet<TEntity> DataSet { get; }

        protected EntityFrameworkRepository(DbContext context, DbSet<TEntity> dataSet)
        {
            DataSet = dataSet;
            _context = context;
        }

        public async Task CommitChangesAsync()
        {
            await _context.SaveChangesAsync();
        }

        public virtual async Task<TEntity> FindByPredicateAndDeleteAsync(Expression<Func<TEntity, bool>> predicate)
        {
            var entity = await DataSet.SingleAsync(predicate);
            DataSet.Remove(entity);

            return entity;
        }

        public virtual async Task<TEntity> GetByPredicateAsync(Expression<Func<TEntity, bool>> predicate)
        {
            return await DataSet.AsNoTracking().FirstOrDefaultAsync(predicate);
        }

        public virtual async Task<ICollection<TEntity>> GetListByPredicateAsync(Expression<Func<TEntity, bool>> predicate)
        {
            return await DataSet.AsNoTracking().Where(predicate).ToListAsync();
        }

        public virtual async Task InsertAsync(TEntity entity)
        {
            await Task.CompletedTask;
            DataSet.Add(entity);
        }

        public virtual async Task RemoveAsync(TEntity entity)
        {
            await Task.CompletedTask;
            DataSet.Remove(entity);
        }

        public virtual async Task RemoveRangeByPredicateAsync(Expression<Func<TEntity, bool>> filter)
        {
            var entitiesToRemove = await GetListByPredicateAsync(filter);
            if (entitiesToRemove.Count > 0)
            {
                DataSet.RemoveRange(entitiesToRemove);
            }
        }

        public virtual async Task UpsertAsync(TEntity entity)
        {
            await Task.CompletedTask;

            if (entity.Id == 0)
            {
                DataSet.Add(entity);
            }
            else
            {
                entity.Updated = DateTime.UtcNow;
                DataSet.Update(entity);
            }
        }
    }
}
