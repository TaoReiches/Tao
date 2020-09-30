using Microsoft.EntityFrameworkCore.Migrations;

namespace TW.DAL.Storage.Migrations
{
    public partial class r2Migration : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<int>(
                name: "Role",
                table: "UserInfo",
                nullable: false,
                defaultValue: 0);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "Role",
                table: "UserInfo");
        }
    }
}
