using System;
using Microsoft.EntityFrameworkCore.Metadata;
using Microsoft.EntityFrameworkCore.Migrations;

namespace TW.DAL.Storage.Migrations
{
    public partial class r1Migration : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "UserInfo",
                columns: table => new
                {
                    Id = table.Column<long>(nullable: false)
                        .Annotation("MySql:ValueGenerationStrategy", MySqlValueGenerationStrategy.IdentityColumn),
                    Updated = table.Column<DateTime>(nullable: false),
                    UserName = table.Column<string>(nullable: true),
                    EMail = table.Column<string>(nullable: true),
                    Password = table.Column<string>(nullable: true),
                    Salt = table.Column<string>(nullable: true)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_UserInfo", x => x.Id);
                });

            migrationBuilder.CreateTable(
                name: "TokenModel",
                columns: table => new
                {
                    Id = table.Column<long>(nullable: false)
                        .Annotation("MySql:ValueGenerationStrategy", MySqlValueGenerationStrategy.IdentityColumn),
                    Updated = table.Column<DateTime>(nullable: false),
                    RequesterId = table.Column<long>(nullable: false),
                    RefreshToken = table.Column<string>(nullable: true),
                    MachineId = table.Column<long>(nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_TokenModel", x => x.Id);
                    table.ForeignKey(
                        name: "FK_TokenModel_UserInfo_RequesterId",
                        column: x => x.RequesterId,
                        principalTable: "UserInfo",
                        principalColumn: "Id",
                        onDelete: ReferentialAction.Cascade);
                });

            migrationBuilder.CreateIndex(
                name: "IX_TokenModel_RequesterId",
                table: "TokenModel",
                column: "RequesterId");

            migrationBuilder.CreateIndex(
                name: "IX_UserInfo_UserName",
                table: "UserInfo",
                column: "UserName",
                unique: true);
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "TokenModel");

            migrationBuilder.DropTable(
                name: "UserInfo");
        }
    }
}
