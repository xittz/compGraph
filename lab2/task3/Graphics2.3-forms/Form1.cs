using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Graphics2._3_forms
{
    public partial class Form1 : Form
    {
        Bitmap sourceBitmap;
        Bitmap bitmap;

        double H = 0.0, S = 0.0, V = 0.0;

        public Form1()
        {
            InitializeComponent();
        }

        public static void ToHSV(Color color, out double hue, out double saturation, out double value)
        {
            int red = color.R;
            int green = color.G;
            int blue = color.B;

            int maxRGB = Math.Max(red, Math.Max(green, blue));
            int minRGB = Math.Min(red, Math.Min(green, blue));

            int diff = maxRGB - minRGB;

            // calc hue
            if (diff == 0)
                hue = 0;
            else if (maxRGB == red)
            {
                if (green >= blue)
                    hue = (60 * (green - blue)) / diff;
                else
                    hue = ((60 * (green - blue)) / diff) + 360;
            }
            else
            {
                if (maxRGB == green)
                    hue = ((60 * (blue - red)) / diff) + 120;
                else
                    hue = ((60 * (red - green)) / diff) + 240;
            }
            //

            saturation = (maxRGB == 0) ? 0 : 1d - (1d * minRGB / maxRGB);

            value = maxRGB / 255d;

        }


        public static Color FromHSV(double hue, double saturation, double value)
        {

            int hi = Convert.ToInt32(Math.Floor(hue / 60)) % 6;
            double f = hue / 60 - Math.Floor(hue / 60);

            value = value * 255;
            int v = Convert.ToInt32(value);
            int vMin = Convert.ToInt32(value * Math.Max(0, (1 - saturation)));
            int a = Convert.ToInt32((v - vMin) * ((hue % 60) / 60));
            int vDec = v - a;
            int vInc = vMin + a;

            if (hi == 0)
                return Color.FromArgb(255, v, vInc, vMin);
            else if (hi == 1)
                return Color.FromArgb(255, vDec, v, vMin);
            else if (hi == 2)
                return Color.FromArgb(255, vMin, v, vInc);
            else if (hi == 3)
                return Color.FromArgb(255, vMin, vDec, v);
            else if (hi == 4)
                return Color.FromArgb(255, vInc, vMin, v);
            else
                return Color.FromArgb(255, v, vMin, vDec);
        }

        private void updatePictureBox()
        {
            if (bitmap == null) return;

            for (int x = 0; x < sourceBitmap.Width; ++x)
                for (int y = 0; y < sourceBitmap.Height; ++y)
                {
                    Color color = sourceBitmap.GetPixel(x, y);
                    double h, s, v;
                    ToHSV(color, out h, out s, out v);

                    double newS = Math.Max(0, Math.Min(1, s + this.S / 100));

                    double newV = Math.Max(0, Math.Min(1, v + this.V / 100));

                    Color newColor = FromHSV(h + H, newS, newV);

                    bitmap.SetPixel(x, y, newColor);
                }

            pictureBox1.Image = bitmap;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            openFileDialog1.ShowDialog();
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            
            label1.Text = "Selected " + openFileDialog1.SafeFileName;

            bitmap = new Bitmap(openFileDialog1.FileName);
            sourceBitmap = new Bitmap(openFileDialog1.FileName);

            for(int x = 0; x < bitmap.Width; ++x)
                for(int y = 0; y < bitmap.Height; ++y)
                {
                    Color color = bitmap.GetPixel(x, y);
                    double h, s, v;
                    ToHSV(color, out h, out s, out v);
                    Color resultColor = FromHSV(h, s, v);
                    bitmap.SetPixel(x, y, resultColor);
                    sourceBitmap.SetPixel(x, y, resultColor);

                }

            
            pictureBox1.Width = bitmap.Width;
            pictureBox1.Height = bitmap.Height;
            pictureBox1.Image = bitmap;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            //bitmap.Save(openFileDialog1.FileName + "-toHSV.jpg");
            saveFileDialog1.ShowDialog();
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e)
        {
            this.S = Convert.ToDouble(numericUpDown2.Value);
            this.updatePictureBox();

        }

        private void numericUpDown3_ValueChanged(object sender, EventArgs e)
        {
            this.V = Convert.ToDouble(numericUpDown3.Value);
            this.updatePictureBox();
        }

        private void saveFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            bitmap.Save(saveFileDialog1.FileName);
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            this.H = Convert.ToDouble(numericUpDown1.Value);
            this.updatePictureBox();
        }
    }
}
