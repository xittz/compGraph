using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace l_systems
{
    public partial class Form1 : Form
    {

        string a, start;
        double angle;
        Dictionary<char, string> rules;
        List<Tuple<PointF, PointF>> states;
        Graphics g;

        public Form1()
        {
            InitializeComponent();
            rules = new Dictionary<char, string>();
            states = new List<Tuple<PointF, PointF>>();
            pictureBox1.Image = new Bitmap(pictureBox1.Width, pictureBox1.Height);
            g = Graphics.FromImage(pictureBox1.Image);
        }

        private void openFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var openDialog = new OpenFileDialog();
            openDialog.ShowDialog();
            var file = openDialog.FileName;

            rules = new Dictionary<char, string>();
            var flines = File.ReadAllLines(file).Where(a => !string.IsNullOrWhiteSpace(a)).ToArray();
            var args = flines[0].Split(' ');

            a = args[0];
            angle = (float.Parse(args[1])) * Math.PI / 180;
            start = args[2];

            foreach (var rl in flines)
            {
                if (rl.Contains(">"))
                {
                    var rule = rl.Split('>');
                    rules[char.Parse(rule[0])] = rule[1];
                }
            }
        }

        int it;
        void draw()
        {
            g.Clear(Color.White);
            string prev, lines = a;
            float x1 = 0, y1 = 0;
            float x2 = 0, y2 = 0;
            for (int i = 0; i < it; i++)
            {
                prev = lines;
                lines = "";
                foreach (var p in prev)
                    lines += rules.ContainsKey(p) ? rules[p] : p.ToString();
            }

            var res = new List<Tuple<PointF, PointF>>();

            if (start == "L")
            {
                x1 = pictureBox1.Width;
                y1 = pictureBox1.Height / 2;
                x2 = -pictureBox1.Width / 2;
            }
            else if (start == "U")
            {
                x1 = pictureBox1.Width / 2;
                y1 = pictureBox1.Height / 2;
                y2 = -pictureBox1.Height / 2;
            }
            else if (start == "R")
            {
                y1 = pictureBox1.Height / 2;
                x2 = pictureBox1.Width / 2;
            }
            else if (start == "D")
            {
                x1 = pictureBox1.Width / 2;
                y2 = pictureBox1.Height / 2;
            }

            var xs = new List<float>();
            var ys = new List<float>();

            xs.Add(x1);
            ys.Add(y1);

            foreach (var l in lines)
                if (l == '[')
                    states.Add(Tuple.Create(new PointF(x1, y1), new PointF(x2, y2)));
                else if (l == ']')
                {
                    var coords = states[states.Count - 1];
                    states.RemoveAt(states.Count - 1);
                    x1 = coords.Item1.X;
                    y1 = coords.Item1.Y;

                    x2 = coords.Item2.X;
                    y2 = coords.Item2.Y;
                }
                else if (l == '-')
                {
                    var temp = x2;
                    x2 = (float)(temp * Math.Cos(angle) + y2 * Math.Sin(angle));
                    y2 = (float)(-temp * Math.Sin(angle) + y2 * Math.Cos(angle));
                }
                else if (l == '+')
                {
                    var temp = x2;
                    x2 = (float)(temp * Math.Cos(angle) - y2 * Math.Sin(angle));
                    y2 = (float)(temp * Math.Sin(angle) + y2 * Math.Cos(angle));
                }
                else
                {
                    res.Add(Tuple.Create(new PointF(x1, y1), new PointF(x1 + x2, y1 + y2)));
                    x1 += x2;
                    y1 += y2;
                    xs.Add(x1);
                    ys.Add(y1);
                }

            var scale = Math.Max(xs.Max() - xs.Min(), ys.Max() - ys.Min());
            foreach (var r in res)
                g.DrawLine(Pens.Black, (xs.Max() - r.Item1.X) / scale * pictureBox1.Width, (ys.Max() - r.Item1.Y) / scale * pictureBox1.Height,
                                      (xs.Max() - r.Item2.X) / scale * pictureBox1.Width, (ys.Max() - r.Item2.Y) / scale * pictureBox1.Height);
        }

        private void button1_Click_1(object sender, EventArgs e)
        {
            it = int.Parse(textBox1.Text);
            draw();
            pictureBox1.Invalidate();
        }
    }
}
