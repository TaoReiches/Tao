using System;
using System.Collections.Generic;
using System.Data;
using System.Text;
using System.IO;
using System.Xml;
using System.Threading;
using ExcelDataReader;

namespace table_gen
{
    public class MyNode
    {
        public readonly MyTable my_table;
        public DataSet def_data_set;
        public DataTable def_table;

        public DataSet content_data_set;
        public DataTable content_table;

        List<DataRow> def_primary_key_rows = new List<DataRow>();

        public string path;
        public string filename;
        public string name;
        public readonly string hpp_filename;
        public readonly string cpp_filename;
        //static public string struct_name;
        //static public string struct_ptr_name;
  //      string class_name;

//        bool has_primary_key;
 //       int field_count;
        bool gen_for_client;
        bool export_code;
        /// Name -> DataType
        List<KeyValuePair<string, string>> primary_keys = new List<KeyValuePair<string, string>>();
//        bool has_foreign_key;
        List<string> foreign_keys = new List<string>();
        //public Dictionary<string, MyScript> scripts = new Dictionary<string, MyScript>();
        List<UInt32> string_lens_ = new List<UInt32>();
        //List<string> index_keys = new List<string>();
//         bool has_key_name_mappings_;

        public MyNode(MyTable _my_table, string _name, string _path, bool _export_code)
        {
            my_table = _my_table;
            path = _path;
            filename = System.IO.Path.GetFileName(path);
            //name = System.IO.Path.GetFileNameWithoutExtension(path);
            name = _name;
            hpp_filename = name + "_table" + ".hpp";
            cpp_filename = name + "_table" + ".cpp";

            export_code = _export_code;
            this.Reset();
        }

        public override string ToString()
        {
            return name;
        }
        public void Reset()
        {
            def_data_set = null;
            def_table = null;

            content_data_set = null;
            content_table = null;

 //           has_primary_key = false;
//            primary_keys.Clear();
 //           has_foreign_key = false;
 //           field_count = 0;

            foreign_keys.Clear();
            //scripts.Clear();
            string_lens_.Clear();
            //index_keys.Clear();
   //         has_key_name_mappings_ = false;
        }

        void LoadContent(string conn)
        {
            using (var stream = new FileStream(conn, FileMode.Open, FileAccess.Read, FileShare.ReadWrite))
            {
                using (IExcelDataReader reader = ExcelReaderFactory.CreateReader(stream))
                {
                    DataSet content_ds = reader.AsDataSet(new ExcelDataSetConfiguration()
                    {
                        UseColumnDataType = false,
                        ConfigureDataTable = (tableReader) => new ExcelDataTableConfiguration()
                        {
                            UseHeaderRow = true
                        }
                    });
                    //foreach (DataColumn c in content_ds.Tables[0].Columns)
                    //{
                    //    c.ColumnMapping = MappingType.Attribute;
                    //}

                    content_data_set = content_ds;
                    content_table = content_ds.Tables[0];
                }
            }

            //using (OleDbDataAdapter content_da = new OleDbDataAdapter("SELECT * FROM [content$]", conn))
            //{
            //    using (DataSet content_ds = new DataSet("root"))
            //    {
            //        content_da.Fill(content_ds, "content");

            //        foreach (DataColumn c in content_ds.Tables[0].Columns)
            //            c.ColumnMapping = MappingType.Attribute;

            //        content_data_set = content_ds;
            //        content_table = content_ds.Tables[0];

            //        //bool remove_first = false;
            //        //CheckTableValid(ref remove_first);
          
            //        //if (remove_first)
            //        //    content_table.Rows.RemoveAt(0);
            //    }

            //    content_da.Dispose();
            //}
        }

