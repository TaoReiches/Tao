namespace table_gen
{
    partial class frm_main
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.clb = new System.Windows.Forms.CheckedListBox();
            this.cms_clb = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.SelectAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.SelectNullToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.FilpSelectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.CancelDropDigDropToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.LoadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.ResetToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lblTable = new System.Windows.Forms.Label();
            this.btn_gen_code = new System.Windows.Forms.Button();
            this.btn_gen_table = new System.Windows.Forms.Button();
            this.cb_data = new System.Windows.Forms.CheckBox();
            this.cb_forign = new System.Windows.Forms.CheckBox();
            this.cb_primary = new System.Windows.Forms.CheckBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.out_filter_txt = new System.Windows.Forms.RichTextBox();
            this.cms_main = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.ClearOutputToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.btn_load_table = new System.Windows.Forms.Button();
            this.main_menu = new System.Windows.Forms.MenuStrip();
            this.FileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ResetAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.ExitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.UtilToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenCurrentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripSeparator();
            this.OpenDataSourceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenDataGenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenCodeGenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenClientCodeGenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.OpenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.AboutToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.SelectAllTable = new System.Windows.Forms.CheckBox();
            this.cms_clb.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.cms_main.SuspendLayout();
            this.main_menu.SuspendLayout();
            this.SuspendLayout();
            // 
            // clb
            // 
            this.clb.ContextMenuStrip = this.cms_clb;
            this.clb.FormattingEnabled = true;
            this.clb.Location = new System.Drawing.Point(12, 60);
            this.clb.Name = "clb";
            this.clb.Size = new System.Drawing.Size(267, 372);
            this.clb.TabIndex = 9;
            this.clb.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.clb_ItemCheck);
            // 
            // cms_clb
            // 
            this.cms_clb.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.SelectAllToolStripMenuItem,
            this.SelectNullToolStripMenuItem,
            this.FilpSelectToolStripMenuItem,
            this.CancelDropDigDropToolStripMenuItem,
            this.toolStripMenuItem1,
            this.LoadToolStripMenuItem,
            this.toolStripMenuItem2,
            this.ResetToolStripMenuItem});
            this.cms_clb.Name = "cms_clb";
            this.cms_clb.Size = new System.Drawing.Size(156, 148);
            // 
            // SelectAllToolStripMenuItem
            // 
            this.SelectAllToolStripMenuItem.Name = "SelectAllToolStripMenuItem";
            this.SelectAllToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.SelectAllToolStripMenuItem.Text = "全部选中";
            this.SelectAllToolStripMenuItem.Click += new System.EventHandler(this.SelectAllToolStripMenuItem_Click);
            // 
            // SelectNullToolStripMenuItem
            // 
            this.SelectNullToolStripMenuItem.Name = "SelectNullToolStripMenuItem";
            this.SelectNullToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.SelectNullToolStripMenuItem.Text = "全部取消";
            this.SelectNullToolStripMenuItem.Click += new System.EventHandler(this.SelectNullToolStripMenuItem_Click);
            // 
            // FilpSelectToolStripMenuItem
            // 
            this.FilpSelectToolStripMenuItem.Name = "FilpSelectToolStripMenuItem";
            this.FilpSelectToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.FilpSelectToolStripMenuItem.Text = "反向选中";
            this.FilpSelectToolStripMenuItem.Click += new System.EventHandler(this.FilpSelectToolStripMenuItem_Click);
            // 
            // CancelDropDigDropToolStripMenuItem
            // 
            this.CancelDropDigDropToolStripMenuItem.Name = "CancelDropDigDropToolStripMenuItem";
            this.CancelDropDigDropToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.CancelDropDigDropToolStripMenuItem.Text = "取消Drop/Dip";
            this.CancelDropDigDropToolStripMenuItem.Click += new System.EventHandler(this.CancelDropDigDropToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(152, 6);
            // 
            // LoadToolStripMenuItem
            // 
            this.LoadToolStripMenuItem.Name = "LoadToolStripMenuItem";
            this.LoadToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.LoadToolStripMenuItem.Text = "读取表格";
            this.LoadToolStripMenuItem.Click += new System.EventHandler(this.LoadToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(152, 6);
            // 
            // ResetToolStripMenuItem
            // 
            this.ResetToolStripMenuItem.Name = "ResetToolStripMenuItem";
            this.ResetToolStripMenuItem.Size = new System.Drawing.Size(155, 22);
            this.ResetToolStripMenuItem.Text = "重置";
            this.ResetToolStripMenuItem.Click += new System.EventHandler(this.ResetToolStripMenuItem_Click);
            // 
            // lblTable
            // 
            this.lblTable.AutoSize = true;
            this.lblTable.Location = new System.Drawing.Point(12, 39);
            this.lblTable.Name = "lblTable";
            this.lblTable.Size = new System.Drawing.Size(125, 12);
            this.lblTable.TabIndex = 16;
            this.lblTable.Text = "数据表: 灰色为基本表";
            // 
            // btn_gen_code
            // 
            this.btn_gen_code.ForeColor = System.Drawing.Color.Blue;
            this.btn_gen_code.Location = new System.Drawing.Point(599, 439);
            this.btn_gen_code.Name = "btn_gen_code";
            this.btn_gen_code.Size = new System.Drawing.Size(142, 23);
            this.btn_gen_code.TabIndex = 21;
            this.btn_gen_code.Text = "生成数据结构(程序用)";
            this.btn_gen_code.UseVisualStyleBackColor = true;
            this.btn_gen_code.Click += new System.EventHandler(this.btn_gen_code_Click);
            // 
            // btn_gen_table
            // 
            this.btn_gen_table.ForeColor = System.Drawing.Color.Green;
            this.btn_gen_table.Location = new System.Drawing.Point(458, 439);
            this.btn_gen_table.Name = "btn_gen_table";
            this.btn_gen_table.Size = new System.Drawing.Size(135, 23);
            this.btn_gen_table.TabIndex = 28;
            this.btn_gen_table.Text = "生成表格(策划用)";
            this.btn_gen_table.UseVisualStyleBackColor = true;
            this.btn_gen_table.Click += new System.EventHandler(this.btn_gen_table_Click);
            // 
            // cb_data
            // 
            this.cb_data.AutoSize = true;
            this.cb_data.Checked = true;
            this.cb_data.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cb_data.Enabled = false;
            this.cb_data.ForeColor = System.Drawing.Color.Blue;
            this.cb_data.Location = new System.Drawing.Point(171, 442);
            this.cb_data.Name = "cb_data";
            this.cb_data.Size = new System.Drawing.Size(72, 16);
            this.cb_data.TabIndex = 27;
            this.cb_data.Text = "数据约束";
            this.cb_data.UseVisualStyleBackColor = true;
            this.cb_data.CheckedChanged += new System.EventHandler(this.cb_data_CheckedChanged);
            // 
            // cb_forign
            // 
            this.cb_forign.AutoSize = true;
            this.cb_forign.Checked = true;
            this.cb_forign.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cb_forign.ForeColor = System.Drawing.Color.Blue;
            this.cb_forign.Location = new System.Drawing.Point(93, 443);
            this.cb_forign.Name = "cb_forign";
            this.cb_forign.Size = new System.Drawing.Size(72, 16);
            this.cb_forign.TabIndex = 26;
            this.cb_forign.Text = "外键约束";
            this.cb_forign.UseVisualStyleBackColor = true;
            // 
            // cb_primary
            // 
            this.cb_primary.AutoSize = true;
            this.cb_primary.Checked = true;
            this.cb_primary.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cb_primary.ForeColor = System.Drawing.Color.Blue;
            this.cb_primary.Location = new System.Drawing.Point(15, 443);
            this.cb_primary.Name = "cb_primary";
            this.cb_primary.Size = new System.Drawing.Size(72, 16);
            this.cb_primary.TabIndex = 25;
            this.cb_primary.Text = "主键约束";
            this.cb_primary.UseVisualStyleBackColor = true;
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Location = new System.Drawing.Point(285, 39);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(460, 393);
            this.tabControl1.TabIndex = 29;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.out_filter_txt);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(452, 367);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "输出";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // out_filter_txt
            // 
            this.out_filter_txt.BackColor = System.Drawing.Color.White;
            this.out_filter_txt.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.out_filter_txt.ContextMenuStrip = this.cms_main;
            this.out_filter_txt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.out_filter_txt.Location = new System.Drawing.Point(3, 3);
            this.out_filter_txt.Name = "out_filter_txt";
            this.out_filter_txt.ReadOnly = true;
            this.out_filter_txt.Size = new System.Drawing.Size(446, 361);
            this.out_filter_txt.TabIndex = 14;
            this.out_filter_txt.Text = "";
            // 
            // cms_main
            // 
            this.cms_main.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ClearOutputToolStripMenuItem});
            this.cms_main.Name = "cms_main";
            this.cms_main.Size = new System.Drawing.Size(101, 26);
            // 
            // ClearOutputToolStripMenuItem
            // 
            this.ClearOutputToolStripMenuItem.Name = "ClearOutputToolStripMenuItem";
            this.ClearOutputToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.ClearOutputToolStripMenuItem.Text = "清空";
            this.ClearOutputToolStripMenuItem.Click += new System.EventHandler(this.ClearOutputToolStripMenuItem_Click);
            // 
            // btn_load_table
            // 
            this.btn_load_table.ForeColor = System.Drawing.Color.Red;
            this.btn_load_table.Location = new System.Drawing.Point(321, 439);
            this.btn_load_table.Name = "btn_load_table";
            this.btn_load_table.Size = new System.Drawing.Size(131, 23);
            this.btn_load_table.TabIndex = 30;
            this.btn_load_table.Text = "读取表格(必须)";
            this.btn_load_table.UseVisualStyleBackColor = true;
            this.btn_load_table.Click += new System.EventHandler(this.btn_load_table_Click);
            // 
            // main_menu
            // 
            this.main_menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileToolStripMenuItem,
            this.UtilToolStripMenuItem,
            this.OpenToolStripMenuItem});
            this.main_menu.Location = new System.Drawing.Point(0, 0);
            this.main_menu.Name = "main_menu";
            this.main_menu.Size = new System.Drawing.Size(748, 25);
            this.main_menu.TabIndex = 31;
            // 
            // FileToolStripMenuItem
            // 
            this.FileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ResetAllToolStripMenuItem,
            this.toolStripMenuItem3,
            this.ExitToolStripMenuItem});
            this.FileToolStripMenuItem.Name = "FileToolStripMenuItem";
            this.FileToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.FileToolStripMenuItem.Text = "文件";
            // 
            // ResetAllToolStripMenuItem
            // 
            this.ResetAllToolStripMenuItem.Name = "ResetAllToolStripMenuItem";
            this.ResetAllToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.ResetAllToolStripMenuItem.Text = "重置";
            this.ResetAllToolStripMenuItem.Click += new System.EventHandler(this.ResetAllToolStripMenuItem_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(97, 6);
            // 
            // ExitToolStripMenuItem
            // 
            this.ExitToolStripMenuItem.Name = "ExitToolStripMenuItem";
            this.ExitToolStripMenuItem.Size = new System.Drawing.Size(100, 22);
            this.ExitToolStripMenuItem.Text = "退出";
            this.ExitToolStripMenuItem.Click += new System.EventHandler(this.ExitToolStripMenuItem_Click);
            // 
            // UtilToolStripMenuItem
            // 
            this.UtilToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.OpenCurrentToolStripMenuItem,
            this.toolStripMenuItem4,
            this.OpenDataSourceToolStripMenuItem,
            this.OpenDataGenToolStripMenuItem,
            this.OpenCodeGenToolStripMenuItem,
            this.OpenClientCodeGenToolStripMenuItem});
            this.UtilToolStripMenuItem.Name = "UtilToolStripMenuItem";
            this.UtilToolStripMenuItem.Size = new System.Drawing.Size(48, 21);
            this.UtilToolStripMenuItem.Text = " 辅助";
            // 
            // OpenCurrentToolStripMenuItem
            // 
            this.OpenCurrentToolStripMenuItem.Name = "OpenCurrentToolStripMenuItem";
            this.OpenCurrentToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.OpenCurrentToolStripMenuItem.Text = "打开当前文件夹";
            this.OpenCurrentToolStripMenuItem.Click += new System.EventHandler(this.OpenCurrentToolStripMenuItem_Click);
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(217, 6);
            // 
            // OpenDataSourceToolStripMenuItem
            // 
            this.OpenDataSourceToolStripMenuItem.Name = "OpenDataSourceToolStripMenuItem";
            this.OpenDataSourceToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.OpenDataSourceToolStripMenuItem.Text = "打开策划数据文件夹";
            this.OpenDataSourceToolStripMenuItem.Click += new System.EventHandler(this.OpenDataSourceToolStripMenuItem_Click);
            // 
            // OpenDataGenToolStripMenuItem
            // 
            this.OpenDataGenToolStripMenuItem.Name = "OpenDataGenToolStripMenuItem";
            this.OpenDataGenToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.OpenDataGenToolStripMenuItem.Text = "打开策划生成文件夹";
            this.OpenDataGenToolStripMenuItem.Click += new System.EventHandler(this.OpenDataGenToolStripMenuItem_Click);
            // 
            // OpenCodeGenToolStripMenuItem
            // 
            this.OpenCodeGenToolStripMenuItem.Name = "OpenCodeGenToolStripMenuItem";
            this.OpenCodeGenToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.OpenCodeGenToolStripMenuItem.Text = "打开服务器代码生成文件夹";
            this.OpenCodeGenToolStripMenuItem.Click += new System.EventHandler(this.OpenCodeGenToolStripMenuItem_Click);
            // 
            // OpenClientCodeGenToolStripMenuItem
            // 
            this.OpenClientCodeGenToolStripMenuItem.Name = "OpenClientCodeGenToolStripMenuItem";
            this.OpenClientCodeGenToolStripMenuItem.Size = new System.Drawing.Size(220, 22);
            this.OpenClientCodeGenToolStripMenuItem.Text = "打开客户端代码生成文件夹";
            this.OpenClientCodeGenToolStripMenuItem.Click += new System.EventHandler(this.OpenClientCodeGenToolStripMenuItem_Click);
            // 
            // 关于ToolStripMenuItem
            // 
            this.OpenToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.AboutToolStripMenuItem1});
            this.OpenToolStripMenuItem.Name = "OpenToolStripMenuItem";
            this.OpenToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.OpenToolStripMenuItem.Text = "Open";
            // 
            // AboutToolStripMenuItem1
            // 
            this.AboutToolStripMenuItem1.Name = "AboutToolStripMenuItem1";
            this.AboutToolStripMenuItem1.Size = new System.Drawing.Size(100, 22);
            this.AboutToolStripMenuItem1.Text = "About";
            this.AboutToolStripMenuItem1.Click += new System.EventHandler(this.AboutToolStripMenuItem1_Click);
            // 
            // SelectAllTable
            // 
            this.SelectAllTable.AutoSize = true;
            this.SelectAllTable.ForeColor = System.Drawing.Color.Blue;
            this.SelectAllTable.Location = new System.Drawing.Point(249, 442);
            this.SelectAllTable.Name = "SelectAllTable";
            this.SelectAllTable.Size = new System.Drawing.Size(48, 16);
            this.SelectAllTable.TabIndex = 32;
            this.SelectAllTable.Text = "全选";
            this.SelectAllTable.UseVisualStyleBackColor = true;
            this.SelectAllTable.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // frm_main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(748, 470);
            this.Controls.Add(this.SelectAllTable);
            this.Controls.Add(this.main_menu);
            this.Controls.Add(this.btn_load_table);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.cb_data);
            this.Controls.Add(this.btn_gen_table);
            this.Controls.Add(this.cb_forign);
            this.Controls.Add(this.cb_primary);
            this.Controls.Add(this.btn_gen_code);
            this.Controls.Add(this.lblTable);
            this.Controls.Add(this.clb);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MainMenuStrip = this.main_menu;
            this.MaximizeBox = false;
            this.Name = "frm_main";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "表格转换工具";
            this.Load += new System.EventHandler(this.frm_main_Load);
            this.cms_clb.ResumeLayout(false);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.cms_main.ResumeLayout(false);
            this.main_menu.ResumeLayout(false);
            this.main_menu.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckedListBox clb;
        private System.Windows.Forms.ContextMenuStrip cms_clb;
        private System.Windows.Forms.ToolStripMenuItem LoadToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem SelectAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem FilpSelectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem SelectNullToolStripMenuItem;
        private System.Windows.Forms.Label lblTable;
        private System.Windows.Forms.Button btn_gen_code;
        private System.Windows.Forms.Button btn_gen_table;
        private System.Windows.Forms.CheckBox cb_data;
        private System.Windows.Forms.CheckBox cb_forign;
        private System.Windows.Forms.CheckBox cb_primary;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.RichTextBox out_filter_txt;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem ResetToolStripMenuItem;
        private System.Windows.Forms.Button btn_load_table;
        private System.Windows.Forms.MenuStrip main_menu;
        private System.Windows.Forms.ToolStripMenuItem FileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ResetAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem ExitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem OpenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem AboutToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem UtilToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem OpenCurrentToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem OpenDataSourceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem OpenDataGenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem OpenCodeGenToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip cms_main;
        private System.Windows.Forms.ToolStripMenuItem ClearOutputToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem OpenClientCodeGenToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem CancelDropDigDropToolStripMenuItem;
        private System.Windows.Forms.CheckBox SelectAllTable;
    }
}

