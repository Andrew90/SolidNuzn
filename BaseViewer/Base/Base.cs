using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
       
namespace BaseViewer
{
    class Base
    {
        public System.Data.OleDb.OleDbConnection conn;
       
        public bool Open(String path)
        {
            String udl = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source="
                  + path
                  + ";Mode=ReadWrite;Persist Security Info=False";
            try
            {
                conn = new System.Data.OleDb.OleDbConnection(udl);
                conn.Open();
                return true;
            }
            catch
            {
                return false;
            }
        }
       
        public bool IsOpen()
        {
           return null != conn && System.Data.ConnectionState.Open == conn.State;
        }
        public void Close()
        {
            if(IsOpen())
            {
                conn.Close();
            }
        }
    }
}
