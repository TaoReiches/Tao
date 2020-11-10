using System;
using System.Text;
using System.Data;
using System.Text.RegularExpressions;
using System.Collections.Generic;

namespace table_gen
{
    class MyScript
    {
        string field_name_;
        string script_;
        string script_expand_;
        string code_;
        DataTable def_table_;

        public string FieldName
        {
            get
            {
                return field_name_;
            }
        } 
        public string Script
        {
            get
            {
                return script_;
            }
        }
        public string ScriptExpand
        {
            get
            {
                return script_expand_;
            }
        }
        
        public string Code
        {
            get
            {
                return code_;
            }
        }

        void GenUtilFunction(StringBuilder sb, string script)
        {
            /// range
            sb.Append("    bool range(int l, int r)");
            sb.Append("    {");
            sb.Append("        return d >= l && d <= r;");
            sb.Append("    }");

            /// list
            sb.Append("    bool list(params int[] values)");
            sb.Append("    {");
            sb.Append("        for (int i = 0; i < values.Length; ++i)");
            sb.Append("        {");
            sb.Append("            if (d == i)");
            sb.Append("                return true;");
            sb.Append("        }");
            sb.Append("        return false;");
            sb.Append("    }");

            /// tos
            sb.Append("    string tos(object obj)");
            sb.Append("    {");
            sb.Append("        return obj.ToString();");
            sb.Append("    }");

            /// toi
            sb.Append("    int toi(string s)");
            sb.Append("    {");
            sb.Append("        int data = 0;");
            sb.Append("        int.TryParse(s, out data);");
            sb.Append("        return data;");
            sb.Append("    }");

            /// tof
            sb.Append("    float tof(string s)");
            sb.Append("    {");
            sb.Append("        float data = 0;");
            sb.Append("        float.TryParse(s, out data);");
            sb.Append("        return data;");
            sb.Append("    }");

            /// gt(field_name, sql)
            sb.Append("    string gt(string field_name, string sql)");
            sb.Append("    {");
            sb.Append("        DataRow[] rows = table_.Select(sql);");
            sb.Append("        if (rows == null || rows.Length == 0)");
            sb.Append("            return \"\";");
            sb.Append("        return rows[0][field_name].ToString();");
            sb.Append("    }");

            /// gt(table_name, field_name, sql)
            sb.Append("    string gt(string table_name, string field_name, string sql)");
            sb.Append("    {");
            sb.Append("        DataTable table = null;");
            sb.Append("        if (!tables_.TryGetValue(table_name, out table))");
            sb.Append("            return \"\";");
            sb.Append("        DataRow[] rows = table.Select(sql);");
            sb.Append("        if (rows == null || rows.Length == 0)");
            sb.Append("            return \"\";");
            sb.Append("        return rows[0][field_name].ToString();");
            sb.Append("    }");

            /// query(table_name, sql)
            sb.Append("    DataRow[] query(string table_name, string sql)");
            sb.Append("    {");
            sb.Append("        DataTable table = null;");
            sb.Append("        if (!tables_.TryGetValue(table_name, out table))");
            sb.Append("            return null;");
            sb.Append("        DataRow[] rows = table.Select(sql);");
            sb.Append("        return rows;");
            sb.Append("    }");

            /// DataRow[] query(string sql)
            sb.Append("    DataRow[] query(string sql)");
            sb.Append("    {");
            sb.Append("        DataRow[] rows = table_.Select(sql);");
            sb.Append("        return rows;");
            sb.Append("    }");

            /// int get_query_count(DataRow[] rows)
            sb.Append("    int get_query_count(DataRow[] rows)");
            sb.Append("    {");
            sb.Append("        if (rows == null)");
            sb.Append("            return 0;");
            sb.Append("        return rows.Length;");
            sb.Append("    }");

            /// string _SQL(object obj)
            sb.Append("    string _SQL(object obj)");
            sb.Append("    {");
            sb.Append("        if (obj is string)");
            sb.Append("            return string.Format(\"'{0}'\", obj.ToString());");
            sb.Append("        return string.Format(\"{0}\", obj.ToString());");
            sb.Append("    }");

            /// bool unique()
            sb.Append("    bool unique()");
            sb.Append("    {");
            sb.Append("        DataRow[] rows = query(field + \"=\" + _SQL(v));");
            sb.Append("        return get_query_count(rows) == 1;");
            sb.Append("    }");

            /// bool has_file(string filename)
            sb.Append("    bool has_file(string filename)");
            sb.Append("    {");
            sb.Append("        return System.IO.File.Exists(filename);");
            sb.Append("    }");

            /// string path_combine(string p0, string p1)
            sb.Append("    string path_combine(string p0, string p1)");
            sb.Append("    {");
            sb.Append("        return System.IO.Path.Combine(p0, p1);");
            sb.Append("    }");

            /// string get_map_path()
            sb.Append("    string get_map_path()");
            sb.Append("    {");
            sb.Append("        return System.IO.Path.Combine(envir_path_, \"map/\");");
            sb.Append("    }");
        }
        
