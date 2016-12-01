using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;
using System.Windows.Forms;

namespace BaseViewer
{
    public class Global
    {
        public static String path
        {
            get { return "..\\..\\..\\Data\\"; } // отладка 
            //get { return "..\\Data\\"; } //TODO РАБОТА
        }
        private static void CreateBase(Base b, String path)
        {
            String udl = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source="
                  + path
                  + ";Mode=ReadWrite;Persist Security Info=False";

            ADOX.Catalog cat = new ADOX.Catalog();

            cat.Create(
                   "Provider=Microsoft.Jet.OLEDB.4.0;Data Source="
                   + path
                   );
            b.conn = new System.Data.OleDb.OleDbConnection(udl);
            b.conn.Open();
            System.Data.OleDb.OleDbCommand com = b.conn.CreateCommand();
            com.CommandText = "create table Common(Path VARCHAR(255))";

            com.ExecuteNonQuery();
        }
        public static IEnumerable<string> BaseFiles(DateTime from, DateTime to)
        {
            try
            {
                string[] files = Directory.GetFiles(Global.path, @"*.mdb");

                IEnumerable<string> x = from n in files
                                        where System.Text.RegularExpressions.Regex.IsMatch(n, @"\d{4}\.mdb$")
                                        select n
                                        ;
                int iFrom = int.Parse(from.ToString("yyMM"));
                int iTo = int.Parse(to.ToString("yyMM"));

                var s = int.Parse(System.Text.RegularExpressions.Regex.Match(x.First<string>(), @"(\d{4})\.mdb$").Groups[1].ToString());

                var z = from n in x
                        let value = int.Parse((Regex.Match(n, @"(\d{4})\.mdb$").Groups[1]).ToString())
                        where value >= iFrom && value <= iTo
                        select n
                    ;
                return z;
            }
            catch(System.Exception)
            {
                const string message = "Необходимо определить путь к базе данных";
                const string caption = "Предупреждение";
                var result = MessageBox.Show(message, caption,
                                             MessageBoxButtons.OK,
                                             MessageBoxIcon.Warning);
                return null;
            }
        }
    };
}
