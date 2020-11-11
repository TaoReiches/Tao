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
            this.clb.Location = new System.Drawing.Point(18, 100);
            this.clb.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.clb.Name = "clb";
            this.clb.Size = new System.Drawing.Size(398, 602);
            this.clb.TabIndex = 9;
            this.clb.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.clb_ItemCheck);
            // 
            // cms_clb
            // 
            this.cms_clb.ImageScalingSize = new System.Drawing.Size(24, 24);
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
            this.cms_clb.Size = new System.Drawing.Size(312, 208);
            // 
            // SelectAllToolStripMenuItem
            // 
            this.SelectAllToolStripMenuItem.Name = "SelectAllToolStripMenuItem";
            this.SelectAllToolStripMenuItem.Size = new System.Drawing.Size(311, 32);
            this.SelectAllToolStripMenuItem.Text = "SelectAllToolStripMenuItem";
            this.SelectAllToolStripMenuItem.Click += new System.EventHandler(this.SelectAllToolStripMenuItem_Click);
            // 
            // SelectNullToolStripMenuItem
            // 
            this.SelectNullToolStripMenuItem.Name = "SelectNullToolStripMenuItem";
            this.SelectNullToolStripMenuItem.Size = new System.Drawing.Size(311, 32);
            this.SelectNullToolStripMenuItem.Text = "SelectNullToolStripMenuItem";
            this.SelectNullToolStripMenuItem.Click += new System.EventHandler(this.SelectNullToolStripMenuItem_Click);
            // 
            // FilpSelectToolStripMenuItem
            // 
            this.FilpSelectToolStripMenuItem.Name = "FilpSelectToolStripMenuItem";
            this.FilpSelectToolStripMenuItem.Size = new System.Drawing.Size(311, 32);
            this.FilpSelectToolStripMenuItem.Text = "FilpSelectToolStripMenuItem";
            this.FilpSelectToolStripMenuItem.Click += new System.EventHandler(this.FilpSelectToolStripMenuItem_Click);
            // 
            // CancelDropDigDropToolStripMenuItem
            // 
            this.CancelDropDigDropToolStripMenuItem.Name = "CancelDropDigDropToolStripMenuItem";
            this.CancelDropDigDropToolStripMenuItem.Size = new System.Drawing.Size(311, 32);
            this.CancelDropDigDropToolStripMenuItem.Text = "CancelDrop/Dip";
            this.CancelDropDigDropToolStripMenuItem.Click += new System.EventHandler(this.CancelDropDigDropToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(308, 6);
            // 
            // LoadToolStripMenuItem
            // 
            this.LoadToolStripMenuItem.Name = "LoadToolStripMenuItem";
            this.LoadToolStripMenuItem.Size = new System.Drawing.Size(311, 32);
            this.LoadToolStripMenuItem.Text = "LoadTool";
            this.LoadToolStripMenuItem.Click += new System.EventHandler(this.LoadToolStripMenuItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(308, 6);
            // 
            // ResetToolStripMenuItem
            // 
            this.ResetToolStripMenuItem.Name = "ResetToolStripMenuItem";
            this.ResetToolStripMenuItem.Size = new System.Drawing.Size(311, 32);
            this.ResetToolStripMenuItem.Text = "ResetTool";
            this.ResetToolStripMenuItem.Click += new System.EventHandler(this.ResetToolStripMenuItem_Click);
            // 
            // lblTable
            // 
            this.lblTable.AutoSize = true;
            this.lblTable.Location = new System.Drawing.Point(18, 65);
            this.lblTable.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblTable.Name = "lblTable";
            this.lblTable.Size = new System.Drawing.Size(47, 20);
            this.lblTable.TabIndex = 16;
            this.lblTable.Text = "Excel";
            // 
            // btn_gen_code
            // 
            this.btn_gen_code.ForeColor = System.Drawing.Color.Blue;
            this.btn_gen_code.Location = new System.Drawing.Point(898, 732);
            this.btn_gen_code.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_gen_code.Name = "btn_gen_code";
            this.btn_gen_code.Size = new System.Drawing.Size(213, 38);
            this.btn_gen_code.TabIndex = 21;
            this.btn_gen_code.Text = "GenerateCode";
            this.btn_gen_code.UseVisualStyleBackColor = true;
            this.btn_gen_code.Click += new System.EventHandler(this.btn_gen_code_Click);
            // 
            // btn_gen_table
            // 
            this.btn_gen_table.ForeColor = System.Drawing.Color.Green;
            this.btn_gen_table.Location = new System.Drawing.Point(687, 732);
            this.btn_gen_table.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_gen_table.Name = "btn_gen_table";
            this.btn_gen_table.Size = new System.Drawing.Size(202, 38);
            this.btn_gen_table.TabIndex = 28;
            this.btn_gen_table.Text = "GenerateTable";
            this.btn_gen_table.UseVisualStyleBackColor = true;
            this.btn_gen_table.Click += new System.EventHandler(this.btn_gen_table_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Location = new System.Drawing.Point(428, 65);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(690, 655);
            this.tabControl1.TabIndex = 29;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.out_filter_txt);
            this.tabPage1.Location = new System.Drawing.Point(4, 29);
            this.tabPage1.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.tabPage1.Size = new System.Drawing.Size(682, 622);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Output";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // out_filter_txt
            // 
            this.out_filter_txt.BackColor = System.Drawing.Color.White;
            this.out_filter_txt.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.out_filter_txt.ContextMenuStrip = this.cms_main;
            this.out_filter_txt.Dock = System.Windows.Forms.DockStyle.Fill;
            this.out_filter_txt.Location = new System.Drawing.Point(4, 5);
            this.out_filter_txt.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.out_filter_txt.Name = "out_filter_txt";
            this.out_filter_txt.ReadOnly = true;
            this.out_filter_txt.Size = new System.Drawing.Size(674, 612);
            this.out_filter_txt.TabIndex = 14;
            this.out_filter_txt.Text = "";
            // 
            // cms_main
            // 
            this.cms_main.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.cms_main.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ClearOutputToolStripMenuItem});
            this.cms_main.Name = "cms_main";
            this.cms_main.Size = new System.Drawing.Size(124, 36);
            // 
            // ClearOutputToolStripMenuItem
            // 
            this.ClearOutputToolStripMenuItem.Name = "ClearOutputToolStripMenuItem";
            this.ClearOutputToolStripMenuItem.Size = new System.Drawing.Size(123, 32);
            this.ClearOutputToolStripMenuItem.Text = "Clear";
            this.ClearOutputToolStripMenuItem.Click += new System.EventHandler(this.ClearOutputToolStripMenuItem_Click);
            // 
            // btn_load_table
            // 
            this.btn_load_table.ForeColor = System.Drawing.Color.Red;
            this.btn_load_table.Location = new System.Drawing.Point(482, 732);
            this.btn_load_table.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.btn_load_table.Name = "btn_load_table";
            this.btn_load_table.Size = new System.Drawing.Size(196, 38);
            this.btn_load_table.TabIndex = 30;
            this.btn_load_table.Text = "Load";
            this.btn_load_table.UseVisualStyleBackColor = true;
            this.btn_load_table.Click += new System.EventHandler(this.btn_load_table_Click);
            // 
            // main_menu
            // 
            this.main_menu.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.main_menu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FileToolStripMenuItem,
            this.OpenToolStripMenuItem});
            this.main_menu.Location = new System.Drawing.Point(0, 0);
            this.main_menu.Name = "main_menu";
            this.main_menu.Padding = new System.Windows.Forms.Padding(9, 3, 0, 3);
            this.main_menu.Size = new System.Drawing.Size(1122, 35);
            this.main_menu.TabIndex = 31;
            // 
            // FileToolStripMenuItem
            // 
            this.FileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ResetAllToolStripMenuItem,
            this.toolStripMenuItem3,
            this.ExitToolStripMenuItem});
            this.FileToolStripMenuItem.Name = "FileToolStripMenuItem";
            this.FileToolStripMenuItem.Size = new System.Drawing.Size(61, 29);
            this.FileToolStripMenuItem.Text = "Tool";
            // 
            // ResetAllToolStripMenuItem
            // 
            this.ResetAllToolStripMenuItem.Name = "ResetAllToolStripMenuItem";
            this.ResetAllToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.ResetAllToolStripMenuItem.Text = "ResetAll";
            this.ResetAllToolStripMenuItem.Click += new System.EventHandler(this.ResetAllToolStripMenuItem_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(267, 6);
            // 
            // ExitToolStripMenuItem
            // 
            this.ExitToolStripMenuItem.Name = "ExitToolStripMenuItem";
            this.ExitToolStripMenuItem.Size = new System.Drawing.Size(270, 34);
            this.ExitToolStripMenuItem.Text = "Exit";
            this.ExitToolStripMenuItem.Click += new System.EventHandler(this.ExitToolStripMenuItem_Click);
            // 
            // OpenToolStripMenuItem
            // 
            this.OpenToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.AboutToolStripMenuItem1});
            this.OpenToolStripMenuItem.Name = "OpenToolStripMenuItem";
            this.OpenToolStripMenuItem.Size = new System.Drawing.Size(72, 29);
            this.OpenToolStripMenuItem.Text = "Open";
            // 
            // AboutToolStripMenuItem1
            // 
            this.AboutToolStripMenuItem1.Name = "AboutToolStripMenuItem1";
            this.AboutToolStripMenuItem1.Size = new System.Drawing.Size(270, 34);
            this.AboutToolStripMenuItem1.Text = "About";
            this.AboutToolStripMenuItem1.Click += new System.EventHandler(this.AboutToolStripMenuItem1_Click);
            // 
            // SelectAllTable
            // 
            this.SelectAllTable.AutoSize = true;
            this.SelectAllTable.ForeColor = System.Drawing.Color.Blue;
            this.SelectAllTable.Location = new System.Drawing.Point(32, 732);
            this.SelectAllTable.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.SelectAllTable.Name = "SelectAllTable";
            this.SelectAllTable.Size = new System.Drawing.Size(80, 24);
            this.SelectAllTable.TabIndex = 32;
            this.SelectAllTable.Text = "Select";
            this.SelectAllTable.UseVisualStyleBackColor = true;
            this.SelectAllTable.CheckedChanged += new System.EventHandler(this.checkBox1_CheckedChanged);
            // 
            // frm_main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1122, 783);
            this.Controls.Add(this.SelectAllTable);
            this.Controls.Add(this.main_menu);
            this.Controls.Add(this.btn_load_table);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.btn_gen_table);
            this.Controls.Add(this.btn_gen_code);
            this.Controls.Add(this.lblTable);
            this.Controls.Add(this.clb);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MainMenuStrip = this.main_menu;
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.MaximizeBox = false;
            this.Name = "frm_main";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "TableGenerate";
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
        private System.Windows.Forms.ContextMenuStrip cms_main;
        private System.Windows.Forms.ToolStripMenuItem ClearOutputToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem CancelDropDigDropToolStripMenuItem;
        private System.Windows.Forms.CheckBox SelectAllTable;
    }
}

