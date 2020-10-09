using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Runtime.InteropServices;
using System.Threading;

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
                int index = clb.Items.Add(node);
                
            //    if (config.IsBaseFile(node.filename))
             //       clb.SetItemCheckState(index, CheckState.Indeterminate);
            }

            //SelectAll();
        }
        void LoadNodes()
        {
            btn_gen_table.Enabled = false;
            btn_gen_code.Enabled = false;

            for (int i = 0; i < clb.Items.Count; ++i)
            {
                if (!clb.GetItemChecked(i))
                {
               //     if (config.UseTip)
               //         System.Windows.Forms.MessageBox.Show("δѡ��ȫ�������ݱ����������������������Լ�顣", "ע��");
                    cb_primary.Checked = false;
                    cb_primary.Enabled = false;
                    cb_forign.Checked = false;
                    cb_forign.Enabled = false;

                    break;
                }
            }

            int total_files = 0;
            double total_seconds = 0;
            PrintText("�������ݱ������ĵȴ� ^_^", System.Drawing.Color.Red, true);
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                if (clb.GetItemChecked(i))
                {
                    ++total_files;

                    DateTime t = DateTime.Now;
                    MyNode node = clb.Items[i] as MyNode;

                    string begin = string.Format("  {0, -32}",
                        string.Format("�� [{0}]", node.name));

                    System.Drawing.Color color = System.Drawing.Color.Black;
              //      if (config.IsBaseFile(node.filename))
              //          color = System.Drawing.Color.Gray;

                    PrintText(begin, color, false);
                    node.Load();

                    double seconds = ((System.TimeSpan)(DateTime.Now - t)).TotalSeconds;
                    total_seconds += seconds;
                    string end = string.Format("��        ��ʱ {0:N2} ��.", seconds);
                    PrintText(end, color, true);
                }
            }

            string done_text = string.Format("ȫ���������, �� {0} ���, ��ʱ {1:N2} ��.\n",
                total_files, total_seconds);
            PrintText(done_text, System.Drawing.Color.Red, true);

            btn_gen_table.Enabled = true;
            btn_gen_code.Enabled = true;
        }
        private void LoadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                LoadNodes();
            }
            catch (System.Exception ex)
            {
                PrintText(ex.Message, System.Drawing.Color.Red, true);
                System.Windows.Forms.MessageBox.Show(ex.Message);
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
            PrintText(text, System.Drawing.Color.Black);
        }
        void PrintText(string text, System.Drawing.Color c)
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
            PrintText("��ʼ�������ݱ��ΪXML��ʽ", System.Drawing.Color.Green, true);
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                if (!clb.GetItemChecked(i))
                    continue;

                MyNode node = clb.Items[i] as MyNode;
                //if (config.IsBaseFile(node.filename))
                //    continue;

                ++total_files;
                string begin = string.Format("  {0, -24}",
                    string.Format("�� [{0}]", node.name));

                PrintText(begin, System.Drawing.Color.Black, false);

                DateTime t = DateTime.Now;
      
                node.Convert(config.DefPath, config.XMLPath, config.CodePath);

                double seconds = ((System.TimeSpan)(DateTime.Now - t)).TotalSeconds;
                total_seconds += seconds;

                string end = string.Format("      ��        ��ʱ {0:N2} ��.", seconds);
                PrintText(end, System.Drawing.Color.Black, true);
            }
            string done_text = string.Format("ȫ�������ɹ�, �� {0} �ļ�, ��ʱ {1:N2} ��.\n",
                total_files, total_seconds);
            PrintText(done_text, System.Drawing.Color.Green, true);
        }

        void DoGenCode()
        {
            int total_files = 0;
            double total_seconds = 0;
            PrintText("��ʼ���ɴ���", System.Drawing.Color.Blue, true);
            for (int i = 0; i < clb.Items.Count; ++i)
            {
                if (!clb.GetItemChecked(i))
                    continue;

                MyNode node = clb.Items[i] as MyNode;
                //if (config.IsBaseFile(node.filename))
                //    continue;

                ++total_files;
                string begin = string.Format("  {0, -24}",
                    string.Format("�� [{0}]", node.name));
                
                PrintText(begin, System.Drawing.Color.Black, false);

                DateTime t = DateTime.Now;
                //PrintText("������ ", System.Drawing.Color.Black, false);
                //node.GenCode(config.CodePath, false);
                //PrintText("�� ", System.Drawing.Color.Black, false);
                PrintText("�ͻ��� ", System.Drawing.Color.Black, false);
                node.GenCode(config.ClientCodePath, true);
                PrintText("�� ", System.Drawing.Color.Black, false);
                double seconds = ((System.TimeSpan)(DateTime.Now - t)).TotalSeconds;
                total_seconds += seconds;

                string end = string.Format("      ��ʱ {0:N2} ��.", seconds);
                PrintText(end, System.Drawing.Color.Black, true);
            }
            string done_text = string.Format("ȫ���������ɳɹ�, �� {0} �ļ�, ��ʱ {1:N2} ��.\n",
                total_files, total_seconds);
            PrintText(done_text, System.Drawing.Color.Blue, true);
        }

        private void btn_gen_table_Click(object sender, EventArgs e)
        {
            //try
            //{
            //    DoCheck();
            //}
            //catch (System.Exception ex)
            //{
            //    string s = string.Format("\n{0}\n\n���ö�ջ:\n{1}", ex.Message, ex.StackTrace);
            //    PrintText(s, System.Drawing.Color.Red, true);
            //    System.Windows.Forms.MessageBox.Show(s, "����XML����");
            //    return;
            //}

            try
            {
                DoConvert();
            }
            catch (System.Exception ex)
            {
                string s = string.Format("\n{0}\n\n���ö�ջ:\n{1}", ex.Message, ex.StackTrace);
                PrintText(s, System.Drawing.Color.Red, true);
                System.Windows.Forms.MessageBox.Show(s, "����XML����");
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
                string s = string.Format("\n{0}\n\n���ö�ջ:\n{1}", ex.Message, ex.StackTrace);
                PrintText(s, System.Drawing.Color.Red, true);
                System.Windows.Forms.MessageBox.Show(s, "���ɴ������");
            }
        }

        void ResetApp()
        {
            config.Load();
            InitAllFile(config.XLSPath);

            cb_primary.Checked = true;
            cb_primary.Enabled = true;
            cb_forign.Checked = true;
            cb_forign.Enabled = true;
            this.out_filter_txt.Clear();

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
            catch (System.Exception ex)
            {
                string s = string.Format("\n{0}\n\n���ö�ջ:\n{1}", ex.Message, ex.StackTrace);
                PrintText(s, System.Drawing.Color.Red, true);
                System.Windows.Forms.MessageBox.Show(s, "���ش���");
            }
        }

        private void LoadTableToolStripMenuItem_Click(object sender, EventArgs e)
        {
            try
            {
                LoadNodes();
            }
            catch (System.Exception ex)
            {
                string s = string.Format("\n{0}\n\n���ö�ջ:\n{1}", ex.Message, ex.StackTrace);
                PrintText(s, System.Drawing.Color.Red, true);
                System.Windows.Forms.MessageBox.Show(s, "���ش���");
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
            string s = "���ת������v1.0\n�߻�xlsx to xml\n������������Ч�Լ��\n�����������\n 2011-05-23";
            System.Windows.Forms.MessageBox.Show(s, "����");
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
            catch (System.Exception ex)
            {
                PrintText(ex.Message, System.Drawing.Color.Red, true);
                System.Windows.Forms.MessageBox.Show(ex.Message);
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
