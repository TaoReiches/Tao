using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.CSharp;

using Microsoft.Office.Interop.Excel;
using System.Reflection;
using System.IO;

using System.Diagnostics;

//using System.Dynamic;

namespace table_gen
{
    class MyExcel
    {
        Microsoft.Office.Interop.Excel.Application kExcelApp = new Microsoft.Office.Interop.Excel.Application();

        static public string[] s_RowIndex = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S",
                                     "T","U","V","W","X","Y","Z",
                                     "AA","AB","AC","AD","AE","AF","AG","AH","AI","AJ","AK","AL","AM","AN","AO","AP","AQ","AR","AS",
                                     "AT","AU","AV","AW","AX","AY","AZ",
                                     "BA","BB","BC","BD","BE","BF","BG","BH","BI","BJ","BK","BL","BM","BN","BO","BP","BQ","BR","BS",
                                     "BT","BU","BV","BW","BX","BY","BZ"      };

        public class DefData
        {
            public string kDesc;        //  描述
            public List<string> kType = new List<string>();     //  类型
            public int iRowIndex;       //  第几个字段
            public string kMemberName;  //  头文件里的成员变量名字
            public int iFatherRow = -1; //  枚举或者位组合 定义在其他列
            public int iLength = 0;     //  数组数据的长度
            public int iUseType = 0;    //  数据是生成给哪一方使用  0 双方  1 仅客户端   2 仅服务器
        }

        static public Dictionary<string, List<DefData>> kTableDefDataList = new Dictionary<string, List<DefData>>();        //  所有字段信息
        static public string[] kTypeClass = { "无符号整形", "字符串", "枚举", "位组合", "浮点数", "整数", "整数数组", "浮点数组", "字符串数组", "默认整数数组", "枚举数值" };
        static public string[] kTypeReal = { "unsigned int", "std::string", "int", "unsigned int", "float", "int", "int", "float", "std::string","int" };

        public void ConvertExcel(string kName, string kNodeName)
        {
            List<DefData> kDefData = new List<DefData>();

            //  打开当前文档
            string kFullPath = Path.Combine(System.AppDomain.CurrentDomain.BaseDirectory, kName);
            Workbook kWorkBook = kExcelApp.Workbooks.Open(kFullPath);
            string a = kWorkBook._CodeName;
            Sheets kWorkSheets = kWorkBook.Worksheets;
            Worksheet kWorkSheet = (Worksheet)kWorkSheets[1];

            //  遍历每一列 获取批注
            int iCountIdex = 0;
            while (true)
            {
                //  获取当前格子的描述 并判断是否有值
                object id = kWorkSheet.get_Range(s_RowIndex[iCountIdex] + 1, Type.Missing).Value2;
                if (id == null)
                {
                    break;
                }
                //  获取批注
                string kStr = kWorkSheet.get_Range(s_RowIndex[iCountIdex] + 1, Type.Missing).Comment.Text();

                //  去掉第一个{ 之前的值
                int iCatIndex = kStr.IndexOf("{");
                string kRealStr = kStr.Substring(iCatIndex);
                //  对批注字符进行解析
                string[] firsts = { "{", "}", "\n" };
                string[] kTempStr = kRealStr.Split(firsts, StringSplitOptions.RemoveEmptyEntries);

                //  存贮内容
                DefData kData = new DefData();
                kData.kDesc = id.ToString();
                kData.iRowIndex = iCountIdex;
                kData.kMemberName = kTempStr[0];

                //  进一步判断一下是否指定了某一方专用
                int iTypeBegin = 1;
                if (kTempStr[1] == "(客户端用)")
                {
                    iTypeBegin = 2;
                    kData.iUseType = 1;
                }
                if (kTempStr[1] == "(服务器用)")
                {
                    iTypeBegin = 2;
                    kData.iUseType = 2;
                }
                for (int i = iTypeBegin; i < kTempStr.Length; i++)
                {
                    kData.kType.Add(kTempStr[i]);
                }

                //  如果是数组数据 就取出长度
                string[] kSplitShuZu = { "#" };
                string[] kAllType = kData.kType[0].Split(kSplitShuZu, StringSplitOptions.RemoveEmptyEntries);
                if(kAllType.Length == 2)
                {
                    kData.kType[0] = kAllType[0];
                    kData.iLength = int.Parse(kAllType[1]);
                }

                //  枚举或者批注 计算一下是否定义在其他列
                if (kData.kType[0] == kTypeClass[2] || kData.kType[0] == kTypeClass[3] || kData.kType[0] == kTypeClass[10])
                {
                    if (kData.kType[1].StartsWith("[") || kData.kType[1].StartsWith("<"))
                    {
                        string[] kSplitEnum = { "[", "]", ":" };
                        string[] kEnumData = kData.kType[1].Split(kSplitEnum, StringSplitOptions.RemoveEmptyEntries);
                        if (kEnumData.Length != 3)
                        {
                            string kFather = kEnumData[0];
                            bool bFind = false;
                            //  找到其定义所在的行
                            foreach (DefData kEnumDefData in kDefData)
                            {
                                if (kEnumDefData.kDesc == kFather)
                                {
                                    bFind = true;
                                    kData.iFatherRow = kEnumDefData.iRowIndex;
                                    break;
                                }
                            }
                            if (!bFind)
                            {
                                //  竟然没找到
                                kData.iFatherRow = -251;
                            }
                        }
                    }
                    else
                    {
                        //  错误的开端
                        kData.iFatherRow = -250;
                    }
                }

                //kTableDefDataList.Add(kName, kData);
                kDefData.Add(kData);

                iCountIdex++;
            }

            kTableDefDataList.Add(kNodeName, kDefData);

            kWorkBook.Close();
            System.Runtime.InteropServices.Marshal.ReleaseComObject(kWorkSheet);
            System.Runtime.InteropServices.Marshal.ReleaseComObject(kWorkSheets);
            System.Runtime.InteropServices.Marshal.ReleaseComObject(kWorkBook);
            System.Runtime.InteropServices.Marshal.ReleaseComObject(kExcelApp);
            GC.Collect();
            GC.WaitForPendingFinalizers();

            //  关闭excel         
            //kExcelApp.Workbooks.Close();
            //kExcelApp.Quit();
        }
    }
    
}
