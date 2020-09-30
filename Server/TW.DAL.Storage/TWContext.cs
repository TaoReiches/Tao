using Microsoft.EntityFrameworkCore;
using TW.DAL.Models.Tokens;
using TW.DAL.Models.Users;

namespace TW.DAL.Storage
{
    public class TWContext : DbContext
    {
        public TWContext(DbContextOptions<TWContext> contextOptions) : base(contextOptions)
        {
        }

        public DbSet<UserInfo> UserInfoData { get; set; }

        public DbSet<TokenModel> TokenData { get; set; }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            base.OnModelCreating(modelBuilder);

            modelBuilder.Entity<UserInfo>()
                .HasIndex(u => u.UserName)
                .IsUnique();
        }
    }
}
