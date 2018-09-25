using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace task_2
{
	public partial class Form1 : Form
	{

        enum ColorType { RED, GREEN, BLUE };

		private static ColorMatrix redMatrix = new ColorMatrix(new float[][] {
				new float[] { 1, 0, 0, 0, 0 },
				new float[] { 0, 0, 0, 0, 0 },
				new float[] { 0, 0, 0, 0, 0 },
				new float[] { 0, 0, 0, 1, 0 },
				new float[] { 0, 0, 0, 0, 1 }
			});
   
		private static ColorMatrix greenMatrix = new ColorMatrix(new float[][] {
				new float[] { 0, 0, 0, 0, 0 },
				new float[] { 0, 1, 0, 0, 0 },
				new float[] { 0, 0, 0, 0, 0 },
				new float[] { 0, 0, 0, 1, 0 },
				new float[] { 0, 0, 0, 0, 1 }
			});
   
		private static ColorMatrix blueMatrix = new ColorMatrix(new float[][] {
				new float[] { 0, 0, 0, 0, 0 },
				new float[] { 0, 0, 0, 0, 0 },
				new float[] { 0, 0, 1, 0, 0 },
				new float[] { 0, 0, 0, 1, 0 },
				new float[] { 0, 0, 0, 0, 1 }
			});

        private Bitmap bitmap;

        public Form1()
		{
			InitializeComponent();
		}
   
		private static Bitmap ConvertColor(Bitmap source, ColorMatrix transformation)
		{
			Bitmap converted = new Bitmap(source.Width, source.Height);
			ImageAttributes attributes = new ImageAttributes();
			attributes.SetColorMatrix(transformation);
			Graphics graphics = Graphics.FromImage(converted);
			graphics.DrawImage(source, new Rectangle(0, 0, source.Width, source.Height),
				0, 0, source.Width, source.Height,
				GraphicsUnit.Pixel, attributes);
			graphics.Dispose();
			return converted;
		}
   
		private static Bitmap Histogram(Bitmap image, ColorType colorType)
		{
			int[] colorDistribution = new int[256];
			Bitmap histogram = new Bitmap(256, 256);
			Graphics graphics = Graphics.FromImage(histogram);
			int max = int.MinValue;
			for (int row = 0; row < image.Height; ++row)
				for (int col = 0; col < image.Width; ++col)
				{
					Color c = image.GetPixel(col, row);
                    int colorValue = 0;
                    switch(colorType)
                    {
                        case ColorType.RED:
                            colorValue = c.R;
                            break;
                        case ColorType.GREEN:
                            colorValue = c.G;
                            break;
                        case ColorType.BLUE:
                            colorValue = c.B;
                            break;
                          
                    }
					++colorDistribution[colorValue];
					if (colorDistribution[colorValue] > max)
                        max = colorDistribution[colorValue];
				}

			for (int i = 0; i < 255; ++i)
			{
                int distrCur = 255 - (int)(256 * colorDistribution[i] / (double)max);
                int distrNext = 255 - (int)(256 * colorDistribution[i + 1] / (double)max);
				graphics.DrawLine(Pens.Black, i, distrCur, i + 1, distrNext);
			}
			graphics.Dispose();
			return histogram;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			var dialog = new OpenFileDialog();
			if (DialogResult.OK != dialog.ShowDialog()) return;
            try
            {
                bitmap = new Bitmap(dialog.FileName);
            }
            catch
            {
                MessageBox.Show("Cannot open file");
                return;
            }

            pictureBox1.Image = bitmap;

        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (radioButton1.Checked)
            {
                var red = ConvertColor(bitmap, redMatrix);
                pictureBox2.Image = red;
                pictureBox3.Image = Histogram(red, ColorType.RED);
            }
            else if (radioButton2.Checked)
            {
                var green = ConvertColor(bitmap, greenMatrix);
                pictureBox2.Image = green;
                pictureBox3.Image = Histogram(green, ColorType.GREEN);
            }
            else if (radioButton3.Checked)
            {
                var blue = ConvertColor(bitmap, blueMatrix);
                pictureBox2.Image = blue;
                pictureBox3.Image = Histogram(blue, ColorType.BLUE);
            }
        }
    }
}