        MyExcel kMyExcel = null;
//        static void LoadAsync(object obj)
//        {
//            MyNode node = obj as MyNode;

//            string path = node.path;

//            kMyExcel.ConvertExcel(path);

//            //string conn = "";

//            //if (path.EndsWith(".xls"))
//            //    conn = "Provider=Microsoft.Jet.OLEDB.4.0;" + "Data Source=" + path + ";" + "Extended Properties=Excel 8.0;";
//            //else if (path.EndsWith(".xlsx"))
//            //    conn = "Provider=Microsoft.ACE.OLEDB.12.0;" + "Data Source=" + path + ";" + "Extended Properties=\"Excel 12.0;HDR=YES;IMEX=1;\";";
//            //else
//            //    throw new Exception("未知的格式");

//            //node.Reset();
////            node.LoadDef(conn);
// //           node.LoadContent(conn);
//        }

        public void Load()
        {
            kMyExcel = new MyExcel();
            kMyExcel.ConvertExcel(path, name);

            Reset();
            LoadContent(path);

            GC.Collect();
        }

        void WriteContent(string xml_path)
        {
            string out_name = name + "_China" + ".xml";
            string out_path = xml_path + "Base/";
            string file = Path.Combine(out_path, out_name);
            if (!Directory.Exists(out_path))
                Directory.CreateDirectory(out_path);
            if (File.Exists(file))
                File.Delete(file);

            using (FileStream fs = new FileStream(file, FileMode.Create))
            {
                using (XmlTextWriter writer = new XmlTextWriter(fs, System.Text.Encoding.GetEncoding("utf-8")))
                {
                    writer.Formatting = Formatting.Indented;
                    writer.Indentation = 6;
                    writer.WriteStartDocument();

                    content_data_set.WriteXml(writer);
                    writer.Close();
                    fs.Close();
                }
            }

            WriteXmlContent();
        }
        MyXml kMyXml = new MyXml();
        void WriteXmlContent()
        {
            StringBuilder sbClient = new StringBuilder();
            string kClientXmlPath = "./Xml/Client/";
            string  kFileName = kClientXmlPath + name + ".xml"; 
            kMyXml.GenXml(name, sbClient, this, 1);

            if (!Directory.Exists(kClientXmlPath))
                Directory.CreateDirectory(kClientXmlPath);
            if (File.Exists(kFileName))
                File.Delete(kFileName);

            using (FileStream fs = new FileStream(kFileName, FileMode.Create))
            {
                using (StreamWriter sw = new StreamWriter(fs, System.Text.Encoding.GetEncoding("utf-8")))
                {
                    sw.Write(sbClient.ToString());
                    sw.Close();
                    fs.Close();
                }
            }

            StringBuilder sbServer = new StringBuilder();
            string kServerXmlPath = "./Xml/Server/";
            string kServerFileName = kServerXmlPath + name + ".xml";
            kMyXml.GenXml(name, sbServer, this, 2);

            if (!Directory.Exists(kServerXmlPath))
                Directory.CreateDirectory(kServerXmlPath);
            if (File.Exists(kServerFileName))
                File.Delete(kServerFileName);

            using (FileStream fs = new FileStream(kServerFileName, FileMode.Create))
            {
                using (StreamWriter sw = new StreamWriter(fs, System.Text.Encoding.GetEncoding("utf-8")))
                {
                    sw.Write(sbServer.ToString());
                    sw.Close();
                    fs.Close();
                }
            }
        }
        public void Convert(string def_path, string xml_path, string code_path)
        {
            WriteContent(xml_path);
        }

        public void GenHpp(string code_path)
        {
            StringBuilder sb = new StringBuilder();

            kGenCode.GenClass(this,sb);

            if (!System.IO.Directory.Exists(code_path))
                System.IO.Directory.CreateDirectory(code_path);

            string hpp_path = System.IO.Path.Combine(code_path, hpp_filename);

            if (System.IO.File.Exists(hpp_path))
                System.IO.File.Delete(hpp_path);

            using (FileStream fs = new FileStream(hpp_path, FileMode.Create))
            {
                using (StreamWriter sw = new StreamWriter(fs, System.Text.Encoding.GetEncoding("utf-8")))
                {
                    sw.Write(sb.ToString());
                    sw.Close();
                    fs.Close();
                }
            }
        }

