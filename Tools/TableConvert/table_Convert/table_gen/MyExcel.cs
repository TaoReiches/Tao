using System;
using System.Collections.Generic;
using Microsoft.Office.Interop.Excel;
using System.IO;

namespace table_gen
{
    class MyExcel
    {
        Application kExcelApp = new Application();

        static public string[] s_RowIndex = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S",
                                     "T","U","V","W","X","Y","Z",
                                     "AA","AB","AC","AD","AE","AF","AG","AH","AI","AJ","AK","AL","AM","AN","AO","AP","AQ","AR","AS",
                                     "AT","AU","AV","AW","AX","AY","AZ",
                                     "BA","BB","BC","BD","BE","BF","BG","BH","BI","BJ","BK","BL","BM","BN","BO","BP","BQ","BR","BS",
                                     "BT","BU","BV","BW","BX","BY","BZ"};

        public class DefData
        {
            public string kDesc;
            public List<string> kType = new List<string>();
            public int iRowIndex;
            public string kMemberName;
            public int iFatherRow = -1;
            public int iLength = 0;
            public int iUseType = 0;    //  0 both  1 client  2 server
        }

        static public Dictionary<string, List<DefData>> kTableDefDataList = new Dictionary<string, List<DefData>>();
        static public string[] kTypeClass = { "UInt", "String", "Enum", "BitmaskEnum", "Float",
                                                "Int", "IntArray", "FloatArray","StringArray" };
        static public string[] kTypeReal = { "unsigned int", "std::string", "int", "unsigned int", "float",
                                                "int", "int", "float", "std::string"};

        public void ConvertExcel(string kName, string kNodeName)
        {
            List<DefData> kDefData = new List<DefData>();

            string kFullPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, kName);
            Workbook kWorkBook = kExcelApp.Workbooks.Open(kFullPath);
            Sheets kWorkSheets = kWorkBook.Worksheets;
            Worksheet kWorkSheet = (Worksheet)kWorkSheets[1];

            int iCountIdex = 0;
            while (true)
            {
                object id = kWorkSheet.get_Range(s_RowIndex[iCountIdex] + 1, Type.Missing).Value2;
                if (id == null)
                {
                    break;
                }
                string kStr = kWorkSheet.get_Range(s_RowIndex[iCountIdex] + 1, Type.Missing).Comment.Text();

                int iCatIndex = kStr.IndexOf("{");
                string kRealStr = kStr.Substring(iCatIndex);

                string[] firsts = { "{", "}", "\n" };
                string[] kTempStr = kRealStr.Split(firsts, StringSplitOptions.RemoveEmptyEntries);

                DefData kData = new DefData();
                kData.kDesc = id.ToString();
                kData.iRowIndex = iCountIdex;
                kData.kMemberName = kTempStr[0];

                int iTypeBegin = 1;
                if (kTempStr[1] == "(ClientOnly)")
                {
                    iTypeBegin = 2;
                    kData.iUseType = 1;
                }
                if (kTempStr[1] == "(ServerOnly)")
                {
                    iTypeBegin = 2;
                    kData.iUseType = 2;
                }
                for (int i = iTypeBegin; i < kTempStr.Length; i++)
                {
                    kData.kType.Add(kTempStr[i]);
                }

                string[] kSplitShuZu = { "#" };
                string[] kAllType = kData.kType[0].Split(kSplitShuZu, StringSplitOptions.RemoveEmptyEntries);
                if(kAllType.Length == 2)
                {
                    kData.kType[0] = kAllType[0];
                    kData.iLength = int.Parse(kAllType[1]);
                }

                if (kData.kType[0] == kTypeClass[2] || kData.kType[0] == kTypeClass[3])
                {
                    if (kData.kType[1].StartsWith("[") || kData.kType[1].StartsWith("<"))
                    {
                        string[] kSplitEnum = { "[", "]", ":" };
                        string[] kEnumData = kData.kType[1].Split(kSplitEnum, StringSplitOptions.RemoveEmptyEntries);
                        if (kEnumData.Length != 3)
                        {
                            string kFather = kEnumData[0];
                            bool bFind = false;

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
                                kData.iFatherRow = -251;
                            }
                        }
                    }
                    else
                    {
                        kData.iFatherRow = -250;
                    }
                }

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
        }
    }
    
}
