using System;
using System.Drawing;
using System.Windows.Forms;

namespace table_gen
{
    public partial class frm_main : Form
    {
        public frm_main()
        {
            InitializeComponent();
        }

        MyConfig config = new MyConfig();
        MyTable my_table = new MyTable();

        void InitAllFile(string path)
        {
            clb.Items.Clear();
            my_table.Init(path);

            foreach (MyNode node in my_table.Nodes.Values)
            {
                _ = clb.Items.Add(node);
            }
        }

        void LoadNodes()
        {
            btn_gen_table.Enabled = false;
            btn_gen_code.Enabled = false;

            int total_files = 0;
            double total_seconds = 0;
            PrintText("Load Excel Begin", Color.Red, true);
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                if (clb.GetItemChecked(i))
                {
                    ++total_files;

                    DateTime t = DateTime.Now;
                    MyNode node = clb.Items[i] as MyNode;

                    string begin = string.Format("  {0, -32}",
                        string.Format("Load excel [{0}]", node.name));

                    var color = Color.Black;

                    PrintText(begin, color, false);
                    node.Load();

                    double seconds = (DateTime.Now - t).TotalSeconds;
                    total_seconds += seconds;
                    string end = string.Format("Used         {0:N2} Seconds.", seconds);
                    PrintText(end, color, true);
                }
            }

            string done_text = string.Format("Load Finished, Total Files {0} , Total Time {1:N2} .\n",
                total_files, total_seconds);
            PrintText(done_text, Color.Red, true);

