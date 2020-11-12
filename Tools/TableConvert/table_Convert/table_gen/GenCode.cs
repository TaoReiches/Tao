using System;
using System.Collections.Generic;
using System.Text;

namespace table_gen
{
    class GenCode
    {
        string kClassName;
        string kFileName;
        public void GenClass(MyNode kNode, StringBuilder sb)
        {
            kFileName = kNode.name;
            kClassName = kNode.name + "Table";

            GenHead(sb);
            GenEnum(sb);
            GenBitMove(sb);
            GenStruct(sb);
            GenGetFun(sb);
        }

        public void GenCpp(StringBuilder sb)
        {
            GenCppGet(sb);
            GenCppGetClass(sb);
            GenConstructor(sb);
            GenDestructor(sb);
            GenLoad(sb);
            GenFillData(sb);
        }

        void GenCppGet(StringBuilder sb)
        {
            sb.AppendLine(string.Format("std::unique_ptr<{0}>       {0}::m_pk{0} = nullptr;", kClassMgrName));
            sb.AppendLine(string.Format("const std::unique_ptr<{0}>& {0}::Get()", kClassMgrName));
            sb.AppendLine("{");
            sb.AppendLine(string.Format("    if(!m_pk{0})", kClassMgrName));
            sb.AppendLine("    {");
            sb.AppendLine(string.Format("        m_pk{0} = std::unique_ptr<{0}>(new {0}());", kClassMgrName));
            sb.AppendLine("    }");
            sb.AppendLine(string.Format("    return m_pk{0};", kClassMgrName));
            sb.AppendLine("}");
            sb.AppendLine("");

            //  get all
            sb.AppendLine("");
            sb.AppendLine(string.Format("const std::map<unsigned int, std::shared_ptr<const {0}>>& {1}::Get{0}Map() const", kClassName, kClassMgrName));
            sb.AppendLine("{");
            sb.AppendLine(string.Format("    return m_k{0}Map;", kClassName));
            sb.AppendLine("}");
        }

        void GenCppGetClass(StringBuilder sb)
        {
            sb.AppendLine("");
            sb.AppendLine(string.Format("const std::shared_ptr<const {0}>& {1}::Get{0}({2} iTypeID) const", kClassName, kClassMgrName, MyExcel.kTypeReal[0]));
            sb.AppendLine("{");
            sb.AppendLine(string.Format("    auto iter = m_k{1}Map.find(iTypeID);", MyExcel.kTypeReal[0], kClassName));
            sb.AppendLine(string.Format("    if(iter != m_k{0}Map.end())", kClassName));
            sb.AppendLine("    {");
            sb.AppendLine("        return iter->second;");
            sb.AppendLine("    }");
            sb.AppendLine(string.Format("    static auto nullValue = std::shared_ptr<const {0}>();", kClassName));
            sb.AppendLine("    return nullValue;");
            sb.AppendLine("}");
        }

        string kClassMgrName;
        void GenGetFun(StringBuilder sb)
        {
            kClassMgrName = kClassName + "Mgr";
            //  manager
            sb.AppendLine("class TiXmlElement;");
            sb.AppendLine("class " + kClassMgrName);
            sb.AppendLine("{");
            //  load function
            sb.AppendLine("public:");
            string s = string.Format("    static const std::unique_ptr<{0}>& Get();", kClassMgrName);
            sb.AppendLine(s);

            //  constructor destructor
            s = string.Format("    {0}();", kClassMgrName);
            sb.AppendLine(s);
            s = string.Format("    ~{0}();", kClassMgrName);
            sb.AppendLine(s);
            sb.AppendLine("");

            //  get member
            sb.AppendLine(string.Format("    const std::shared_ptr<const {0}>& Get{0}({1} iTypeID) const;", kClassName, MyExcel.kTypeReal[0]));
            //  get all
            sb.AppendLine(string.Format("    const std::map<unsigned int, std::shared_ptr<const {0}>>& Get{0}Map() const;", kClassName));

            GenMember(sb);

            sb.AppendLine("};");
        }

        void GenMember(StringBuilder sb)
        {
            sb.AppendLine("");
            sb.AppendLine("private:");
            sb.AppendLine("    bool    Load();");
            sb.AppendLine(string.Format("    void    FillData({0}* row, TiXmlElement* element);", kClassName));
            sb.AppendLine("");
            sb.AppendLine(string.Format("    std::map<{0},std::shared_ptr<const {1}>>      m_k{2}Map;", MyExcel.kTypeReal[0], kClassName, kClassName));
            sb.AppendLine(string.Format("    static std::unique_ptr<{0}>        m_pk{0};", kClassMgrName));
        }

        void GenHead(StringBuilder sb)
        {
            sb.AppendLine("#pragma once");
            sb.AppendLine("#pragma execution_character_set(\"utf-8\")");
            sb.AppendLine("");
            sb.AppendLine("/**********************************************");
            sb.AppendLine("* Author: Tao Wang  Copyright reserved");
            sb.AppendLine("* Contact: tao.reiches@gmail.com");
            sb.AppendLine("**********************************************/");
            sb.AppendLine("");
            sb.AppendLine("#include <string>");
            sb.AppendLine("#include <map>");
            sb.AppendLine("#include <memory>");
        }

