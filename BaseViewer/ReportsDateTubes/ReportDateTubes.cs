using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data;
using System.Xml;
using System.Globalization;

namespace BaseViewer
{
    class ReportDateTubes
    {
        DataTable table;
        DataSet ds;

        public ReportDateTubes()
        {
            ds = new DataSet();
        }

        public int CountRows()
        {
            if (null != table) return table.Rows.Count;
            return 0;
        }

        public bool ExportExcel(string path)
        {
            if (null != table && 0 != table.Rows.Count)
            {                               

                ds.WriteXml(path);
                return true;
            }
            return false;
        }

        public void SetGrid(DateTime from, DateTime to, System.Windows.Forms.DataGridView grid)
        {
            var files = Global.BaseFiles(from, to);
            if (null == files || 0 == files.Count()) return;
            Base b = new Base();
            table = new DataTable();
            ds.Tables.Add(table);
            table.Columns.Add().DataType = typeof(DateTime);
            table.Columns.Add().DataType = typeof(int);
            table.Columns.Add().DataType = typeof(string);
            table.Columns.Add().DataType = typeof(string);

            table.Columns[0].ColumnName = "Время";
            table.Columns[1].ColumnName = "Номер трубы";
            table.Columns[2].ColumnName = "Диаметр трубы";
            table.Columns[3].ColumnName = "Группа прочности";

            foreach(var path in files)
            {
                if(b.Open(path))
                {
                    System.Data.OleDb.OleDbCommand com = b.conn.CreateCommand();

                    System.Data.OleDb.OleDbParameter from_ = new System.Data.OleDb.OleDbParameter("", System.Data.OleDb.OleDbType.Date);//DBTimeStamp);
                    from_.Value = from;
                    com.Parameters.Add(from_);

                    System.Data.OleDb.OleDbParameter to_ = new System.Data.OleDb.OleDbParameter("", System.Data.OleDb.OleDbType.Date);//DBTimeStamp);
                    to_.Value = to;
                    com.Parameters.Add(to_);

                    com.CommandText = 
                        "SELECT t.CurrentTime, t.CurrentTubes, t.Diameters, t.GroupStrength"
                        + " FROM TubesTable as t"
                        + " WHERE t.CurrentTime>=? and t.CurrentTime<?"
                        ;
                    System.Data.OleDb.OleDbDataReader reader = com.ExecuteReader();

                    while (reader.Read())
                    {
                        table.Rows.Add(
                              (DateTime)reader[0]
                              , (int   )reader[1]
                              , (string)reader[2]
                              , (string)reader[3]
                            );
                    }
                }
            }
            if (0 != table.Rows.Count)
            {
                grid.DataSource = table;
                grid.Columns[0].Width = 100;
                grid.Columns[1].Width = 100;
                grid.Columns[2].Width = 70;
                grid.Columns[3].Width = 70;
                for (int i = 0; i < grid.ColumnCount; ++i)
                {
                    grid.Columns[i].ReadOnly = true;
                }
                foreach (DataTable dt in ds.Tables)
                {
                    foreach (DataColumn dc in dt.Columns)
                    {
                        if (dc.DataType == typeof(DateTime))
                        {
                            dc.DateTimeMode = DataSetDateTime.Unspecified;
                        }
                    }
                }
            }
            grid.Update();            
        }
    }
}
