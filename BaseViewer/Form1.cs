using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.IO;

namespace BaseViewer
{
    public partial class MainForm : Form
    {
        ReportDateTubes reportDateTubes;
        public MainForm()
        {
            reportDateTubes = new ReportDateTubes();
            InitializeComponent();
        }

        private void CloseBtn_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void OkBtn_Click(object sender, EventArgs e)
        {
            reportDateTubes.SetGrid(
                fromDateTimePicker.Value
                , toDateTimePicker.Value
                , dataGridView
                );
        }

        private void saveXlsBtn_Click(object sender, EventArgs e)
        {
            StringBuilder s = new StringBuilder();
            Directory.CreateDirectory("./xls/");
            s.AppendFormat("./xls/{0:s}_{1:s}.xls", fromDateTimePicker.Value, toDateTimePicker.Value);
            string z = Regex.Replace( s.ToString(), "[:]", "");
            z = Regex.Replace(z, "[T]", "_");
            if (reportDateTubes.ExportExcel(z))
            {
                MessageBox.Show("Данные сохранены", "Сообщение", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }
    }
}
