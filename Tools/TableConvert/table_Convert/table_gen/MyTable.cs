using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml;
using System.Data.OleDb;
using System.Data.Odbc;
using System.Reflection;
using System.Threading;

//using Microsoft.Office.Interop.Excel;

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
        //void LoadDef(string conn)
        //{
        //    using (OleDbDataAdapter def_da = new OleDbDataAdapter("SELECT * FROM [def$]", conn))
        //    {
        //        using (DataSet def_ds = new DataSet("def_root"))
        //        {
        //            def_da.Fill(def_ds, "def");

        //            foreach (DataColumn c in def_ds.Tables[0].Columns)
        //                c.ColumnMapping = MappingType.Attribute;

        //            def_data_set = def_ds;
        //            def_table = def_ds.Tables[0];
        //            def_table.CaseSensitive = true;
        //            for (int i = 0; i < def_table.Rows.Count; ++i)
        //                string_lens_.Add(0);

        //            /// check 字段名 数据类型
        //            {
        //                Dictionary<string, string> valid_types = new Dictionary<string, string>();
        //                valid_types.Add("INT8", "INT8");
        //                valid_types.Add("INT16", "INT16");
        //                valid_types.Add("INT32", "INT32");
        //                valid_types.Add("INT64", "INT64");
        //                valid_types.Add("UINT8", "UINT8");
        //                valid_types.Add("UINT16", "UINT16");
        //                valid_types.Add("UINT32", "UINT32");
        //                valid_types.Add("UINT64", "UINT64");
        //                valid_types.Add("float", "float");

        //                for (int i = 0; i < def_table.Rows.Count; ++i)
        //                {
        //                    DataRow row = def_table.Rows[i];
        //                    string field_name = row[0].ToString();

        //                    string key = row["主键"].ToString();
        //                    string client = row["客户端用"].ToString();
        //                    if (key == "1" && client != "1")
        //                    {
        //                        string se = string.Format("主键[{0}]必须导出给客户端使用", field_name);
        //                        throw new Exception(se);
        //                    }

        //                    /// 字段名非空检测
        //                    if (string.IsNullOrEmpty(field_name))
        //                    {
        //                        string s = string.Format("字段名不可为空, 行[{0}]", i);
        //                        throw new Exception(s);
        //                    }

        //                    /// 字段名首字母检测
        //                    char first_char = field_name[0];
        //                    if (first_char != '_' && !char.IsLetter(first_char))
        //                    {
        //                        string s = string.Format("字段名必须以下划线或者字母开始, 行[{0}], 字段名[{1}]",
        //                            i, field_name);
        //                        throw new Exception(s);
        //                    }

        //                    /// 字段名合法性检测
        //                    for (int j = 1; j < field_name.Length; ++j)
        //                    {
        //                        char c = field_name[j];
        //                        if (c != '_' && !char.IsLetterOrDigit(c))
        //                        {
        //                            string fmt = "字段名命名非法, 正确的字段名必须以下划线或者字母开始, " +
        //                                "随后以下划线、字母、数字组成, 行[{0}], 字段名[{1}].\n" +
        //                                "提示: 是否使用了空格或者其他符号.";
        //                            string s = string.Format(fmt, i, field_name);
        //                            throw new Exception(s);
        //                        }
        //                    }

        //                    /// 数据类型非空检测
        //                    string type_in_table = row["数据类型"].ToString();
        //                    if (string.IsNullOrEmpty(type_in_table))
        //                    {
        //                        string s = string.Format("数据类型不可为空, 字段名[{0}]", field_name);
        //                        throw new Exception(s);
        //                    }

        //                    /// 数据类型合法性检测
        //                    if (type_in_table.StartsWith("char"))
        //                    {
        //                        if ((type_in_table.Length < 7)
        //                            || (type_in_table[4] != '[')
        //                            || (type_in_table[type_in_table.Length - 1] != ']'))
        //                        {
        //                            string fmt = "字段名[{0}], 不支持该数据类型[{1}].\n" +
        //                                "目前支持的类型: \n" +
        //                                "INT8 INT16 INT32 INT64 UINT8 UINT16 UINT32 UINT64 char[1-256] float";
        //                            string s = string.Format(fmt, field_name, type_in_table);
        //                            throw new Exception(s);
        //                        }

        //                        string t = type_in_table.Substring(5, type_in_table.Length - 6);
        //                        UInt32 d = 0;
        //                        if (!uint.TryParse(t, out d))
        //                        {
        //                            string fmt = "字段名[{0}], 不支持该数据类型[{1}].\n" +
        //                                "目前支持的类型: \n" +
        //                                "INT8 INT16 INT32 INT64 UINT8 UINT16 UINT32 UINT64 char[1-256] float";
        //                            string s = string.Format(fmt, field_name, type_in_table);
        //                            throw new Exception(s);
        //                        }

        //                        const UInt32 MAX_CHAR_SIZE = 4096;
        //                        if (d <= 0 || d > MAX_CHAR_SIZE)
        //                        {
        //                            string fmt = "字段名[{0}], 不支持该数据类型[{1}].\n" +
        //                                "字符串有效长度范围[1 - {2}], 当前长度[{3}].\n" +
        //                                "目前支持的类型: \n" +
        //                                "INT8 INT16 INT32 INT64 UINT8 UINT16 UINT32 UINT64 char[1-256] float";

        //                            string s = string.Format(fmt,
        //                                field_name, type_in_table, MAX_CHAR_SIZE, d);
        //                            throw new Exception(s);
        //                        }

        //                        string_lens_[i] = d;
        //                    }
        //                    else if (!valid_types.ContainsKey(type_in_table))
        //                    {
        //                        string fmt = "字段名[{0}], 不支持该数据类型[{1}].\n" +
        //                            "目前支持的类型: \n" +
        //                            "INT8 INT16 INT32 INT64 UINT8 UINT16 UINT32 UINT64 char[1-256] float";
        //                        string s = string.Format(fmt, field_name, type_in_table);
        //                        throw new Exception(s);
        //                    }
        //                }
        //            }

        //            /// 缓存所有主键
        //            {
        //                DataRow[] rows = def_table.Select("主键=1");
        //                if (rows != null && rows.Length != 0)
        //                {
        //                    has_primary_key = true;
        //                    foreach (DataRow row in rows)
        //                    {
        //                        string type_in_table = row["数据类型"].ToString();
        //                        if (type_in_table.ToLower().StartsWith("char"))
        //                            type_in_table = "std::string";
        //                        primary_keys.Add(
        //                            new KeyValuePair<string, string>(row["字段名"].ToString(), type_in_table));
        //                    }
        //                }
        //            }

        //            /// 缓存所有外键
        //            {
        //                DataRow[] rows = def_table.Select("外键 is NOT NULL");
        //                if (rows != null && rows.Length != 0)
        //                {
        //                    has_foreign_key = true;
        //                    foreach (DataRow row in rows)
        //                        foreign_keys.Add(row["字段名"].ToString());
        //                }
        //            }

        //            if (!has_primary_key && !has_foreign_key)
        //                throw new Exception("不合法的表：既没有主键又没有外键!");

        //            ///// 缓存所有唯一索引
        //            //{
        //            //    DataRow[] rows = def_table.Select("索引=1");
        //            //    if (rows != null && rows.Length != 0)
        //            //    {
        //            //        foreach (DataRow row in rows)
        //            //            index_keys.Add(row["字段名"].ToString());
        //            //    }
        //            //}

        //            foreach (DataRow row in def_table.Rows)
        //            {
        //                string field_name = row["字段名"].ToString();
        //                if (field_name == "KeyName")
        //                {
        //                    has_key_name_mappings_ = true;
        //                    break;
        //                }
        //            }
        //        }
        //    }
        //}
        //void CheckTableValid(ref bool remove_first)
        //{
        //    /// 1: 检测所有字段名数量是否一致
        //    if (content_table.Columns.Count != def_table.Rows.Count)
        //    {
        //        string def_cols = "";
        //        foreach (DataRow row in def_table.Rows)
        //        {
        //            def_cols += row["字段名"] + " ";
        //        }
        //        string content_cols = "";
        //        foreach (DataColumn c in content_table.Columns)
        //        {
        //            content_cols += c.ColumnName + " ";
        //        }

        //        string field_name_in_def = "";
        //        string field_name_in_content = "";
        //        int min = Math.Min(def_table.Rows.Count, content_table.Columns.Count);
        //        for (int i = 0; i < min; ++i)
        //        {
        //            field_name_in_def = def_table.Rows[i]["字段名"].ToString();
        //            field_name_in_content = content_table.Columns[i].ColumnName;
        //            if (field_name_in_def != field_name_in_content)
        //                break;
        //        }

        //        string ext = "";
        //        if (field_name_in_def != field_name_in_content)
        //            ext = string.Format("首个不同字段 def[{0}], content[{1}]\n", field_name_in_def, field_name_in_content);
        //        string s = string.Format("严重错误! [{0}] 字段数量不同. def有[{1}]字段, content有[{2}]字段.\n{3}\n{4}\n\n{5}",
        //            filename, def_table.Rows.Count, content_table.Columns.Count, ext, def_cols, content_cols);
        //        throw new Exception(s);
        //    }

        //    /// 2: 检测所有字段名是否一一对应
        //    foreach (DataColumn c in content_table.Columns)
        //    {
        //        DataRow[] rows = def_table.Select("字段名='" + c.ColumnName + "'");
        //        if (rows == null || rows.Length == 0)
        //        {
        //            string s = string.Format("严重错误! [{0}]字段不匹配，[{1}]存在于content，而未在def中声明.",
        //                filename, c.ColumnName);
        //            throw new Exception(s);
        //        }
        //        else if (rows.Length > 1)
        //        {
        //            string s = string.Format("严重错误! [{0}] def表中存在重名[{1}]字段.",
        //                filename, c.ColumnName);
        //            throw new Exception(s);
        //        }
        //    }
                      
        //    /// 3: 检测所有字段出现顺序是否一致
        //    for (int i = 0; i < def_table.Rows.Count; ++i)
        //    {
        //        DataRow def_row = def_table.Rows[i];
        //        string def_field_name = def_row["字段名"].ToString();
        //        string content_field_name = content_table.Columns[i].ColumnName;

        //        if (def_field_name != content_field_name)
        //        {                   
        //            string s = string.Format("严重错误! def表与content表字段出现顺序不一致.\n首个不同字段 def[{0}], content[{1}].",
        //                def_field_name, content_field_name);
        //            throw new Exception(s);
        //        }
        //    }

        //    /// 4: 检测所有字段数据与数据类型是否匹配
        //    for (int i = 0; i < def_table.Rows.Count; ++i)
        //    {
        //        DataRow def_row = def_table.Rows[i];
        //        string field_name = def_row["字段名"].ToString();
        //        string type_in_table = def_row["数据类型"].ToString();

        //        for (int j = 0; j < content_table.Rows.Count; ++j)
        //        {
        //            DataRow row = content_table.Rows[j];
        //            string v = row[field_name].ToString();
                                        
        //            int first_string_len = -1;
        //            int max_string_len = -1;

        //            bool parse_ok = false;
        //            if (type_in_table == "INT8")
        //            {
        //                sbyte d = 0;
        //                parse_ok = sbyte.TryParse(v, out d);
        //            }
        //            else if (type_in_table == "INT16")
        //            {
        //                Int16 d = 0;
        //                parse_ok = Int16.TryParse(v, out d);
        //            }
        //            else if (type_in_table == "INT32")
        //            {
        //                Int32 d = 0;
        //                parse_ok = Int32.TryParse(v, out d);
        //            }
        //            else if (type_in_table == "INT64")
        //            {
        //                Int64 d = 0;
        //                parse_ok = Int64.TryParse(v, out d);
        //            }
        //            else if (type_in_table == "UINT8")
        //            {
        //                byte d = 0;
        //                parse_ok = byte.TryParse(v, out d);
        //            }
        //            else if (type_in_table == "UINT16")
        //            {
        //                UInt16 d = 0;
        //                parse_ok = UInt16.TryParse(v, out d);
        //            }
        //            else if (type_in_table == "UINT32")
        //            {
        //                UInt32 d = 0;
        //                parse_ok = UInt32.TryParse(v, out d);
        //            }
        //            else if (type_in_table == "UINT64")
        //            {
        //                UInt64 d = 0;
        //                parse_ok = UInt64.TryParse(v, out d);
        //            }
        //            else if (type_in_table == "float")
        //            {
        //                float d = 0;
        //                parse_ok = float.TryParse(v, out d);
        //            }
        //            else if (type_in_table.StartsWith("char"))
        //            {
        //                byte[] content = System.Text.Encoding.GetEncoding("gb2312").GetBytes(v);
        //                int len = content.Length;
        //                parse_ok = len < (Int32)string_lens_[i];

        //                if (j == 0)
        //                {
        //                    if (v == "EXCEL_STRING")
        //                        remove_first = true;

        //                    first_string_len = len;

        //                    if (string.IsNullOrEmpty(v) || (v == "0"))
        //                    {
        //                        string ext = "";
        //                        if (type_in_table.StartsWith("char"))
        //                            ext = string.Format("\n提示: 是否第一行数据未填或为0，短类型字符串类型值在Excel中第一行请填 EXCEL_STRING.");

        //                        string s = string.Format("行[{0}], 字段名[{1}], 值[{2}], 字符串识别异常[{3}].{4}",
        //                            j, field_name, v, type_in_table, ext);
        //                        throw new Exception(s);
        //                    }
        //                }

        //                if (len > max_string_len)
        //                    max_string_len = len;
        //            }

        //            if (max_string_len != -1)
        //            {
        //                if ((max_string_len >= 255) && (first_string_len <= 255))
        //                {
        //                    string s = string.Format(
        //                        "字段名[{1}], 该列字段存在长字符串(> 255字节),而第一行小于255字节,会导致Excel导出数据出错.",
        //                                           field_name);
        //                    throw new Exception(s);
        //                }
        //            }

        //            if (!parse_ok)
        //            {
        //                string ext = "";
        //                if (type_in_table.StartsWith("char"))
        //                    ext = string.Format("\n提示: 是否字符串长度溢出.");
                        
        //                string s = string.Format("行[{0}], 字段名[{1}], 值[{2}], 不匹配def中定义的数据类型[{3}].{4}",
        //                    j, field_name, v, type_in_table, ext);
        //                throw new Exception(s);
        //            }
        //        }
        //    }

        //}
        void LoadContent(string conn)
        {
            using (OleDbDataAdapter content_da = new OleDbDataAdapter("SELECT * FROM [content$]", conn))
            {
                using (DataSet content_ds = new DataSet("root"))
                {
                    content_da.Fill(content_ds, "content");

                    foreach (DataColumn c in content_ds.Tables[0].Columns)
                        c.ColumnMapping = MappingType.Attribute;

                    content_data_set = content_ds;
                    content_table = content_ds.Tables[0];

                    //bool remove_first = false;
                    //CheckTableValid(ref remove_first);
          
                    //if (remove_first)
                    //    content_table.Rows.RemoveAt(0);
                }

                content_da.Dispose();
            }
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
            //  通过批注处理字段和类型
            kMyExcel = new MyExcel();
            kMyExcel.ConvertExcel(path, name);

            string conn = "";

            if (path.EndsWith(".xls"))
                conn = "Provider=Microsoft.Jet.OLEDB.4.0;" + "Data Source=" + path + ";" + "Extended Properties=Excel 8.0;";
            else if (path.EndsWith(".xlsx"))
                conn = "Provider=Microsoft.ACE.OLEDB.12.0;" + "Data Source=" + path + ";" + "Extended Properties=\"Excel 12.0;HDR=YES;IMEX=1;\";";
            else
                throw new Exception("未知的格式");

            Reset();
            //LoadDef(conn);
            LoadContent(conn);

            GC.Collect();
        }

        //void WriteDef(string def_path)
        //{
        //    if (!export_code)
        //        return;

        //    string out_name = name + "_def.xml";
        //    def_data_set.WriteXml(System.IO.Path.Combine(def_path, out_name));
        //}
        void WriteContent(string xml_path)
        {
            string out_name = name + "_China" + ".xml";
            string out_path = xml_path + "Base/";
            string file = System.IO.Path.Combine(out_path, out_name);
            if (!System.IO.Directory.Exists(out_path))
                System.IO.Directory.CreateDirectory(out_path);
            if (System.IO.File.Exists(file))
                System.IO.File.Delete(file);

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
            //  写客户端
            StringBuilder sbClient = new StringBuilder();
            string kClientXmlPath = "./Xml/Client/";
            string  kFileName = kClientXmlPath + name + ".xml"; 
            kMyXml.GenXml(name, sbClient, this, 1);

            if (!System.IO.Directory.Exists(kClientXmlPath))
                System.IO.Directory.CreateDirectory(kClientXmlPath);
            if (System.IO.File.Exists(kFileName))
                System.IO.File.Delete(kFileName);

            using (FileStream fs = new FileStream(kFileName, FileMode.Create))
            {
                using (StreamWriter sw = new StreamWriter(fs, System.Text.Encoding.GetEncoding("utf-8")))
                {
                    sw.Write(sbClient.ToString());
                    sw.Close();
                    fs.Close();
                }
            }

            //  写服务器
            StringBuilder sbServer = new StringBuilder();
            string kServerXmlPath = "./Xml/Server/";
            string kServerFileName = kServerXmlPath + name + ".xml";
            kMyXml.GenXml(name, sbServer, this, 2);

            if (!System.IO.Directory.Exists(kServerXmlPath))
                System.IO.Directory.CreateDirectory(kServerXmlPath);
            if (System.IO.File.Exists(kServerFileName))
                System.IO.File.Delete(kServerFileName);

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
           // WriteDef(def_path);
            WriteContent(xml_path);
        }

        //void GenStruct(StringBuilder sb)
        //{
        //    struct_name = "";
        //    string[] name_segs = name.ToLower().Split('_');
        //    for (int i = 0; i < name_segs.Length; ++i)
        //    {
        //        string first_char = name_segs[i][0].ToString().ToUpper();
        //        string lower_name = name_segs[i].ToLower();
        //        struct_name += first_char + name_segs[i].Substring(1);
        //    }

        //    int client_field_count = 0;
        //    struct_ptr_name = struct_name + " *";
        //    sb.AppendLine("struct " + struct_name);
        //    sb.AppendLine("{");     /// struct begin
        //    foreach (DataRow row in def_table.Rows)
        //    {
        //        string client = row["客户端用"].ToString();
        //        bool is_client_use = client == "1";
        //        if (gen_for_client && !is_client_use)
        //            continue;
     
        //        string field = row["字段名"].ToString();
        //        string data_type = row["数据类型"].ToString();
        //        string comment = row["中文"].ToString();
        //        comment = comment.Replace('\n', ' ');

        //        int size = -1;
        //        int index = data_type.IndexOf('[');
        //        if (index != -1)
        //        {
        //            int last_index = data_type.IndexOf(']');
        //            size = int.Parse(data_type.Substring(index + 1, last_index - index - 1));
        //        }

        //        string real_type = (size == -1) ? (data_type) : (data_type.Substring(0, data_type.IndexOf('[')));
        //        string real_field = (size == -1) ? (field + ";") : (field + "[" + size.ToString() + "];");
        //        string s = string.Format("    {0,-12}{1,-24}     /// {2}", real_type, real_field, comment);
        //        sb.AppendLine(s);

        //        ++client_field_count;
        //    }
        //    if (gen_for_client)
        //        field_count = client_field_count;
        //    else
        //        field_count = def_table.Rows.Count;
        //    sb.AppendLine("};");    /// struct end
        //    sb.AppendLine("");
        //}
        //void GenTypedefs(StringBuilder sb)
        //{
        //    string data_typedef = string.Format("    typedef std::vector<{0} *>", struct_name);
        //    data_typedef = string.Format("{0, -52}Rows;", data_typedef);
        //    string data_ite_typedef = string.Format("{0, -52}RowsIte;", "    typedef Rows::iterator");
        //    sb.AppendLine(data_typedef);
        //    sb.AppendLine(data_ite_typedef);
        //    sb.AppendLine("");

        //    string row_typedef = string.Format("    typedef {0}", struct_name);
        //    row_typedef = string.Format("{0, -52}T{1};", row_typedef, primary_keys.Count);
        //    sb.AppendLine(row_typedef);
        //    int size = primary_keys.Count;
        //    for (int i = size - 1; i >= 0; --i)
        //    {
        //        string field_name = primary_keys[i].Key;
        //        string field_type = primary_keys[i].Value;

        //        string last_stage = string.Format("T{0}", i + 1);
        //        string stage = string.Format("T{0}", i);

        //        string t_typedef = string.Format("    typedef std::map<{0}, {1} *>", field_type, last_stage);
        //        t_typedef = string.Format("{0, -52}{1};", t_typedef, stage);

        //        string t_ite = string.Format("    typedef {0}::iterator", stage);
        //        t_ite = string.Format("{0, -52}{1}Ite;", t_ite, stage);

        //        sb.AppendLine(t_typedef);
        //        sb.AppendLine(t_ite);
        //    }
        //    sb.AppendLine("");

        //    string key_name_typedef = string.Format("    typedef std::map<std::string, {0} *>", struct_name);
        //    key_name_typedef = string.Format("{0, -52}KeyNameMapping;", key_name_typedef);
        //    string key_name_ite_typedef = string.Format("{0, -52}KeyNameMappingIte;", "    typedef KeyNameMapping::iterator");
        //    sb.AppendLine(key_name_typedef);
        //    sb.AppendLine(key_name_ite_typedef);
        //    sb.AppendLine("");
        //}
        //void GenDestructorFor(StringBuilder sb, int i)
        //{
        //    if (i == primary_keys.Count)
        //        return;

        //    string s = new string(' ', (i + 1) * 4);

        //    sb.Append(s);
        //    sb.AppendLine(string.Format("    for (T{0}Ite ite{1} = t{2}.begin(); ite{3} != t{4}.end(); ++ite{5})", i, i, i, i, i, i));

        //    sb.Append(s);
        //    sb.AppendLine("    {");

        //    sb.Append(s);
        //    sb.AppendLine(string.Format("        T{0} &t{1} = *((*ite{2}).second);", i + 1, i + 1, i));

        //    GenDestructorFor(sb, i + 1);

        //    sb.AppendLine("");

        //    sb.Append(s);
        //    sb.AppendLine(string.Format("        delete &t{0};", i + 1));

        //    sb.Append(s);
        //    sb.AppendLine("    }");
        //}
        //void GenReleaseFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("    void Release()");
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        T0 &t0 = mappings_;");
        //    sb.AppendLine("");
        //    GenDestructorFor(sb, 0);
        //    sb.AppendLine("");
        //    sb.AppendLine("        data_.clear();");
        //    sb.AppendLine("        mappings_.clear();");
        //    sb.AppendLine("        key_name_mappings_.clear();");
        //    sb.AppendLine("    }");
        //}
        //void GenDestructorFunction(StringBuilder sb)
        //{
        //    sb.AppendLine(string.Format("    ~{0}()", class_name));
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        this->Release();");
        //    sb.AppendLine("    }");
        //}
        //void GenLoadFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("    bool Load(const std::string &path)");
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        LOG_INF(__fmt(\"load %1% begin.\") % path);");
        //    sb.AppendLine("");

        //    sb.AppendLine("        TiXmlDocument doc;");
        //    sb.AppendLine("        if (!doc.LoadFile(path.c_str()))");
        //    sb.AppendLine("        {");
        //    sb.AppendLine("            std::string err = path + \"   \" + std::string(doc.ErrorDesc());");
        //    sb.AppendLine("            throw std::exception(err.c_str());");      
        //    sb.AppendLine("        }");
        //    sb.AppendLine("");

        //    sb.AppendLine("        TiXmlElement* root = doc.FirstChildElement(\"root\");");

        //    sb.AppendLine("        if (root == 0)");
        //    sb.AppendLine("        {");
        //    sb.AppendLine("            throw std::exception(\"root is null!\");");   
        //    sb.AppendLine("        }");
        //    sb.AppendLine("");

        //    sb.AppendLine("        TiXmlElement* element = root->FirstChildElement(\"content\");");
        //    sb.AppendLine("        while (element != 0)");
        //    sb.AppendLine("        {");

        //    sb.AppendLine(string.Format("            {0} * row = new {1}();", struct_name, struct_name));
        //    sb.AppendLine("            FillData(row, element);");
        //    sb.AppendLine("            FillMapping(row);");

        //    sb.AppendLine("            element = element->NextSiblingElement();");

        //    sb.AppendLine("        }");
        //    sb.AppendLine("");

        //    sb.AppendLine("        LOG_INF(__fmt(\"load %1% end.\") % path);");
        //    sb.AppendLine("");

        //    sb.AppendLine("        return true;");
        //    sb.AppendLine("    }");
        //}

        //void GenReLoadFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("    bool ReLoad(const std::string &path, bool is_release_old = false)");
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        LOG_INF(__fmt(\"reload %1% begin.\") % path);");
        //    sb.AppendLine("");
        //    sb.AppendLine("        if (is_release_old)");
        //    sb.AppendLine("            this->Release();");
        //    sb.AppendLine("");
        //    sb.AppendLine("        TiXmlDocument doc;");
        //    sb.AppendLine("        if (!doc.LoadFile(path.c_str()))");
        //    sb.AppendLine("        {");
        //    sb.AppendLine("            std::string err = path + \"   \" + std::string(doc.ErrorDesc());");
        //    sb.AppendLine("            throw std::exception(err.c_str());");
        //    sb.AppendLine("        }");
        //    sb.AppendLine("");

        //    sb.AppendLine("        TiXmlElement* root = doc.FirstChildElement(\"root\");");

        //    sb.AppendLine("        if (root == 0)");
        //    sb.AppendLine("            return false;");
        //    sb.AppendLine("");

        //    sb.AppendLine("        TiXmlElement* element = root->FirstChildElement(\"content\");");
        //    sb.AppendLine("        while (element != 0)");
        //    sb.AppendLine("        {");
        //    sb.AppendLine("            int int_value = 0;");

        //    for (int i = 0; i < primary_keys.Count; ++i)
        //    {
        //        string field_type = primary_keys[i].Value;
        //        if (field_type == "std::string")
        //        {
        //            sb.AppendLine("            const char* str_value = 0;");
        //            break;
        //        }
        //    }

        //    sb.AppendLine("");

        //    for (int i = 0; i < primary_keys.Count; ++i)
        //    {
        //        string field_name = primary_keys[i].Key;
        //        string field_type = primary_keys[i].Value;

        //        if (field_type == "std::string")
        //        {
        //            sb.AppendLine(string.Format("            str_value = element->Attribute(\"{0}\");", field_name));
        //            sb.AppendLine(string.Format("            const char* p{0} = str_value;", i));
        //        }
        //        else
        //        {
        //            sb.AppendLine(string.Format("            element->Attribute(\"{0}\", &int_value);", field_name));
        //            sb.AppendLine(string.Format("            {0} p{1} = ({2})int_value;", field_type, i, field_type));
        //        }
        //    }
        //    sb.AppendLine("");

        //    sb.AppendLine(string.Format(
        //        "            {0} *row = 0;",
        //        struct_name));

        //    string pp = "";
        //    for (int i = 0; i < primary_keys.Count; ++i)
        //    {
        //        string index = i.ToString();
        //        if (i == primary_keys.Count - 1)
        //        {
        //            pp += "p" + index;
        //        }
        //        else
        //        {
        //            pp += "p" + index + ", ";
        //        }
        //    }
        //    sb.AppendLine(string.Format("            row = Get({0});", pp));

        //    sb.AppendLine("            if (row)");
        //    sb.AppendLine("            {");
        //    sb.AppendLine("                FillData(row, element);");
        //    sb.AppendLine("            }");
        //    sb.AppendLine("            else");
        //    sb.AppendLine("            {");
        //    sb.AppendLine(string.Format(
        //        "                row = new {0}();",
        //        struct_name));
        //    sb.AppendLine("                FillData(row, element);");
        //    sb.AppendLine("                FillMapping(row);");
        //    sb.AppendLine("            }");

        //    sb.AppendLine("            element = element->NextSiblingElement();");

        //    sb.AppendLine("        }");
        //    sb.AppendLine("");
        //    sb.AppendLine("        LOG_INF(__fmt(\"reload %1% end.\") % path);");
        //    sb.AppendLine("");
        //    sb.AppendLine("        return true;");
        //    sb.AppendLine("    }");
        //}
            
        
    //    bool ReLoad(const std::string &path)
    //{
    //    LOG_INF(__fmt("reload %1% begin.") % path);

    //    TiXmlDocument doc;
    //    if (!doc.LoadFile(path.c_str()))
    //    {
    //        std::string err = path + "   " + std::string(doc.ErrorDesc());
    //        throw std::exception(err.c_str());
    //    }

    //    TiXmlElement* root = doc.FirstChildElement("root");
    //    if (root == 0)
    //        return false;

    //    TiXmlElement* element = root->FirstChildElement("content");
    //    while (element != 0)
    //    {
    //        int int_value = 0;
    //        const char* str_value = 0;

    //        element->Attribute("Level", &int_value);
    //        UINT32 p0 = (UINT32)int_value;
    //        element->Attribute("Job", &int_value);
    //        UINT8 p1 = (UINT8)int_value;

    //        str_value = element->Attribute("Name");

    //        Role *row = 0;
            
    //        row = Get(p0, p1);
    //        if (row)
    //        {
    //            FillData(row, element);
    //        }
    //        else
    //        {
    //            row = new Role();
    //            FillData(row, element);
    //            FillMapping(row);
    //        }
    //        element = element->NextSiblingElement();
    //    }

    //    LOG_INF(__fmt("reload %1% end.") % path);

    //    return true;
    //}

        //void GenFillDataFunction(StringBuilder sb)
        //{
        //    sb.AppendLine(string.Format(
        //        "    void FillData({0} *row, TiXmlElement* element)", 
        //        struct_name));
        //    sb.AppendLine("    {");

        //    sb.AppendLine("        int int_value = 0;");

        //    foreach (DataRow row in def_table.Rows)
        //    {
        //        string data_type = row["数据类型"].ToString();
        //        if (data_type.ToLower().StartsWith("char"))
        //        {
        //            sb.AppendLine("        const char* str_value = 0;");
        //            break;
        //        }
        //    }

        //    foreach (DataRow row in def_table.Rows)
        //    {
        //        string data_type = row["数据类型"].ToString();
        //        if (data_type.ToLower().StartsWith("float")
        //            || data_type.ToLower().StartsWith("int64")
        //            || data_type.ToLower().StartsWith("uint64"))
        //        {
        //            sb.AppendLine("        double double_value = 0.0f;");
        //            break;
        //        }
        //    }

        //    sb.AppendLine("");

        //    foreach (DataRow row in def_table.Rows)
        //    {
        //        string client = row["客户端用"].ToString();
        //        bool is_client_use = client == "1";
        //        if (gen_for_client && !is_client_use)
        //            continue;

        //        string field = row["字段名"].ToString();
        //        string data_type = row["数据类型"].ToString();
        //        string comment = row["中文"].ToString();

        //        if (data_type.StartsWith("char"))
        //        {
        //            sb.AppendLine(string.Format("        str_value = element->Attribute(\"{0}\");", field));
        //            sb.AppendLine(string.Format("        if (str_value)"));
        //            sb.AppendLine(string.Format("            StrSafeCopy(row->{0}, str_value);", field));
        //            sb.AppendLine(string.Format("        else"));
        //            sb.AppendLine(string.Format("            memset(row->{0}, 0, sizeof(row->{1}));", field, field));
        //        }
        //        else if (data_type.StartsWith("float") 
        //            || data_type.ToLower().StartsWith("int64") 
        //            || data_type.ToLower().StartsWith("uint64"))
        //        {
        //            sb.AppendLine(string.Format("        element->Attribute(\"{0}\", &double_value);", field));
        //            sb.AppendLine(string.Format("        row->{0} = ({1})double_value;", field, data_type));
        //        }
        //        else
        //        {
        //            sb.AppendLine(string.Format("        element->Attribute(\"{0}\", &int_value);", field));
        //            sb.AppendLine(string.Format("        row->{0} = ({1})int_value;", field, data_type));
        //        }
        //    }
                        
        //    sb.AppendLine("    }");
        //}
        //void GenFillMappingFunction(StringBuilder sb)
        //{
        //    sb.AppendLine(string.Format("    void FillMapping({0} *row)", struct_name));
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        data_.push_back(row);");
        //    sb.AppendLine("");

        //    sb.AppendLine("        T0 *t0 = &mappings_;");
        //    sb.AppendLine("");
        //    for (int i = 0; i < primary_keys.Count; ++i)
        //    {
        //        sb.AppendLine(string.Format("        if (t{0}->find(row->{1}) == t{2}->end())", i, primary_keys[i].Key, i));
        //        if (i != primary_keys.Count - 1)
        //            sb.AppendLine(string.Format("            (*t{0})[row->{1}] = new T{2}();", i, primary_keys[i].Key, i + 1));
        //        else
        //            sb.AppendLine(string.Format("            (*t{0})[row->{1}] = 0;", i, primary_keys[i].Key));
        //        sb.AppendLine(string.Format("        T{0} *&t{1} = (*t{2})[row->{3}];", i + 1, i + 1, i, primary_keys[i].Key));
        //        sb.AppendLine("");
        //    }
        //    sb.AppendLine(string.Format("        t{0} = row;", primary_keys.Count));
        //    if (has_key_name_mappings_)
        //    {
        //        sb.AppendLine("");
        //        sb.AppendLine("        key_name_mappings_[row->KeyName] = row;");
        //    }

        //    sb.AppendLine("    }");
        //}

        /// param_count[1, primary_keys.Count]
        //string GetPrimaryKey2ParamList(int param_count)
        //{
        //        string t = "";
        //        for (int i = 0; i < param_count; ++i)
        //        {
        //            string last = (i == param_count - 1) ? "" : ", ";
        //            t += string.Format("{0} {1}{2}", primary_keys[i].Value, primary_keys[i].Key, last);
        //        }
        //        return t;
      
        //}
        //void GenGetFunction(StringBuilder sb)
        //{
        //    for (int k = 1; k <= primary_keys.Count; ++k)
        //    {
        //        sb.AppendLine("    //////////////////////////////////////////////////////////////////////////");
        //        sb.AppendLine(string.Format("    T{0} *Get({1})", k, GetPrimaryKey2ParamList(k)));
        //        sb.AppendLine("    {");
        //        sb.AppendLine("        T0 *t0 = &mappings_;");
        //        sb.AppendLine("");
        //        for (int i = 0; i < k; ++i)
        //        {
        //            sb.AppendLine(string.Format("        if (t{0}->find({1}) == t{2}->end())", i, primary_keys[i].Key, i));
        //            sb.AppendLine(string.Format("            return 0;"));
        //            sb.AppendLine(string.Format("        T{0} *&t{1} = (*t{2})[{3}];", i + 1, i + 1, i, primary_keys[i].Key));
        //            sb.AppendLine("");
        //        }
        //        sb.AppendLine(string.Format("        return t{0};", k));
        //        sb.AppendLine("    }");
        //    }
        //}
        //void GenHasFunction(StringBuilder sb)
        //{
        //    for (int k = 1; k <= primary_keys.Count; ++k)
        //    {
        //        sb.AppendLine("    //////////////////////////////////////////////////////////////////////////");
        //        sb.AppendLine(string.Format("    bool Has({0})", GetPrimaryKey2ParamList(k)));
        //        sb.AppendLine("    {");
        //        sb.AppendLine("        T0 *t0 = &mappings_;");
        //        sb.AppendLine("");
        //        for (int i = 0; i < k; ++i)
        //        {
        //            sb.AppendLine(string.Format("        if (t{0}->find({1}) == t{2}->end())", i, primary_keys[i].Key, i));
        //            sb.AppendLine(string.Format("            return false;"));
        //            if (i != k - 1)
        //                sb.AppendLine(string.Format("        T{0} *&t{1} = (*t{2})[{3}];", i + 1, i + 1, i, primary_keys[i].Key));
        //            sb.AppendLine("");
        //        }
        //        sb.AppendLine(string.Format("        return true;"));
        //        sb.AppendLine("    }");
        //    }
        //}
        //void GenAtFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("    //////////////////////////////////////////////////////////////////////////");
        //    sb.AppendLine(string.Format("    {0} *At(int index)", struct_name));
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        return data_[index];");
        //    sb.AppendLine("    }");
        //}
        //void GenGetSizeFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("    //////////////////////////////////////////////////////////////////////////");
        //    sb.AppendLine("    int GetSize(void)");
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        return data_.size();");
        //    sb.AppendLine("    }");
        //}
        //void GenGetFieldCountFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("    //////////////////////////////////////////////////////////////////////////");
        //    sb.AppendLine("    int GetFieldCount(void)");
        //    sb.AppendLine("    {");
        //    sb.AppendLine(string.Format("        return {0};", field_count));
        //    sb.AppendLine("    }");
        //}
        //void GenGetDataFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("    //////////////////////////////////////////////////////////////////////////");
        //    sb.AppendLine("    Rows &GetData(void)");
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        return data_;");
        //    sb.AppendLine("    }");
        //}
        //void GenGetMappingsFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("    //////////////////////////////////////////////////////////////////////////");
        //    sb.AppendLine("    T0 &GetMappings(void)");
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        return mappings_;");
        //    sb.AppendLine("    }");
        //}
        //void GenKeyNameMappingsFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("    //////////////////////////////////////////////////////////////////////////");
        //    sb.AppendLine(string.Format("    {0} *GetByKeyName(const std::string &key_name)", struct_name));
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        KeyNameMappingIte ite = key_name_mappings_.find(key_name);");
        //    sb.AppendLine("        if (ite == key_name_mappings_.end())");
        //    sb.AppendLine("            return 0;");
        //    sb.AppendLine("");
        //    sb.AppendLine("        return (*ite).second;");
        //    sb.AppendLine("    }");
        //}

        //void GenClass(StringBuilder sb)
        //{
        //    sb.AppendLine("");
        //    sb.AppendLine("#pragma warning( push )");
        //    sb.AppendLine("#pragma warning( disable : 4996 )");
        //    sb.AppendLine("");

        //    class_name = struct_name + "Config";
        //    sb.AppendLine("class " + class_name);
        //    sb.AppendLine("{");     /// class begin
        //    sb.AppendLine(string.Format("    DECLARE_SINGLETON({0}Config);", struct_name));
        //    sb.AppendLine("public:");               /// public typedef
        //    this.GenTypedefs(sb);

        //    sb.AppendLine("public:");               /// public function
        //    /// gen function
        //    this.GenReleaseFunction(sb);
        //    this.GenDestructorFunction(sb);
        //    this.GenLoadFunction(sb);
        //    this.GenReLoadFunction(sb);
        //    this.GenFillDataFunction(sb);
        //    this.GenFillMappingFunction(sb);
        //    this.GenGetFunction(sb);
        //    this.GenHasFunction(sb);
        //    this.GenAtFunction(sb);
        //    this.GenGetSizeFunction(sb);
        //    this.GenGetFieldCountFunction(sb);
        //    this.GenGetDataFunction(sb);
        //    this.GenGetMappingsFunction(sb);
        //    this.GenKeyNameMappingsFunction(sb);
                
        //    sb.AppendLine("");

        //    sb.AppendLine("private:");              /// private data member
        //    sb.AppendLine("    Rows                 data_;");
        //    sb.AppendLine("    T0                   mappings_;");
        //    sb.AppendLine("    KeyNameMapping       key_name_mappings_;");

        //    sb.AppendLine("");
            
        //    sb.AppendLine("};");    /// class end
                       
        //    sb.AppendLine("");
        //    sb.AppendLine("#pragma warning( pop )");
        //    sb.AppendLine("");

        //    sb.AppendLine(string.Format("#define s{0}Config (*DATA::{1}::Instance())", struct_name, class_name));
        //}
        public void GenHpp(string code_path)
        {
            StringBuilder sb = new StringBuilder();

            kGenCode.GenClass(this,sb);

            //sb.AppendLine("/// Generate by table_gen tools, PLEASE DO NOT EDIT IT!");
            //sb.AppendLine("");
            //sb.AppendLine("#pragma once");
            //sb.AppendLine("");
            //sb.AppendLine("#include \"db_include.h\"");
            //sb.AppendLine("");

            //sb.AppendLine("namespace DATA");
            //sb.AppendLine("{");

            //this.GenStruct(sb);
            //this.GenClass(sb);

            //sb.AppendLine("}");     /// namespace end
            //sb.AppendLine("");

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

            //sb.AppendLine("/// Generate by table_gen tools, PLEASE DO NOT EDIT IT!");
            //sb.AppendLine("");
            //sb.AppendLine("#include \"stdafx.h\"");tinyxml.h
            sb.AppendLine(String.Format("#include \"{0}\"", hpp_filename));
            sb.AppendLine("#include \"tinyxml.h\"");
            sb.AppendLine("#include \"EeFileMemory.h\"");
            sb.AppendLine("#include \"EeFilePackage.h\"");

            sb.AppendLine("");
            //sb.AppendLine("namespace DATA");
            //sb.AppendLine("{");
            //sb.AppendLine(String.Format("    INSTANCE_SINGLETON({0});", class_name));
            //sb.AppendLine("}");
            //sb.AppendLine("");

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

        //void CheckForPrimary()
        //{
        //    if (!has_primary_key)
        //        return;

        //    /// 主键非空检测
        //    try
        //    {
        //        DataRow[] rows = def_table.Select("外键 is NOT NULL");
        //        for (int i = 0; i < primary_keys.Count; ++i)
        //        {
        //            string key = primary_keys[i].Key;
        //            string sql = string.Format("{0} is NULL", key);

        //            DataRow[] select_rows = content_table.Select(sql);
        //            if (select_rows.Length > 0)
        //            {
        //                string s = string.Format(
        //                    "主键 [{0}] 为空! 总行数 [{1}], 空行数 [{2}]",
        //                    key, content_table.Rows.Count, select_rows.Length);
        //                throw new Exception(s);
        //            }
        //        }
        //    }
        //    catch (System.Exception ex)
        //    {
        //        string s = string.Format(
        //            "主键非空检测未通过! \n{0}.",
        //            ex.Message);
        //        throw new Exception(s);
        //    }

        //    //foreach (DataRow row in content_table.Rows)
        //    //{
        //    //    string sql = "";
        //    //    for (int i = 0; i < primary_keys.Count; ++i)
        //    //    {
        //    //        string key = primary_keys[i].Key;
        //    //        string runtime_type = primary_keys[i].Value;
        //    //        string select_value = row[key].ToString();
        //    //        if (runtime_type == "std::string")
        //    //            select_value = string.Format("'{0}'", select_value);
        //    //        string last = (i == primary_keys.Count - 1) ? "" : " and ";
        //    //        sql += string.Format("{0}={1}{2}",
        //    //            key, select_value, last);
        //    //    }

        //    //    try
        //    //    {
        //    //        DataRow[] select_rows = content_table.Select(sql);
        //    //        if (select_rows.Length > 1)
        //    //        {
        //    //            string text = string.Format("主键不唯一! ");
        //    //            throw new Exception(text);
        //    //        }
        //    //    }
        //    //    catch (System.Exception ex)
        //    //    {
        //    //        string text = string.Format(
        //    //            "主键逐行验证出错! \n{0}. sql = [{1}]", ex.Message, sql);
        //    //        throw new Exception(text);
        //    //    }
        //    //}

        //    for (int i = 0; i < content_table.Rows.Count; ++i)
        //    {
        //        DataRow ri = content_table.Rows[i];
        //        for (int j = i + 1; j < content_table.Rows.Count; ++j)
        //        {
        //            DataRow rj = content_table.Rows[j];
        //            bool flag = true;
        //            string pk = "";
        //            for (int k = 0; k < primary_keys.Count; ++k)
        //            {
        //                string key = primary_keys[k].Key;
        //                if (ri[key].ToString() != rj[key].ToString())
        //                {
        //                    flag = false;
        //                    break;
        //                }

        //                pk += string.Format("{0}={1} ", key, ri[key].ToString());
        //            }
        //            if (flag)
        //            {
        //                string text = string.Format(
        //                    "{0}行主键不唯一.\n在{1}行发现重复项! 主键集 [{2}]", i, j, pk);
        //                throw new Exception(text);
        //            }
        //        }
        //    }
        //}
        //void CheckForForeign()
        //{
        //    /// test each foreign_key
        //    foreach (string foreign_key in foreign_keys)
        //    {
        //        DataRow[] foreign_row = def_table.Select("字段名='" + foreign_key + "'");
        //        if (foreign_row == null || foreign_row.Length == 0)
        //            continue;

        //        string foreign_value = foreign_row[0]["外键"].ToString();
        //        string[] t = foreign_value.Split('/');
        //        if (t.Length != 2)
        //        {
        //            string text = string.Format("无效的外键 [{0}]，正确的外键格式为(表名/字段名)!", 
        //                foreign_value);
        //            throw new Exception(text);
        //        }
        //        string node_name = t[0];
        //        string field = t[1];

        //        MyNode foreign_node = my_table.GetNode(node_name);
        //        if (foreign_node == null)
        //        {
        //            string fmt = "主键 [{0}] 上绑定的外键 [{1}] 无效, 外键对应的表不存在. \n" +
        //                "正确的外键格式为(表名/字段名)! \n" +
        //                "提示：是否外键中表名或者字段名大小写搞错了?";
        //            string s = string.Format(fmt, foreign_key, foreign_value);
        //            throw new Exception(s);
        //        }
                   
        //        string foreign_ingnore_value = foreign_row[0]["忽略外键"].ToString();
        //        string[] ingnore_values = foreign_ingnore_value.Split(',');

        //        /// test each row in this node
        //        foreach (DataRow row in content_table.Rows)
        //        {
        //            string value = row[foreign_key].ToString();
        //            /// check ignore foreign key
        //            bool find = false;
        //            foreach (string ingnore_value in ingnore_values)
        //            {
        //                if (ingnore_value == value)
        //                {
        //                    find = true;
        //                    break;
        //                }
        //            }
        //            /// find, ignore this value
        //            if (find)
        //                continue;

        //            /// now do real work
        //            bool is_string = foreign_row[0]["数据类型"].ToString().StartsWith("char");
        //            string v = is_string ? "'" + value + "'" : value;

        //            string sql = string.Format("{0}={1}", field, v);
        //            if (foreign_node.content_table != null)
        //            {
        //                DataRow[] select_rows = foreign_node.content_table.Select(sql);
        //                if (select_rows == null || select_rows.Length == 0)
        //                {
        //                    string text = string.Format("数据完整性检测失败，主键 [{0}] 对应的外键 [{1}] 对应数据不存在! sql [{2}].",
        //                        foreign_key, foreign_value, sql);
        //                    throw new Exception(text);
        //                }
        //            }
        //        }
        //    }
        //}

        //void ApplyScript()
        //{   
        //    Dictionary<string, DataTable> tables = new Dictionary<string, DataTable>();
        //    foreach (KeyValuePair<string, MyNode> p in MyTable.Instance.Nodes)
        //        tables.Add(p.Key, p.Value.content_table);

        //    foreach (DataColumn c in content_table.Columns)
        //    {
        //        MyScript script = null;
        //        //scripts.TryGetValue(c.ColumnName, out script);
        //        if (script == null)
        //            continue;

        //        for (int i = 0; i < content_table.Rows.Count; ++i)
        //        {
        //            string msg = "";
        //            try
        //            {
        //                DataRow row = content_table.Rows[i];
        //                string v = row[c.ColumnName].ToString();
        //                if (!script.Check(c.ColumnName, v, row, ref msg, content_table, tables, MyConfig.Instance.EnvirPath))
        //                {
        //                    string fmt = "行[{0}], 值[{1}].";
        //                    string s = string.Format(fmt, i, v);
        //                    throw new Exception(s);
        //                }
        //            }
        //            catch (System.Exception ex)
        //            {
        //                string fmt = "数据验证失败, 字段名[{0}], {1}\n\n脚本内的错误描述 = {2}.\n" +
        //                    "\n验证脚本:\n{3}" +
        //                    "\n\n脚本展开:\n{4}" +
        //                    "\n\n脚本验证异常调用堆栈:\n{4}";

        //                string s = string.Format(fmt,
        //                    c.ColumnName, ex.Message, msg,
        //                    script.Script, script.ScriptExpand, ex.StackTrace);
        //                throw new Exception(s);
        //            }
        //        }
        //    }
        //}

        internal List<ManualResetEvent> wait_handles = new List<ManualResetEvent>();

        //static void DoGen(Res res)
        //{
        //    MyScript script = res.script;
        //    ManualResetEvent evt = res.evt;

        //    try
        //    {
        //        script.Build();
        //    }
        //    catch (System.Exception ex)
        //    {
        //        string fmt = "数据验证脚本错误, 未能生成验证脚本, 字段名 = [{0}].\n{1}." +
        //            "\n验证脚本:\n{2}\n\n" +
        //            "脚本展开:\n{3}\n\n" +
        //            "提示: 注意查看脚本和脚本展开是否正确.";
        //        string s = string.Format(fmt, script.FieldName, ex.Message, script.Script, script.ScriptExpand);
        //        throw new Exception(s);
        //    }
        //    finally
        //    {
        //        evt.Set();
        //    }
        //}
        //static void GenFunc(object obj)
        //{
        //    Res res = obj as Res;
        //    try
        //    {
        //        DoGen(res);
        //    }
        //    catch (System.Exception ex)
        //    {
        //        System.Windows.Forms.MessageBox.Show(ex.Message);
        //    }
        //}
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
 //           node.scripts.Clear();
//            DataRow[] rows = def_table.Select("数据验证 is NOT NULL");
            bool use_thread_pool = false;
//            if (rows != null && rows.Length != 0)
//            {
//                if (rows.Length <= 64)
//                    use_thread_pool = true;

//                if (use_thread_pool)
//                {
//                    for (int i = 0; i < rows.Length; ++i)
//                        node.wait_handles.Add(new ManualResetEvent(false));
//                }

//                for (int i = 0; i < rows.Length; ++i)
//                {
//                    DataRow row = rows[i];
//                    string field_name = row["字段名"].ToString();
//                    MyScript script = new MyScript(field_name, row["数据验证"].ToString(), def_table);

//                    try
//                    {
//                        if (use_thread_pool)
//                        {
//                            ThreadPool.QueueUserWorkItem(new WaitCallback(GenFunc),
//                                     new Res(script, node.wait_handles[i]));
//                        }
//                        else
//                        {
//                            script.Build();
//                        }
//                    }
//                    catch (System.Exception ex)
//                    {
//                        string fmt = "数据验证脚本错误, 未能生成验证脚本, 字段名 = [{0}].\n{1}." +
//                            "\n验证脚本:\n{2}\n\n" +
//                            "脚本展开:\n{3}\n\n" +
//                            "提示: 注意查看脚本和脚本展开是否正确.";
//                        string s = string.Format(fmt, script.FieldName, ex.Message, script.Script, script.ScriptExpand);
//                        throw new Exception(s);
//                    }

////                    node.scripts.Add(row["字段名"].ToString(), script);
//                }
//            }

            if (use_thread_pool && node.wait_handles.Count != 0)
                WaitHandle.WaitAll(node.wait_handles.ToArray(), 1000 * 60 * 2, false);
        }

        //void CheckForData()
        //{ 
        //    /// 构建数据验证脚本
        //    Thread thread = new Thread(Work);
        //    thread.Start(this);
        //    thread.Join();

        //    /// 使用脚本验证
        //    /// 构造content集合
        //    this.ApplyScript();
        //}
        //public void Check(bool use_primary, bool use_foreign, bool use_data)
        //{
        //    if (use_primary)
        //        CheckForPrimary();

        //    if (use_foreign)
        //        CheckForForeign();

        //    if (use_data)
        //        CheckForData();
        //}
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

  //          MessageBox.Show(string.Format("Find path {0}", path));

            string[] files = System.IO.Directory.GetFiles(path, "*.xlsx", SearchOption.AllDirectories);

   //         MessageBox.Show(string.Format("Find file count {0}", files.Length.ToString()));

            foreach (string file in files)
            {
                string filename = System.IO.Path.GetFileName(file);

                /// 忽略零时文件
                if (filename.StartsWith("~$"))
                    continue;

                /// 忽略零时文件
                if (filename.ToLower().StartsWith("t_"))
                    continue;

       //         MessageBox.Show(string.Format("Find File {0}", filename));
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