        string GenCode(string script)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("using System;");
            sb.Append("using System.Xml;");
            sb.Append("using System.Data;");
            sb.Append("using System.Collections.Generic;");

            sb.Append(Environment.NewLine);
            sb.Append("public class Checker");
            sb.Append("{");
            sb.Append("    string field;");
            sb.Append("    int d = 0;");
            sb.Append("    float f = 0;");
            sb.Append("    string v = \"\";");
            sb.Append("    DataTable table_ = null;");
            sb.Append("    Dictionary<string, DataTable> tables_ = null;");
            sb.Append("    string envir_path_ = \"\";");
            
            this.GenUtilFunction(sb, script_);

            sb.Append("    public bool Run(string _filed_name, string v, DataRow row, ref string msg, DataTable __table, Dictionary<string, DataTable> __tables, string __envir_path)");
            sb.Append("    {");
            sb.Append("        field = _filed_name;");
            sb.Append("        int len = v.Length;");
            sb.Append("        int.TryParse(v, out d);");
            sb.Append("        float.TryParse(v, out f);");
            sb.Append("        this.v = v;");

            sb.Append("        table_ = __table;");
            sb.Append("        tables_ = __tables;");
            sb.Append("        envir_path_ = __envir_path;");

            sb.Append(script_expand_);

            sb.Append("        return true;");
            sb.Append("    }");
            sb.Append("}");

            code_ = sb.ToString();
            return code_;
        }

        void TransformCode()
        {
            script_expand_ = script_;

            /// transform field_name
            foreach (DataRow row in def_table_.Rows)
            {
                string field_name = row["×Ö¶ÎÃû"].ToString();
                string rep_field_name = "(" + field_name + ")";

                if (script_expand_.Contains(field_name))
                {
                    string new_field_name = string.Format("(row[\"{0}\"].ToString())", field_name);
                    script_expand_ = script_expand_.Replace(rep_field_name, new_field_name);
                }
            }

            /// transform except
            script_expand_ = script_expand_.Replace("except", "throw new System.Exception(\"\")");

            /// transform ret
            List<string> msgs = new List<string>();
            string regex_text = @"ret\(\042(.*?)\042\);\s*";
            MatchCollection mc = Regex.Matches(script_expand_, regex_text);
            foreach (Match match in mc)
            {
                string msg = match.Groups[1].Value;
                msgs.Add(msg);
            }
            foreach (string m in msgs)
            {
                string src = string.Format("ret(\"{0}\");", m);
                int index = script_expand_.IndexOf(src, 0);
                if (index != -1)
                {
                    string dst = string.Format("\n    msg = \"{0}\";\n    return false;\n", m);
                    dst = "\n{" + dst + "}\n";
                    script_expand_ = script_expand_.Replace(src, dst);
                }
            }

            /// fill return 
            if (script_expand_.IndexOf("return") == -1)
                script_expand_ = "return " + script_expand_;
            if (script_expand_.Length != 0 && script_expand_[script_expand_.Length - 1] != ';')
                script_expand_ += ";";

        }

        public MyScript(string field_name, string script, DataTable def_table)
        {
            field_name_ = field_name;
            script_ = script;
            def_table_ = def_table;

            TransformCode();
        }
    }

}
