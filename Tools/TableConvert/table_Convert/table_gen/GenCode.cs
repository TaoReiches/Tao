using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace table_gen
{
    class GenCode
    {
        //  生成头文件
        string kClassName;
        string kFileName;
        public void GenClass(MyNode kNode, StringBuilder sb)
        {
            kFileName = kNode.name;
            kClassName = kNode.name + "Table";

            GenHead(sb);
            //  枚举
            GenEnum(sb);
            //  位组合
            GenBitMove(sb);

            //  数据结构
            GenStruct(sb);

            //  函数定义
            GenGetFun(sb);

        }

        //  生成cpp文件
        public void GenCpp(StringBuilder sb)
        {
            //  获取函数
            GenCppGet(sb);
            GenCppGetClass(sb);
            //  构造函数
            GenGouzao(sb);
            //  析构
            GenXigou(sb);
            //  加载
            GenLoad(sb);
            //  填充数据
            GenFillData(sb);
        }

        void GenCppGet(StringBuilder sb)
        {
            sb.AppendLine(string.Format("{0}* {0}::m_pk{0} = NULL;", kClassMgrName));
            sb.AppendLine(string.Format("{0}* {0}::Get()", kClassMgrName));
            sb.AppendLine("{");
            sb.AppendLine(string.Format("    if(m_pk{0} == NULL)", kClassMgrName));
            sb.AppendLine("    {");
            sb.AppendLine(string.Format("        m_pk{0} = new {0}();", kClassMgrName));
            sb.AppendLine("    }");
            sb.AppendLine(string.Format("    return m_pk{0};", kClassMgrName));
            sb.AppendLine("}");
            sb.AppendLine("");

            //  获取全部
            sb.AppendLine("");
            sb.AppendLine(string.Format("const std::map<unsigned int, {0}*>& {1}::Get{0}Map()", kClassName, kClassMgrName));
            sb.AppendLine("{");
            sb.AppendLine(string.Format("    return m_k{0}Map;", kClassName));
            sb.AppendLine("}");

            //  获取给lua的数据
            sb.AppendLine("");
            sb.AppendLine(string.Format("TableResArray {0}Mgr::Get{0}Vec()", kClassName));
            sb.AppendLine("{");
            sb.AppendLine("    TableResArray kRecVec;");
            sb.AppendLine(string.Format("    for (std::map<unsigned int, {0}*>::iterator iMapItr = m_k{0}Map.begin(); iMapItr != m_k{0}Map.end(); ++iMapItr)", kClassName));
            sb.AppendLine("    {");
            sb.AppendLine("        kRecVec.pushBack(iMapItr->second);");
            sb.AppendLine("    }");
            sb.AppendLine("");
            sb.AppendLine("    return kRecVec;");
            sb.AppendLine("}");
        }

        void GenCppGetClass(StringBuilder sb)
        {
            sb.AppendLine("");
            sb.AppendLine(string.Format("const {0}* {1}::Get{0}({2} iTypeID)", kClassName, kClassMgrName, MyExcel.kTypeReal[0]));
            sb.AppendLine("{");
            sb.AppendLine(string.Format("    std::map<{0}, {1}*>::iterator iter = m_k{1}Map.find(iTypeID);", MyExcel.kTypeReal[0], kClassName));
            sb.AppendLine(string.Format("    if(iter != m_k{0}Map.end())", kClassName));
            sb.AppendLine("    {");
            sb.AppendLine("        return iter->second;");
            sb.AppendLine("    }");
            sb.AppendLine("    return NULL;");
            sb.AppendLine("}");
        }

        string kClassMgrName;
        void GenGetFun(StringBuilder sb)
        {
            kClassMgrName = kClassName + "Mgr";
            //  管理类
            sb.AppendLine("class TiXmlElement;");
            sb.AppendLine("class " + kClassMgrName);
            sb.AppendLine("{");
            //  加载函数
            sb.AppendLine("public:");
            string s = string.Format("    static {0}* Get();", kClassMgrName);
            sb.AppendLine(s);

            //  构造函数 析构函数 加载函数
            s = string.Format("    {0}();", kClassMgrName);
            sb.AppendLine(s);
            s = string.Format("    ~{0}();", kClassMgrName);
            sb.AppendLine(s);
            sb.AppendLine("");

            //  获取成员
            sb.AppendLine(string.Format("    const {0}* Get{0}({1} iTypeID);", kClassName, MyExcel.kTypeReal[0]));
            //  获取全部
            sb.AppendLine(string.Format("    const std::map<unsigned int, {0}*>& Get{0}Map();", kClassName));
            //  给lua提供数据
            sb.AppendLine(string.Format("    TableResArray Get{0}Vec();", kClassName));           

            //  成员变量和方法
            GenMember(sb);

            sb.AppendLine("};");
        }

        //  生成成员数据和方法
        void GenMember(StringBuilder sb)
        {
            sb.AppendLine("");
            sb.AppendLine("private:");
            sb.AppendLine("    bool    Load();");
            sb.AppendLine(string.Format("    void    FillData({0}* row, TiXmlElement* element);", kClassName));
            sb.AppendLine("");
            sb.AppendLine(string.Format("    std::map<{0},{1}*>      m_k{2}Map;", MyExcel.kTypeReal[0], kClassName, kClassName));
            sb.AppendLine(string.Format("    static {0}* m_pk{0};", kClassMgrName));
        }

        //  生成包含的头
        void GenHead(StringBuilder sb)
        {
            sb.AppendLine("#pragma once");
            sb.AppendLine("#pragma execution_character_set(\"utf-8\")");
            sb.AppendLine("");
            sb.AppendLine("#include <string>");
            sb.AppendLine("#include <map>");
            sb.AppendLine("#include \"SeTableResBase.h\"");
        }

        //  生成数据结构体
        void GenStruct(StringBuilder sb)
        {
            //  解析类名                      
            sb.AppendLine("");
            sb.AppendLine("//  自动生成表结构");
            sb.AppendLine(string.Format("struct {0} : SeTableResBase", kClassName));
            sb.AppendLine("{");

            //  每个成员变量
            //sb.AppendLine("public:");
            List<MyExcel.DefData> kDefData = MyExcel.kTableDefDataList[kFileName];
            foreach (MyExcel.DefData kData in kDefData)
            {
                string kType = kData.kType[0];

                //  仅客户端用的不写入
                if(kData.iUseType == 1)
                {
                    continue;
                }

                ////  解析是否为数组
                //string[] kSplitShuZu = { "#" };
                //string[] kType = kAllType.Split(kSplitShuZu, StringSplitOptions.RemoveEmptyEntries);

                for (int i = 0; i < MyExcel.kTypeClass.Length; i++)
                {
                    if (kType == MyExcel.kTypeClass[i])
                    {
                        string s = "";
                        if (i == 0 || i == 2 || i == 3)
                        {
                            s = string.Format("    {0,-12}               ui{1,-32}  ;   //  {2}   ", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc);
                        }
                        else if (i == 1)
                        {
                            s = string.Format("    {0,-12}               k{1,-32}   ;   //  {2}   ", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc);
                        }
                        else if (i == 4)
                        {
                            s = string.Format("    {0,-12}               f{1,-32}   ;   //  {2}   ", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc);
                        }
                        else if (i == 5)
                        {
                            s = string.Format("    {0,-12}               i{1,-32}   ;   //  {2}   ", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc);
                        }
                        else if (i == 6 || i == 9)
                        {
                            s = string.Format("    {0,-12}               i{1}[{3}]{4,-23}   ;   //  {2}   ", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc, kData.iLength, "");
                        }
                        else if (i == 7)
                        {
                            //int iSpaseLen = -32 + kData.kMemberName.Length + kData.iLength + 3;
                            s = string.Format("    {0,-12}               f{1}[{3}]{4,-23}   ;   //  {2}   ", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc, kData.iLength, "");
                        }
                        else if (i == 8)
                        {
                            s = string.Format("    {0,-12}               k{1}[{3}]{4,-23}   ;   //  {2}   ", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc, kData.iLength, "");
                        }
                        else if(i == 10)
                        {
                            //  这个比较蛋疼  枚举和数值合并成一列了
                            s = string.Format("    {0,-12}               ui{1,-32}  ;   //  {2}   ", MyExcel.kTypeReal[0], kData.kMemberName, kData.kDesc);
                            sb.AppendLine(s);
                            s = string.Format("    {0,-12}               f{1,-32}   ;   //  {2}   ", MyExcel.kTypeReal[4], kData.kMemberName, kData.kDesc);
                        }

                        sb.AppendLine(s);
                        break;
                    }
                }
            }

            sb.AppendLine("};");
            sb.AppendLine("");
        }

        //  生成枚举
        void GenEnum(StringBuilder sb)
        {
            List<MyExcel.DefData> kDefData = MyExcel.kTableDefDataList[kFileName];

            sb.AppendLine("");
            foreach (MyExcel.DefData kData in kDefData)
            {
                //  判断是否非法
                if (kData.iFatherRow != -1)
                {
                    continue;
                }

                string kType = kData.kType[0];

                if (kType == MyExcel.kTypeClass[2] || kType == MyExcel.kTypeClass[10])
                {
                    string strEnumName = kClassName.ToUpper() + "_" + kData.kMemberName.ToUpper();
                    sb.AppendLine("");
                    sb.AppendLine(string.Format("enum E_{0}", strEnumName));
                    sb.AppendLine("{");

                    for (int k = 1; k < kData.kType.Count; k++)
                    {
                        //  去掉不是【 开头的
                        if (!kData.kType[k].StartsWith("["))
                        {
                            continue;
                        }
                        string[] kSplitEnum = { "[", "]", ":" };
                        string[] kEnumData = kData.kType[k].Split(kSplitEnum, StringSplitOptions.RemoveEmptyEntries);
                        if (kEnumData.Length != 3)
                        {
                            continue;
                        }

                        string strname = kFileName.ToUpper() + "_" + kData.kMemberName.ToUpper() + "_" + kEnumData[2].ToUpper();
                        string s = string.Format("    {0,-32}            =    {1,-6} ,   //  {2}", strname, kEnumData[1], kEnumData[0]);
                        sb.AppendLine(s);
                    }

                    sb.AppendLine("};");
                }
            }
        }

        //  生成位组合
        void GenBitMove(StringBuilder sb)
        {
            sb.AppendLine("");

            List<MyExcel.DefData> kDefData = MyExcel.kTableDefDataList[kFileName];
            foreach (MyExcel.DefData kData in kDefData)
            {
                //  判断是否非法
                if (kData.iFatherRow != -1)
                {
                    continue;
                }

                string kType = kData.kType[0];

                if (kType == MyExcel.kTypeClass[3])
                {
                    string strEnumName = kClassName.ToUpper() + "_" + kData.kMemberName.ToUpper();
                    sb.AppendLine("");
                    sb.AppendLine(string.Format("enum M_{0}", strEnumName));
                    sb.AppendLine("{");

                    for (int k = 1; k < kData.kType.Count; k++)
                    {
                        //  去掉不是【 开头的
                        if (!kData.kType[k].StartsWith("<"))
                        {
                            continue;
                        }
                        string[] kSplitEnum = { "<", ">", ":" };
                        string[] kEnumData = kData.kType[k].Split(kSplitEnum, StringSplitOptions.RemoveEmptyEntries);

                        string strname = kFileName.ToUpper() + "_" + kData.kMemberName.ToUpper() + "_" + kEnumData[2].ToUpper();
                        string s = string.Format("    {0,-32}            =    1 << {1,-8} ,   //  {2}", strname, kEnumData[1], kEnumData[0]);
                        sb.AppendLine(s);
                    }

                    sb.AppendLine("};");
                }
            }
        }




        void GenGouzao(StringBuilder sb)
        {
            sb.AppendLine("");
            sb.AppendLine(string.Format("{0}::{0}()", kClassMgrName));
            sb.AppendLine("{");
            sb.AppendLine("    Load();");
            sb.AppendLine("}");
        }

        void GenXigou(StringBuilder sb)
        {
            sb.AppendLine("");
            sb.AppendLine(string.Format("{0}::~{0}()", kClassMgrName));
            sb.AppendLine("{");
            sb.AppendLine(string.Format("    m_k{0}Map.clear();", kClassName));
            sb.AppendLine("}");
        }
        void GenLoad(StringBuilder sb)
        {
            sb.AppendLine("");
            sb.AppendLine(string.Format("bool {0}::Load()", kClassMgrName));
            sb.AppendLine("{");
            sb.AppendLine(string.Format("    std::string path = \"Data/Table/{0}.xml\";", kFileName));
            sb.AppendLine("    FileMemory kMemory;");
            sb.AppendLine("    if(!FileLoader::LoadTableFile(path.c_str(),kMemory))");
            sb.AppendLine("    {");
            sb.AppendLine("        return false;");
            sb.AppendLine("    }");
            sb.AppendLine("");
            sb.AppendLine("    TiXmlDocument doc;");
            sb.AppendLine("    doc.Parse(kMemory.GetData());");
            sb.AppendLine("    if (doc.Error())");
            sb.AppendLine("    {");
            sb.AppendLine("        std::string err = path + \"   \" + std::string(doc.ErrorDesc());");
            sb.AppendLine("        // throw std::exception(err.c_str());");
            sb.AppendLine("        return false;");
            sb.AppendLine("    }");
            sb.AppendLine("");

            sb.AppendLine("    TiXmlElement* root = doc.FirstChildElement(\"root\");");

            sb.AppendLine("    if (root == 0)");
            sb.AppendLine("    {");
            sb.AppendLine("        // throw std::exception(\"root is null!\");");
            sb.AppendLine("        return false;");
            sb.AppendLine("    }");
            sb.AppendLine("");

            sb.AppendLine("    TiXmlElement* element = root->FirstChildElement(\"content\");");
            sb.AppendLine("    while (element != 0)");
            sb.AppendLine("    {");

            sb.AppendLine(string.Format("        {0} * row = new {1}();", kClassName, kClassName));
            sb.AppendLine("        FillData(row, element);");
            //sb.AppendLine("        FillMapping(row);");

            sb.AppendLine("        element = element->NextSiblingElement();");
            sb.AppendLine("    }");
            sb.AppendLine("");
            sb.AppendLine("    return true;");
            sb.AppendLine("}");
        }

        void GenFillData(StringBuilder sb)
        {
            List<MyExcel.DefData> kDefData = MyExcel.kTableDefDataList[kFileName];

            sb.AppendLine("");
            sb.AppendLine(string.Format("void {0}::FillData({1}* row, TiXmlElement* element)", kClassMgrName, kClassName));
            sb.AppendLine("{");
            //  添加获取元素的常量
            sb.AppendLine("    int int_value = 0;");
            foreach (MyExcel.DefData kData in kDefData)
            {
                if (kData.kType[0] == MyExcel.kTypeClass[1])
                {
                    sb.AppendLine("    std::string str_value = \"\";");
                    break;
                }
            }
            foreach (MyExcel.DefData kData in kDefData)
            {
                if (kData.kType[0] == MyExcel.kTypeClass[4])
                {
                    sb.AppendLine("    double float_value = 0.0f;");
                    break;
                }
            }
            sb.AppendLine("");

            string kIDkey = "";
            //  为每一个元素赋值
            foreach (MyExcel.DefData kData in kDefData)
            {
                //  仅客户端使用的不写入
                if(kData.iUseType == 1)
                {
                    continue;
                }
                if (kIDkey == "")
                {
                    kIDkey = kData.kMemberName;
                }
                if (kData.kType[0] == MyExcel.kTypeClass[1])
                {
                    sb.AppendLine(string.Format("    str_value = element->Attribute(\"{0}\");", kData.kMemberName));
                    sb.AppendLine(string.Format("    row->k{0} = str_value;", kData.kMemberName));
                }
                else if (kData.kType[0] == MyExcel.kTypeClass[4])
                {
                    sb.AppendLine(string.Format("    element->Attribute(\"{0}\", &float_value);", kData.kMemberName));
                    sb.AppendLine(string.Format("    row->f{0} = ({1})float_value;", kData.kMemberName, MyExcel.kTypeReal[4]));
                }
                else if (kData.kType[0] == MyExcel.kTypeClass[5])
                {
                    sb.AppendLine(string.Format("    element->Attribute(\"{0}\", &int_value);", kData.kMemberName));
                    sb.AppendLine(string.Format("    row->i{0} = int_value;", kData.kMemberName));
                }
                else if (kData.kType[0] == MyExcel.kTypeClass[6])
                {
                    for (int i = 0; i < kData.iLength; i++)
                    {
                        sb.AppendLine(string.Format("    element->Attribute(\"{0}{1}\", &int_value);", kData.kMemberName, i));
                        sb.AppendLine(string.Format("    row->i{0}[{1}] = int_value;", kData.kMemberName, i));
                    }
                }
                else if (kData.kType[0] == MyExcel.kTypeClass[7])
                {
                    for (int i = 0; i < kData.iLength; i++)
                    {
                        sb.AppendLine(string.Format("    element->Attribute(\"{0}{1}\", &float_value);", kData.kMemberName, i));
                        sb.AppendLine(string.Format("    row->f{0}[{1}] = float_value;", kData.kMemberName, i));
                    }
                }
                else if (kData.kType[0] == MyExcel.kTypeClass[8])
                {
                    for (int i = 0; i < kData.iLength; i++)
                    {
                        sb.AppendLine(string.Format("    str_value = element->Attribute(\"{0}{1}\");", kData.kMemberName, i));
                        sb.AppendLine(string.Format("    row->k{0}[{1}] = str_value;", kData.kMemberName, i));
                    }
                }
                else if (kData.kType[0] == MyExcel.kTypeClass[9])
                {
                    for (int i = 0; i < kData.iLength; i++)
                    {
                        sb.AppendLine(string.Format("    element->Attribute(\"{0}{1}\", &int_value);", kData.kMemberName, i));
                        sb.AppendLine(string.Format("    row->i{0}[{1}] = int_value;", kData.kMemberName, i));
                    }
                }
                else if (kData.kType[0] == MyExcel.kTypeClass[10])
                {
                    sb.AppendLine(string.Format("    element->Attribute(\"ui{0}\", &int_value);", kData.kMemberName));
                    sb.AppendLine(string.Format("    row->ui{0} = ({1})int_value;", kData.kMemberName, MyExcel.kTypeReal[0]));
                    sb.AppendLine(string.Format("    element->Attribute(\"f{0}\", &float_value);", kData.kMemberName));
                    sb.AppendLine(string.Format("    row->f{0} = ({1})float_value;", kData.kMemberName, MyExcel.kTypeReal[4]));
                }
                else
                {
                    sb.AppendLine(string.Format("    element->Attribute(\"{0}\", &int_value);", kData.kMemberName));
                    sb.AppendLine(string.Format("    row->ui{0} = ({1})int_value;", kData.kMemberName, MyExcel.kTypeReal[0]));
                }
            }

            //  保存数据
            sb.AppendLine("");
            sb.AppendLine(string.Format("    m_k{0}Map[row->ui{1}] = row;", kClassName, kIDkey));
            sb.AppendLine("}");
        }

        //void GenFillDataFunction(StringBuilder sb)
        //{
        //    sb.AppendLine(string.Format(
        //        "    void FillData({0} *row, TiXmlElement* element)",
        //        kClassName));
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
        //    sb.AppendLine(string.Format("void FillMapping({0} *row)", kClassName));
        //    sb.AppendLine("{");
        //    sb.AppendLine("data_.push_back(row);");
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

        //void GenStruct(StringBuilder sb)
        //{
        //    MyNode.struct_name = "";

        //    string[] name_segs = MyNode.name.ToLower().Split('_');

        //    for (int i = 0; i < name_segs.Length; ++i)
        //    {
        //        string first_char = name_segs[i][0].ToString().ToUpper();
        //        string lower_name = name_segs[i].ToLower();
        //        MyNode.struct_name += first_char + name_segs[i].Substring(1);
        //    }

        //    int client_field_count = 0;
        //    MyNode.struct_ptr_name = MyNode.struct_name + " *";
        //    sb.AppendLine("struct " + MyNode.struct_name);
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
        //    sb.AppendLine(string.Format("    ~{0}::{0}()", kClassMgrName));
        //    sb.AppendLine("    {");
        //    sb.AppendLine("        this->Release();");
        //    sb.AppendLine("    }");
        //}

        ////  加载函数
        //void GenLoadFunction(StringBuilder sb)
        //{
        //    sb.AppendLine("");
        //    sb.AppendLine("    bool Load(const std::string &path)");
        //    sb.AppendLine("    {");
        //    //sb.AppendLine("        LOG_INF(__fmt(\"load %1% begin.\") % path);");
        //    //sb.AppendLine("");

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

        //    sb.AppendLine(string.Format("            {0} * row = new {1}();", kClassName, kClassName));
        //    sb.AppendLine("            FillData(row, element);");
        //    sb.AppendLine("            FillMapping(row);");

        //    sb.AppendLine("            element = element->NextSiblingElement();");

        //    sb.AppendLine("        }");
        //    sb.AppendLine("");

        //    //sb.AppendLine("        LOG_INF(__fmt(\"load %1% end.\") % path);");
        //    //sb.AppendLine("");

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
    }
}