        public void GenCpp(string code_path)
        {
            StringBuilder sb = new StringBuilder();

            sb.AppendLine(String.Format("#include \"{0}\"", hpp_filename));
            sb.AppendLine("#include \"tinyxml.h\"");
            sb.AppendLine("#include \"EeFileMemory.h\"");
            sb.AppendLine("#include \"EeFilePackage.h\"");

            sb.AppendLine("");

            kGenCode.GenCpp(sb);

            if (!System.IO.Directory.Exists(code_path))
                System.IO.Directory.CreateDirectory(code_path);
            string cpp_path = System.IO.Path.Combine(code_path, cpp_filename);

            if (System.IO.File.Exists(cpp_path))
                System.IO.File.Delete(cpp_path);

            using (FileStream fs = new FileStream(cpp_path, FileMode.Create))
            {
                using (StreamWriter sw = new StreamWriter(fs, System.Text.Encoding.GetEncoding("utf-8")))
                {
                    sw.Write(sb.ToString());
                    sw.Close();
                    fs.Close();
                }
            }
        }

        GenCode kGenCode = new GenCode();
        public void GenCode(string code_path, bool gen_for_client)
        {
            if (!export_code)
                return;

            this.gen_for_client = gen_for_client;
            GenHpp(code_path);
            GenCpp(code_path);
           
        }

        internal List<ManualResetEvent> wait_handles = new List<ManualResetEvent>();

        class Res
        {
            public MyScript script;
            public ManualResetEvent evt;

            public Res(MyScript _script, ManualResetEvent _evt)
            {
                script = _script;
                evt = _evt;
            }
        }

        static void Work(object obj)
        {
            MyNode node = obj as MyNode;
            DataTable def_table = node.def_table;

            node.wait_handles.Clear();
            bool use_thread_pool = false;

            if (use_thread_pool && node.wait_handles.Count != 0)
                WaitHandle.WaitAll(node.wait_handles.ToArray(), 1000 * 60 * 2, false);
        }
    }

    public class MyTable
    {
        static MyTable instance_;
        public static MyTable Instance
        {
            get
            {
                return instance_;
            }
        }

        public MyTable()
        {
            instance_ = this;
        }

        public void Init(string path)
        {
            bool h = ThreadPool.SetMinThreads(64, 64);
            bool g = ThreadPool.SetMaxThreads(64, 64);

            this.Clear();

            string[] files = System.IO.Directory.GetFiles(path, "*.xlsx", SearchOption.AllDirectories);

            foreach (string file in files)
            {
                string filename = System.IO.Path.GetFileName(file);

                if (filename.StartsWith("~$"))
                    continue;

                if (filename.ToLower().StartsWith("t_"))
                    continue;

                InitOne(path, file);
            }
        }
        
        public void Clear()
        {
            nodes_.Clear();
        }

        void InitOne(string path, string file)
        {
            try
            {
                string s = file.Substring(path.Length);
                string[] dirs = s.Split(new char[] {'\\'});
                string name = System.IO.Path.GetFileNameWithoutExtension(s);
                if (dirs.Length > 1)
                    name = System.IO.Path.Combine(dirs[0], name);

                nodes_.Add(name, new MyNode(this, name, file, dirs.Length == 1));
            }
            catch (System.Exception ex)
            {
                throw new Exception(ex.Message);
            }
        }

        public Dictionary<string, MyNode> Nodes
        {
            get
            {
                return nodes_;
            }
        }

        public MyNode GetNode(string name)
        {
            MyNode node = null;
            nodes_.TryGetValue(name, out node);
            return node;
        }

        Dictionary<string, MyNode> nodes_ = new Dictionary<string, MyNode>();
    }
}