        void GenStruct(StringBuilder sb)
        {
            sb.AppendLine("");
            // sb.AppendLine("//  Auto genarate table struct");
            sb.AppendLine(string.Format("class {0}", kClassName));
            sb.AppendLine("{");
            sb.AppendLine("public:");

            List<MyExcel.DefData> kDefData = MyExcel.kTableDefDataList[kFileName];
            foreach (MyExcel.DefData kData in kDefData)
            {
                string kType = kData.kType[0];

                // don't write client data
                if(kData.iUseType == 1)
                {
                    continue;
                }

                for (int i = 0; i < MyExcel.kTypeClass.Length; i++)
                {
                    if (kType == MyExcel.kTypeClass[i])
                    {
                        string s = "";
                        if (i == 0 || i == 2 || i == 3)
                        {
                            s = string.Format("    {0,-12}               ui{1,-32}  ;   //  {2}", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc);
                        }
                        else if (i == 1)
                        {
                            s = string.Format("    {0,-12}               k{1,-32}   ;   //  {2}", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc);
                        }
                        else if (i == 4)
                        {
                            s = string.Format("    {0,-12}               f{1,-32}   ;   //  {2}", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc);
                        }
                        else if (i == 5)
                        {
                            s = string.Format("    {0,-12}               i{1,-32}   ;   //  {2}", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc);
                        }
                        else if (i == 6 || i == 9)
                        {
                            s = string.Format("    {0,-12}               i{1}[{3}]{4,-23}   ;   //  {2}", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc, kData.iLength, "");
                        }
                        else if (i == 7)
                        {
                            //int iSpaseLen = -32 + kData.kMemberName.Length + kData.iLength + 3;
                            s = string.Format("    {0,-12}               f{1}[{3}]{4,-23}   ;   //  {2}", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc, kData.iLength, "");
                        }
                        else if (i == 8)
                        {
                            s = string.Format("    {0,-12}               k{1}[{3}]{4,-23}   ;   //  {2}", MyExcel.kTypeReal[i], kData.kMemberName, kData.kDesc, kData.iLength, "");
                        }
                        else if(i == 10)
                        {
                            s = string.Format("    {0,-12}               ui{1,-32}  ;   //  {2}", MyExcel.kTypeReal[0], kData.kMemberName, kData.kDesc);
                            sb.AppendLine(s);
                            s = string.Format("    {0,-12}               f{1,-32}   ;   //  {2}", MyExcel.kTypeReal[4], kData.kMemberName, kData.kDesc);
                        }

                        sb.AppendLine(s);
                        break;
                    }
                }
            }

            sb.AppendLine("};");
            sb.AppendLine("");
        }

        void GenEnum(StringBuilder sb)
        {
            List<MyExcel.DefData> kDefData = MyExcel.kTableDefDataList[kFileName];

            sb.AppendLine("");
            foreach (MyExcel.DefData kData in kDefData)
            {
                if (kData.iFatherRow != -1)
                {
                    continue;
                }

                string kType = kData.kType[0];

                if (kType == MyExcel.kTypeClass[2])
                {
                    string strEnumName = kClassName.ToUpper() + "_" + kData.kMemberName.ToUpper();
                    sb.AppendLine("");
                    sb.AppendLine(string.Format("enum E_{0}", strEnumName));
                    sb.AppendLine("{");

                    for (int k = 1; k < kData.kType.Count; k++)
                    {
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

        void GenBitMove(StringBuilder sb)
        {
            sb.AppendLine("");

            List<MyExcel.DefData> kDefData = MyExcel.kTableDefDataList[kFileName];
            foreach (MyExcel.DefData kData in kDefData)
            {
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

        void GenConstructor(StringBuilder sb)
        {
            sb.AppendLine("");
            sb.AppendLine(string.Format("{0}::{0}()", kClassMgrName));
            sb.AppendLine("{");
            sb.AppendLine("    Load();");
            sb.AppendLine("}");
        }

        void GenDestructor(StringBuilder sb)
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
            sb.AppendLine("    char* fileData = nullptr;");
            sb.AppendLine("    std::ifstream file (path, std::ios::in | std::ios::binary | std::ios::ate);");
            sb.AppendLine("    if (file.is_open())");
            sb.AppendLine("    {");
            sb.AppendLine("        std::streampos size = file.tellg();");
            sb.AppendLine("        fileData = new char[size];");
            sb.AppendLine("        file.seekg(0, std::ios::beg);");
            sb.AppendLine("        file.read(fileData, size);");
            sb.AppendLine("        file.close();");
            sb.AppendLine("    }");
            sb.AppendLine("    if (fileData == nullptr)");
            sb.AppendLine("    {");
            sb.AppendLine("        return false;");
            sb.AppendLine("    }");

            sb.AppendLine("    TiXmlDocument doc;");
            sb.AppendLine("    doc.Parse(fileData);");
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
            sb.AppendLine("    delete[] fileData;");
            sb.AppendLine("    return true;");
            sb.AppendLine("}");
        }

        void GenFillData(StringBuilder sb)
        {
            List<MyExcel.DefData> kDefData = MyExcel.kTableDefDataList[kFileName];

            sb.AppendLine("");
            sb.AppendLine(string.Format("void {0}::FillData({1}* row, TiXmlElement* element)", kClassMgrName, kClassName));
            sb.AppendLine("{");
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
            foreach (MyExcel.DefData kData in kDefData)
            {
                //  don't write client data
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
                        sb.AppendLine(string.Format("    row->f{0}[{1}] = ({2})float_value;", kData.kMemberName, i, MyExcel.kTypeReal[4]));
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
                else
                {
                    sb.AppendLine(string.Format("    element->Attribute(\"{0}\", &int_value);", kData.kMemberName));
                    sb.AppendLine(string.Format("    row->ui{0} = ({1})int_value;", kData.kMemberName, MyExcel.kTypeReal[0]));
                }
            }

            sb.AppendLine("");
            sb.AppendLine(string.Format("    m_k{0}Map[row->ui{1}] = std::shared_ptr<{0}>(row);", kClassName, kIDkey));
            sb.AppendLine("}");
        }
    }
}
