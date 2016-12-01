сделал возможность смены каталога базы данных из меню

Open a new Visual C# .NET console application.
In Solution Explorer, right-click the References node and select Add Reference.
On the COM tab, select Microsoft ADO Ext. 2.7 for DDL and Security, click Select to add it to the Selected Components, and then click OK.
Delete all of the code from the code window for Class1.cs.
Paste the following code into the code window:
using System;
using ADOX;

namespace ConsoleApplication1
{
	class Class1
	{
		[STAThread]
		static void Main(string[] args)
		{
			ADOX.CatalogClass cat = new ADOX.CatalogClass();

			cat.Create("Provider=Microsoft.Jet.OLEDB.4.0;" +
				   "Data Source=D:\\AccessDB\\NewMDB.mdb;" +
				   "Jet OLEDB:Engine Type=5");

			Console.WriteLine("Database Created Successfully");

			cat = null;

		}
	}
}
D:\projects\project\Reports\base