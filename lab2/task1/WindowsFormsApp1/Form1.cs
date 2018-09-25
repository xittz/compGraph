using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

        }

        private Bitmap image1 = new Bitmap("fruct.jpg");

        private void drawSepia(PictureBox p, double k1, double k2, double k3)
        {
            Bitmap image2 = new Bitmap(image1.Width, image1.Height);
            double sum = 0;

            for (int i = 0; i < image1.Width; i++)
                for (int j = 0; j < image1.Height; j++)
                {
                    sum = k1 * image1.GetPixel(i, j).R +
                            k2 * image1.GetPixel(i, j).G +
                            k3 * image1.GetPixel(i, j).B;

                    image2.SetPixel(i, j, Color.FromArgb((byte)sum, (byte)sum, (byte)sum));
                }

            p.SizeMode = PictureBoxSizeMode.StretchImage;
            p.BorderStyle = BorderStyle.Fixed3D;
            p.Image = image2;
        }

        double[] saturationArray(Bitmap p)
        {
            Bitmap image2 = new Bitmap(p.Width, p.Height);
            double[] arr = new double[256];

            for (int i = 0; i < p.Size.Width; i++)
                for (int j = 0; j < p.Size.Height; j++)
                    arr[p.GetPixel(i, j).R] += 1;

            return arr;
        }

        private void drawDifference(PictureBox th, Bitmap a, Bitmap b)
        {

            Bitmap image2 = new Bitmap(a.Width, a.Height);

            for (int i = 0; i < a.Width; i++)
                for (int j = 0; j < b.Height; j++)
                {
                    int R = Math.Abs(a.GetPixel(i, j).R - b.GetPixel(i, j).R);
                    //int G = (a.GetPixel(i, j).G - b.GetPixel(i, j).G);
                    //int B = (a.GetPixel(i, j).B - b.GetPixel(i, j).B);

                    image2.SetPixel(i, j, Color.FromArgb((byte)R, (byte)R, (byte)R));
                }


            th.SizeMode = PictureBoxSizeMode.StretchImage;
            th.BorderStyle = BorderStyle.Fixed3D;
            th.Image = image2;
        }

        private void histogramm(double[] sut, PictureBox p)
        {

            double max = sut.Max();

            Bitmap image3 = new Bitmap(p.Size.Width, p.Size.Height);
            p.Image = image3;
            Graphics g = Graphics.FromImage(image3);


            for (int i = 0; i < 256; i++)
            {
                int h1 = (int)(sut[i] * (p.Size.Height - 20) / max);
                Rectangle rect = new Rectangle(20 + i, p.Size.Height - h1 - 20, 1, h1);
                g.DrawRectangle(new Pen(Color.Red, .5f), rect);
            }

        }

        private void Form1_Load(object sender, EventArgs e)
        {

            pictureBox1.SizeMode = PictureBoxSizeMode.StretchImage;
            pictureBox1.BorderStyle = BorderStyle.Fixed3D;
            pictureBox1.Image = image1;

            drawSepia(pictureBox2, 0.33, 0.33, 0.33);
            drawSepia(pictureBox3, 0.21, 0.72, 0.07);

            drawDifference(pictureBox4, (Bitmap)pictureBox3.Image, (Bitmap)pictureBox2.Image);

            histogramm(saturationArray((Bitmap)pictureBox2.Image), pictureBox5);
            histogramm(saturationArray((Bitmap)pictureBox3.Image), pictureBox6);
        }
    }
}