            btn_gen_table.Enabled = true;
            btn_gen_code.Enabled = true;
        }
        private void LoadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                LoadNodes();
            }
            catch (Exception ex)
            {
                PrintText(ex.Message, Color.Red, true);
                MessageBox.Show(ex.Message);
            }
        }

        private void frm_main_Load(object sender, EventArgs e)
        {
            ResetApp();
        }

        void SelectAll()
        {
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                clb.SetItemChecked(i, true);
            }
        }

        private void SelectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SelectAll();
        }

        private void SelectNullToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                clb.SetItemChecked(i, false);
            }
        }

        private void FilpSelectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                clb.SetItemChecked(i, !clb.GetItemChecked(i));
            }
        }
        void PrintText(string text)
        {
            PrintText(text, Color.Black);
        }
        void PrintText(string text, Color c)
        {
            PrintText(text, c, false);
        }
        void PrintText(string text, System.Drawing.Color c, bool new_line)
        {
            System.Drawing.Color old_color = out_filter_txt.SelectionColor;
            out_filter_txt.SelectionColor = c;
            out_filter_txt.AppendText(text);
            if (new_line)
                out_filter_txt.AppendText("\n");
            out_filter_txt.SelectionColor = old_color;

            Application.DoEvents();
        }

        void DoConvert()
        {
            int total_files = 0;
            double total_seconds = 0;
            PrintText("Convert begin", Color.Green, true);
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                if (!clb.GetItemChecked(i))
                    continue;

                MyNode node = clb.Items[i] as MyNode;

                ++total_files;
                string begin = string.Format("  {0, -24}",
                    string.Format("Start [{0}]", node.name));

                PrintText(begin, Color.Black, false);

                DateTime t = DateTime.Now;
      
                node.Convert(config.DefPath, config.XMLPath, config.CodePath);

                double seconds = (DateTime.Now - t).TotalSeconds;
                total_seconds += seconds;

                string end = string.Format("              Used {0:N2} Seconds.", seconds);
                PrintText(end, Color.Black, true);
            }
            string done_text = string.Format("Covent Finished, Total Files {0} , Total Time {1:N2} .\n",
                total_files, total_seconds);
            PrintText(done_text, Color.Green, true);
        }

        void DoGenCode()
        {
            int total_files = 0;
            double total_seconds = 0;
            PrintText("Covent Code Begin", Color.Blue, true);
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                if (!clb.GetItemChecked(i))
                    continue;

                MyNode node = clb.Items[i] as MyNode;

                ++total_files;
                string begin = string.Format("  {0, -24}",
                    string.Format("Begin [{0}]", node.name));
                
                PrintText(begin, Color.Black, false);

                DateTime t = DateTime.Now;
                PrintText("Begin ", Color.Black, false);
                node.GenCode(config.ClientCodePath, true);
                PrintText("End ", Color.Black, false);
                double seconds = (DateTime.Now - t).TotalSeconds;
                total_seconds += seconds;

                string end = string.Format("      Time {0:N2} Ãë.", seconds);
                PrintText(end, Color.Black, true);
            }
            string done_text = string.Format("Convert Code Finished, Total Files {0} , Total Time {1:N2} .\n",
                total_files, total_seconds);
            PrintText(done_text, Color.Blue, true);
        }

        private void btn_gen_table_Click(object sender, EventArgs e)
        {
            try
            {
                DoConvert();
            }
            catch (Exception ex)
            {
                string s = string.Format("\n{0}\n\n Error:\n{1}", ex.Message, ex.StackTrace);
                PrintText(s, Color.Red, true);
                MessageBox.Show(s, "Convert Table Failed");
            }
        }

        private void btn_gen_code_Click(object sender, EventArgs e)
        {
            try
            {
                DoGenCode();
            }
            catch (System.Exception ex)
            {
                string s = string.Format("\n{0}\n\nError:\n{1}", ex.Message, ex.StackTrace);
                PrintText(s, Color.Red, true);
                MessageBox.Show(s, "Convert Code Failed");
            }
        }

        void ResetApp()
        {
            config.Load();
            InitAllFile(config.XLSPath);

            out_filter_txt.Clear();

            btn_gen_table.Enabled = false;
            btn_gen_code.Enabled = false;
        }

        private void ResetToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ResetApp();
        }

        private void btn_load_table_Click(object sender, EventArgs e)
        {
            try
            {
                LoadNodes();
            }
            catch (Exception ex)
            {
                string s = string.Format("\n{0}\n\nError:\n{1}", ex.Message, ex.StackTrace);
                PrintText(s, Color.Red, true);
                MessageBox.Show(s, "Load Excel Error");
            }
        }

        private void ResetAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ResetApp();
        }

        private void ExitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void AboutToolStripMenuItem1_Click(object sender, EventArgs e)
        {
            string s = "Tao Wang";
            MessageBox.Show(s, "OK");
        }

        private void clb_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (e.CurrentValue == CheckState.Indeterminate)
                e.NewValue = CheckState.Indeterminate;
        }

        void OpenDir(string dir)
        {
            try
            {
                if (!System.IO.Directory.Exists(dir))
                    return;

                System.Diagnostics.Process.Start("explorer.exe", dir);
            }
            catch (Exception ex)
            {
                PrintText(ex.Message, Color.Red, true);
                MessageBox.Show(ex.Message);
            }
        }
        private void OpenCurrentToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenDir(Application.StartupPath);
        }

        private void OpenDataSourceToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenDir(System.IO.Path.Combine(Application.StartupPath, config.XLSPath));
        }

        private void OpenDataGenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenDir(System.IO.Path.Combine(Application.StartupPath, config.XMLPath));
        }

        private void OpenCodeGenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenDir(System.IO.Path.Combine(Application.StartupPath, config.CodePath));
        }

        private void OpenClientCodeGenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenDir(System.IO.Path.Combine(Application.StartupPath, config.ClientCodePath));
        }

        private void ClearOutputToolStripMenuItem_Click(object sender, EventArgs e)
        {
            out_filter_txt.Clear();
        }

        private void CancelDropDigDropToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                string v = clb.Items[i].ToString();
                if (v.StartsWith("drop") || v.StartsWith("dig"))
                    clb.SetItemChecked(i, false);
            }
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (SelectAllTable.Checked)
            {
                for (int i = 0; i < clb.Items.Count; ++i)
                {
                    clb.SetItemChecked(i, true);
                }
            }
            else
            {
                for (int i = 0; i < clb.Items.Count; ++i)
                {
                    clb.SetItemChecked(i, false);
                }
            }
            
        }

        private void cb_data_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
