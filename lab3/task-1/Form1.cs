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

namespace task_1
{
    public partial class Form1 : Form
    {
        private Graphics g;
        private Graphics g2;
        private Bitmap image;
        private Bitmap image2;
        private Color paletteColor;
        private Color curPixel;
        private int xG, yG;
        

        public Form1()
        {
            InitializeComponent();
            paletteColor = Color.White;
        }

        // загрузка картинки
        private void loadImageToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openDialog = new OpenFileDialog();
            openDialog.Filter = "Image Files(*.BMP;*.JPG;*.JPEG;*.GIF;*.PNG)|*.BMP;*.JPG;*.JPEG;*.GIF;*.PNG|All files (*.*)|*.*"; //формат загружаемого файла
            if (openDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    image = new Bitmap(openDialog.FileName);
                    pictureBox1.Image = image;
                    g = Graphics.FromImage(pictureBox1.Image);
                    pictureBox1.Invalidate();
                    Form1.ActiveForm.Width = image.Width + 70;
                    Form1.ActiveForm.Height = image.Height + menuStrip1.Height + panel1.Height;
                }
                catch
                {
                    DialogResult rezult = MessageBox.Show("Невозможно открыть выбранный файл",
                    "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        // Выбор цвета на палитре
        private void paletteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                paletteColor = colorDialog1.Color;
            }
        }

        // Заливка цветом
        private void paint(int x, int y, Color curColor, Color newColor)
        {
            if (image.GetPixel(x, y) == newColor || image.GetPixel(x, y) != curColor)
                return;
            
            int leftX = x - 1, rightX = x + 1;

            Color pix;

            // Цвет правого пикселя
            if (rightX != image.Width)
            {
                pix = image.GetPixel(rightX, y);

                while (rightX < image.Width && pix == curColor)
                {
                    rightX++;
                    if (rightX < image.Width)
                        pix = image.GetPixel(rightX, y);
                }
            }


            // Цвет левого пикселя
            if (leftX != -1)
            {
                pix = image.GetPixel(leftX, y);

                while (leftX >= 0 && pix == curColor)
                {
                    leftX--;
                    if (leftX >= 0)
                        pix = image.GetPixel(leftX, y);
                }
            }

            rightX--;
            leftX++;

            Pen pen = new Pen(newColor);
            g.DrawLine(pen, leftX, y, rightX, y);

            for (int i = leftX; i < rightX; ++i)
            {
                if (y < image.Height - 1)
                    paint(i, y + 1, curColor, newColor);
              
                if (y > 0)
                    paint(i, y - 1, curColor, newColor);
            }
        }


        // Выбрать пиксель, с которого начнется заливка
        private void pictureBox1_MouseClick(object sender, MouseEventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                xG = e.Location.X;
                yG = e.Location.Y;
                curPixel = (pictureBox1.Image as Bitmap).GetPixel(e.Location.X, e.Location.Y);
                label2.Text = yG.ToString();
            }
        }
               
        // Загрузка шаблона
        private void templateToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openDialog = new OpenFileDialog();
            openDialog.Filter = "Image Files(*.BMP;*.JPG;*.JPEG;*.GIF;*.PNG)|*.BMP;*.JPG;*.JPEG;*.GIF;*.PNG|All files (*.*)|*.*"; //формат загружаемого файла
            if (openDialog.ShowDialog() == DialogResult.OK)
            {
                try
                {
                    image2 = new Bitmap(openDialog.FileName);
                    pictureBox2.Image = image2;
                    g2 = Graphics.FromImage(pictureBox2.Image);
                    pictureBox2.Invalidate();
                }
                catch
                {
                    DialogResult rezult = MessageBox.Show("Невозможно открыть выбранный файл",
                    "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            }
        }

        // Вызвать заливку цветом или шаблоном
        private void button1_MouseClick(object sender, MouseEventArgs e)
        {
            if (pictureBox1.Image != null)
            {
                if (radioButton1.Checked && curPixel.ToArgb() != paletteColor.ToArgb())
                {
                    paint(xG, yG, curPixel, paletteColor);
                    pictureBox1.Refresh();
                }
                else if (radioButton2.Checked)
                {
                    if (pictureBox2.Image == null)
                        MessageBox.Show("Выберите шаблон");
                    else
                    {
                        paintTemplate(xG, yG, 0, curPixel);
                        pictureBox1.Refresh();
                    }
                }
            }
        }
        

        // Заливка шаблоном
        private void paintTemplate(int xImg, int yImg, int yTemplate, Color oldColor)
        {
            //pictureBox1.Refresh();

            int xLeft = xImg - 1, xRight = xImg + 1;

            if (image.GetPixel(xImg, yImg).ToArgb() != oldColor.ToArgb())
                return;
            
            Color pix;

            // Цвет левого пикселя
            if (xLeft != -1)
            {
                pix = image.GetPixel(xLeft, yImg);

                while (xLeft >= 0 && pix == oldColor)
                {
                    xLeft--;
                    if (xLeft >= 0)
                        pix = image.GetPixel(xLeft, yImg);
                }
            }

            pix = image.GetPixel(xRight, yImg);

            // Цвет правого пикселя
            if (xRight != image.Width)
            {
                pix = image.GetPixel(xRight, yImg);

                while (xRight < image.Width && pix == oldColor)
                {
                    xRight++;
                    if (xRight < image.Width)
                        pix = image.GetPixel(xRight, yImg);
                }
            }
            

            // --- рисуем линию из пикселей шаблона ---
            int newX;
            newX = image2.Width - (xG - xLeft) % image2.Width;

            if (yTemplate == -1)
                yTemplate = image2.Height - 1;
            ++xLeft;
            --xRight;

            for (int i = xLeft; i < xRight; ++i)
            {
                image.SetPixel(i, yImg, image2.GetPixel((image2.Width / 2 + newX++) % image2.Width, /*(image2.Height / 2 + yTemplate) % image2.Height*/ yTemplate % image2.Height));
            }

            // ---

            for (int i = xLeft; i < xRight; ++i)
            {
                if (yImg < pictureBox1.Image.Height - 1)
                    paintTemplate(i, yImg + 1, yTemplate + 1, oldColor);
                if (yImg > 0)
                    paintTemplate(i, yImg - 1, yTemplate - 1, oldColor);
            }

        }
        
        // Выбрать пиксель, с которого начнется рисование карандашом
        private void pictureBox1_MouseDown(object sender, MouseEventArgs e)
        {
            xG = e.Location.X;
            yG = e.Location.Y;
        }

        // Процесс рисования карандашом
        private void pictureBox1_DrawLine(MouseEventArgs e)
        {
            g.DrawLine(new Pen(paletteColor, trackBar1.Value), new Point(xG, yG), e.Location);
            //g.DrawEllipse(new Pen(paletteColor, 10), xG, yG, 1, 1);
            xG = e.Location.X;
            yG = e.Location.Y;
            pictureBox1.Refresh();

        }

        private void pictureBox1_MouseMove(object sender, MouseEventArgs e)
        {
            if (pictureBox1.Image != null && e.Button == MouseButtons.Left)
                pictureBox1_DrawLine(e);
        }
        




    }
}
