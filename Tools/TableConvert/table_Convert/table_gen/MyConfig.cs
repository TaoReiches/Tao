using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Xml;
using System.Net.Sockets;

namespace table_gen
{
    class MyConfig
    {
        public static MyConfig Instance = null;

        public MyConfig()
        {
            Instance = this;
        }

        bool use_tip_ = false;
        string xls_path_ = "";
        string def_path_ = "";
        string envir_path_ = "";
        string xml_path_ = "";
        string code_path_ = "";
        string client_code_path_ = "";
        Dictionary<string, string> base_tables_ = new Dictionary<string,string>();

        public bool UseTip
        {
            get 
            {
                return use_tip_;
            }
        }

        public string XLSPath
        {
            get
            {
                return xls_path_;
            }
        }

        public string DefPath
        {
            get
            {
                return def_path_;
            }
        }

        public string EnvirPath
        {
            get
            {
                return envir_path_;
            }
        }

        public string XMLPath
        {
            get
            {
                return xml_path_;
            }
        }

        public string CodePath
        {
            get
            {
                return code_path_;
            }
        }
        public string ClientCodePath
        {
            get
            {
                return client_code_path_;
            }
        }
        
        public bool IsBaseFile(string filename)
        {
            return base_tables_.ContainsKey(filename);
        }

        public void Clear()
        {
            base_tables_.Clear();
        }

        public void Load()
        {
            this.Clear();

            //XmlDocument doc = new XmlDocument();
            //doc.Load("./config.xml");

            //XmlNode root_node = doc.SelectSingleNode("table_gen");
            //XmlNode path_node = root_node.SelectSingleNode("path");
            //use_tip_ = bool.Parse(root_node.Attributes["use_tip"].Value);
            
            //xls_path_ = path_node.SelectSingleNode("xls_path").Attributes["name"].Value;
            //def_path_ = path_node.SelectSingleNode("def_path").Attributes["name"].Value;
            //envir_path_ = path_node.SelectSingleNode("envir_path").Attributes["name"].Value;
            //xml_path_ = path_node.SelectSingleNode("xml_path").Attributes["name"].Value;
            //code_path_ = path_node.SelectSingleNode("code_path").Attributes["name"].Value;
            //client_code_path_ = path_node.SelectSingleNode("client_code_path").Attributes["name"].Value;
            //XmlNode base_table_node = root_node.SelectSingleNode("base_tables");
            //XmlNodeList base_table_nodes = base_table_node.SelectNodes("base_table");

            //foreach (XmlNode node in base_table_nodes)
            //{
            //    string filename = node.Attributes["name"].Value;
            //    base_tables_.Add(filename, filename);
            //}
            def_path_ = "Def/";
            xml_path_ = "Xml/";
            code_path_ = "Code/";
            client_code_path_ = "Code/";
            xls_path_ = "Xlsx/";
            if (!Directory.Exists(def_path_))
                Directory.CreateDirectory(def_path_);

            if (!Directory.Exists(xml_path_))
                Directory.CreateDirectory(xml_path_);

            if (!Directory.Exists(code_path_))
                Directory.CreateDirectory(code_path_);

            if (!Directory.Exists(xls_path_))
                Directory.CreateDirectory(xls_path_);
        }
    }
}
