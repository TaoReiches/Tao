using System;
using System.Collections.Generic;
using System.Text;
using System.Data;

namespace table_gen
{
    class MyXml
    {
        public void GenXml(string kFileName, StringBuilder sb, MyNode kNode,int iType)
        {
            List<MyExcel.DefData> kDefData = MyExcel.kTableDefDataList[kFileName];

            sb.AppendLine("<?xml version=\"1.0\" encoding=\"utf-8\"?>");
            sb.AppendLine("<root>");

            int iRowIndex = 1;

            foreach (DataRow kData in kNode.content_data_set.Tables[0].Rows)
            {
                iRowIndex++;

                object[] kAllValue = kData.ItemArray;
                sb.Append("    <content  ");
                
                for(int i = 0; i < kAllValue.Length; i++)
                {
                    if(i >= kDefData.Count)
                    {
                        System.Windows.Forms.MessageBox.Show(string.Format("Exceed the maximum column{0}", MyExcel.s_RowIndex[i]));
                        return;
                    }
                    MyExcel.DefData kDefTypeData = kDefData[i];
                    object kValue = kAllValue[i];
                
                    if(iType == 1)
                    {
                        if(kDefTypeData.iUseType == 2)
                        {
                            continue;
                        }
                        if (kValue.ToString() == "-")
                        {
                            continue;
                        }
                    }
                    if (iType == 2)
                    {
                        if (kDefTypeData.iUseType == 1)
                        {
                            continue;
                        }
                    }

                    if (MyExcel.kTypeClass[0] == kDefTypeData.kType[0] || MyExcel.kTypeClass[5] == kDefTypeData.kType[0])
                    {
                        int iIntValue = 250;

                        if(kValue.ToString() == "-")
                        {
                            iIntValue = -1;
                        }
                        else if(!int.TryParse(kValue.ToString(), out iIntValue))
                        {
                            byte[] idIntBytes = Encoding.UTF8.GetBytes(kValue.ToString());
                            if (idIntBytes.Length == 4)
                            {
                                iIntValue = 0
                                            | (idIntBytes[0] << 24)
                                            | (idIntBytes[1] << 16)
                                            | (idIntBytes[2] << 8)
                                            | (idIntBytes[3] << 0);
                            }
                        }
                        sb.Append(string.Format("{0}=", kDefTypeData.kMemberName));
                        sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                    }
                    else if(MyExcel.kTypeClass[1] == kDefTypeData.kType[0])
                    {
                        sb.Append(string.Format("{0}=", kDefTypeData.kMemberName));
                        sb.Append(string.Format("\"{0}\" ", kValue.ToString()));
                    }
                    else if(MyExcel.kTypeClass[2] == kDefTypeData.kType[0])
                    {
                        int iEnumValue = 0;

                        List<string> kDefTypeStr = kDefTypeData.kType;
                        if(kDefTypeData.iFatherRow != -1)
                        {
                            bool bFind = false;
                            foreach(MyExcel.DefData kFindData in kDefData)
                            {
                                if(kFindData.iRowIndex == kDefTypeData.iFatherRow)
                                {
                                    bFind = true;
                                    kDefTypeStr = kFindData.kType;
                                    break;
                                }
                            }
                            if(!bFind)
                            {
                                System.Windows.Forms.MessageBox.Show(string.Format("An error Enum occurs at {0}Row{1}Column", iRowIndex, MyExcel.s_RowIndex[i]));
                            }
                        }

                        bool bFindEnum = false;
                        for (int k = 1; k < kDefTypeStr.Count; k++)
                        {
                            if(!kDefTypeStr[k].StartsWith("["))
                            {
                                continue;
                            }
                            string[] kSplitEnum = { "[", "]", ":" };
                            string[] kEnumData = kDefTypeStr[k].Split(kSplitEnum, StringSplitOptions.RemoveEmptyEntries);
                            string[] kSplitValue = { ":" };
                            string[] kValueData = kValue.ToString().Split(kSplitValue, StringSplitOptions.RemoveEmptyEntries);
                            if (kValueData[0] == kEnumData[0])
                            {
                                iEnumValue = int.Parse(kEnumData[1]);
                                bFindEnum = true;
                                break;
                            }
                        }
                        if(!bFindEnum)
                        {
                            if(kValue.ToString() != "-")
                            {
                                System.Windows.Forms.MessageBox.Show(string.Format("Doesn't exsit Enum used at{0}Row{1}Column", iRowIndex, MyExcel.s_RowIndex[i]));
                            }
                        }
                        sb.Append(string.Format("{0}=", kDefTypeData.kMemberName));
                        sb.Append(string.Format("\"{0}\" ", iEnumValue.ToString()));
                    }
                    //else if (MyExcel.kTypeClass[10] == kDefTypeData.kType[0])
                    //{
                    //    int iEnumValue = 0;
                    //    float fEnumValue = 0.0f;

                    //    List<string> kDefTypeStr = kDefTypeData.kType;
                    //    if (kDefTypeData.iFatherRow != -1)
                    //    {
                    //        bool bFind = false;
                    //        foreach (MyExcel.DefData kFindData in kDefData)
                    //        {
                    //            if (kFindData.iRowIndex == kDefTypeData.iFatherRow)
                    //            {
                    //                bFind = true;
                    //                kDefTypeStr = kFindData.kType;
                    //                break;
                    //            }
                    //        }
                    //        if (!bFind)
                    //        {
                    //            System.Windows.Forms.MessageBox.Show(
                    //                string.Format("An error Enum occurs at {0}Row{1}Column", kData.ToString(), kDefTypeStr.ToString()));
                    //        }
                    //    }

                    //    for (int k = 1; k < kDefTypeStr.Count; k++)
                    //    {
                    //        if (!kDefTypeStr[k].StartsWith("["))
                    //        {
                    //            continue;
                    //        }
                    //        string[] kSplitEnum = { "[", "]", ":" };
                    //        string[] kEnumData = kDefTypeStr[k].Split(kSplitEnum, StringSplitOptions.RemoveEmptyEntries);
                    //        string[] kSplitValue = { ":" };
                    //        string[] kValueData = kValue.ToString().Split(kSplitValue, StringSplitOptions.RemoveEmptyEntries);
                    //        if (kValueData[0] == kEnumData[0])
                    //        {
                    //            iEnumValue = int.Parse(kEnumData[1]);
                    //            fEnumValue = float.Parse(kValueData[1]);
                    //            break;
                    //        }
                    //    }
                    //    sb.Append(string.Format("ui{0}=", kDefTypeData.kMemberName));
                    //    sb.Append(string.Format("\"{0}\" ", iEnumValue.ToString()));
                    //    sb.Append(string.Format("f{0}=", kDefTypeData.kMemberName));
                    //    sb.Append(string.Format("\"{0}\" ", fEnumValue.ToString()));
                    //}
                    else if (MyExcel.kTypeClass[3] == kDefTypeData.kType[0])
                    {
                        int iBitMoveValue = 0;
                        bool bFindBitMove = false;
                        for (int k = 1; k < kDefTypeData.kType.Count; k++)
                        {
                            string[] kSplitEnum = { "<", ">", ":" };
                            string[] kEnumData = kDefTypeData.kType[k].Split(kSplitEnum, StringSplitOptions.RemoveEmptyEntries);
                            string kBitMoveStr = kValue.ToString();
                            string[] kSplitBitMove = { "|" };
                            string[] kBitMoveData = kBitMoveStr.Split(kSplitBitMove, StringSplitOptions.RemoveEmptyEntries);
                            for (int m = 0; m < kBitMoveData.Length; m++ )
                            {
                                if (kBitMoveData[m] == kEnumData[0])
                                {
                                    iBitMoveValue |= 1 << int.Parse(kEnumData[1]);
                                    bFindBitMove = true;
                                    break;
                                }
                            }
                        }
                        if (!bFindBitMove)
                        {
                            if (kValue.ToString() != "-")
                            {
                                System.Windows.Forms.MessageBox.Show(
                                    string.Format("Not exsit Bitmask Enum at {0}Row{1}Column", iRowIndex, MyExcel.s_RowIndex[i]));
                            }
                        }

                        sb.Append(string.Format("{0}=", kDefTypeData.kMemberName));
                        sb.Append(string.Format("\"{0}\" ", (iBitMoveValue).ToString()));
                    }
                    else if (MyExcel.kTypeClass[4] == kDefTypeData.kType[0])
                    {
                        float fFloatValue = 0.0f;
                        string kStr = kValue.ToString();
                        if (kStr == "-" || kStr == "")
                        {
                            fFloatValue = -1.0f;
                        }
                        else
                        {
                            fFloatValue = float.Parse(kStr);
                        }
                        //
                        sb.Append(string.Format("{0}=", kDefTypeData.kMemberName));
                        sb.Append(string.Format("\"{0}\" ", fFloatValue.ToString()));
                    }
                    else if(MyExcel.kTypeClass[6] == kDefTypeData.kType[0])
                    {
                        int iIntValue = -1;
                        int iIndex = 0;

                        string kSrcStr = kValue.ToString();
                        if(kSrcStr.Contains(","))
                        {
                            string[] kSplitShuZu = { ",", "\n" };
                            string[] kShuZu = kValue.ToString().Split(kSplitShuZu, StringSplitOptions.RemoveEmptyEntries);
                            for (int n = 0; n < kDefTypeData.iLength; n++)
                            {
                                sb.Append(string.Format("{0}{1}=", kDefTypeData.kMemberName, n));
                                if (kValue.ToString() == "-")
                                {
                                    sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                                }
                                else
                                {                                    
                                    if (n < kShuZu.Length && !int.TryParse(kShuZu[n], out iIntValue))
                                    {
                                        byte[] idIntBytes = Encoding.UTF8.GetBytes(kShuZu[n]);
                                        if (idIntBytes.Length == 4)
                                        {
                                            iIntValue = 0
                                                        | (idIntBytes[0] << 24)
                                                        | (idIntBytes[1] << 16)
                                                       | ((Int32)idIntBytes[2] << 8)
                                                      | ((Int32)idIntBytes[3] << 0);
                                        }
                                    }

                                    sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                                }
                            }
                        }
                        else
                        {
                            string[] kSplitShuZu = { "#", "\n" };
                            string[] kShuZu = kValue.ToString().Split(kSplitShuZu, StringSplitOptions.RemoveEmptyEntries);

                            
                            for (int n = 0; n < kDefTypeData.iLength; n++)
                            {
                                sb.Append(string.Format("{0}{1}=", kDefTypeData.kMemberName, n));
                                if (kValue.ToString() == "-")
                                {
                                    sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                                }
                                else if (kShuZu.Length == 1)
                                {
                                    if (!int.TryParse(kShuZu[iIndex], out iIntValue))
                                    {
                                        byte[] idIntBytes = System.Text.UTF8Encoding.UTF8.GetBytes(kShuZu[iIndex]);
                                        if (idIntBytes.Length == 4)
                                        {
                                            iIntValue = 0
                                                        | ((Int32)idIntBytes[0] << 24)
                                                        | ((Int32)idIntBytes[1] << 16)
                                                       | ((Int32)idIntBytes[2] << 8)
                                                      | ((Int32)idIntBytes[3] << 0);
                                        }
                                    }
                                    sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                                }
                                else
                                {
                                    if (n * 2 < kShuZu.Length && int.Parse(kShuZu[iIndex]) == n + 1)
                                    {
                                        iIndex++;

                                        if (!int.TryParse(kShuZu[iIndex], out iIntValue))
                                        {
                                            byte[] idIntBytes = System.Text.UTF8Encoding.UTF8.GetBytes(kShuZu[iIndex]);
                                            if (idIntBytes.Length == 4)
                                            {
                                                iIntValue = 0
                                                            | ((Int32)idIntBytes[0] << 24)
                                                            | ((Int32)idIntBytes[1] << 16)
                                                           | ((Int32)idIntBytes[2] << 8)
                                                          | ((Int32)idIntBytes[3] << 0);
                                            }
                                        }

                                        iIndex++;
                                    }
                                    sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                                }
                            }
                        }                        
                    }
                    else if (MyExcel.kTypeClass[7] == kDefTypeData.kType[0])
                    {
                        float fFloatValue = -1.0f;

                        string[] kSplitShuZu = { "#", "\n" };
                        string[] kShuZu = kValue.ToString().Split(kSplitShuZu, StringSplitOptions.RemoveEmptyEntries);
                       
                        int iIndex = 0;
                        for (int n = 0; n < kDefTypeData.iLength; n++)
                        {                            
                            sb.Append(string.Format("{0}{1}=", kDefTypeData.kMemberName, n));
                            if (kValue.ToString() == "-")
                            {
                                sb.Append(string.Format("\"{0}\" ", fFloatValue.ToString()));
                            }
                            else
                            {
                                if(kShuZu.Length < 2)
                                {
                                    System.Windows.Forms.MessageBox.Show(
                                        string.Format("Error float format at {0}Row{1}Column", iRowIndex, MyExcel.s_RowIndex[i]));
                                }
                                if (n * 2 < kShuZu.Length && int.Parse(kShuZu[iIndex]) == n + 1)
                                {
                                    iIndex++;
                                    fFloatValue = float.Parse(kShuZu[iIndex]);
                                    iIndex++;
                                }
                                sb.Append(string.Format("\"{0}\" ", fFloatValue.ToString()));
                            }
                        }
                    }
                    else if (MyExcel.kTypeClass[8] == kDefTypeData.kType[0])
                    {
                        string[] kSplitShuZu = { "#", "\n" };
                        string[] kShuZu = kValue.ToString().Split(kSplitShuZu, StringSplitOptions.RemoveEmptyEntries);

                        if (kShuZu.Length <= kDefTypeData.iLength)
                        {
                            for (int n = 0; n < kDefTypeData.iLength; n++)
                            {
                                sb.Append(string.Format("{0}{1}=", kDefTypeData.kMemberName, n));
                                if (kValue.ToString() == "-")
                                {
                                    sb.Append(string.Format("\"{0}\" ", kValue.ToString()));
                                }
                                else
                                {
                                    if (n < kShuZu.Length)
                                    {
                                        sb.Append(string.Format("\"{0}\" ", kShuZu[n]));
                                    }
                                    else
                                    {
                                        sb.Append(string.Format("\"{0}\" ", "-"));
                                    }
                                }
                            }
                        }
                        else
                        {
                            int iIndex = 0;
                            for (int n = 0; n < kDefTypeData.iLength; n++)
                            {
                                sb.Append(string.Format("{0}{1}=", kDefTypeData.kMemberName, n));
                                if (kValue.ToString() == "-")
                                {
                                    sb.Append(string.Format("\"{0}\" ", kValue.ToString()));
                                }
                                else
                                {
                                    if (n * 2 < kShuZu.Length && int.Parse(kShuZu[iIndex]) == n + 1)
                                    {
                                        iIndex++;
                                        sb.Append(string.Format("\"{0}\" ", kShuZu[iIndex]));
                                        iIndex++;
                                    }
                                }
                            }
                        }                       
                    }
                    //else if (MyExcel.kTypeClass[9] == kDefTypeData.kType[0])
                    //{
                    //    int iIntValue = 0;
                    //    string kSrcStr = kValue.ToString();
                    //    if (kSrcStr.Contains(","))
                    //    {
                    //        string[] kSplitShuZu = { ",", "\n" };
                    //        string[] kShuZu = kValue.ToString().Split(kSplitShuZu, StringSplitOptions.RemoveEmptyEntries);
                    //        for (int n = 0; n < kDefTypeData.iLength; n++)
                    //        {
                    //            iIntValue = 0;
                    //            sb.Append(string.Format("{0}{1}=", kDefTypeData.kMemberName, n));
                    //            if (kValue.ToString() == "-")
                    //            {
                    //                sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                    //            }
                    //            else
                    //            {
                    //                if (n < kShuZu.Length && !int.TryParse(kShuZu[n], out iIntValue))
                    //                {
                    //                    byte[] idIntBytes = Encoding.UTF8.GetBytes(kShuZu[n]);
                    //                    if (idIntBytes.Length == 4)
                    //                    {
                    //                        iIntValue = 0
                    //                                    | ((Int32)idIntBytes[0] << 24)
                    //                                    | ((Int32)idIntBytes[1] << 16)
                    //                                   | ((Int32)idIntBytes[2] << 8)
                    //                                  | ((Int32)idIntBytes[3] << 0);
                    //                    }
                    //                }

                    //                sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                    //            }
                    //        }
                    //    }
                    //    else
                    //    {
                    //        string[] kSplitShuZu = { "#", "\n" };
                    //        string[] kShuZu = kValue.ToString().Split(kSplitShuZu, StringSplitOptions.RemoveEmptyEntries);

                    //        int iIndex = 0;
                    //        for (int n = 0; n < kDefTypeData.iLength; n++)
                    //        {
                    //            iIntValue = 0; 

                    //            sb.Append(string.Format("{0}{1}=", kDefTypeData.kMemberName, n));
                    //            if (kValue.ToString() == "-")
                    //            {
                    //                sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                    //            }
                    //            else
                    //            {
                    //                if (n * 2 < kShuZu.Length && int.Parse(kShuZu[iIndex]) == n + 1)
                    //                {
                    //                    iIndex++;

                    //                    if (!int.TryParse(kShuZu[iIndex], out iIntValue))
                    //                    {
                    //                        byte[] idIntBytes = System.Text.UTF8Encoding.UTF8.GetBytes(kShuZu[iIndex]);
                    //                        if (idIntBytes.Length == 4)
                    //                        {
                    //                            iIntValue = 0
                    //                                        | ((Int32)idIntBytes[0] << 24)
                    //                                        | ((Int32)idIntBytes[1] << 16)
                    //                                       | ((Int32)idIntBytes[2] << 8)
                    //                                      | ((Int32)idIntBytes[3] << 0);
                    //                        }
                    //                    }

                    //                    iIndex++;
                    //                }
                    //                sb.Append(string.Format("\"{0}\" ", iIntValue.ToString()));
                    //            }
                    //        }
                    //    }
                    //}
                }

                sb.AppendLine("/>");
            }

            sb.AppendLine("</root>");
        }
    }
}
