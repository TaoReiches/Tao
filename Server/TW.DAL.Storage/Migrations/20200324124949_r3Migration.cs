using Microsoft.EntityFrameworkCore.Migrations;

namespace TW.DAL.Storage.Migrations
{
    public partial class r3Migration : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "MachineId",
                table: "TokenModel");
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.AddColumn<long>(
                name: "MachineId",
                table: "TokenModel",
                type: "bigint",
                nullable: false,
                defaultValue: 0L);
        }
    }
}
