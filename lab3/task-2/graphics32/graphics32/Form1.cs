using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace graphics32
{
    public partial class Form1 : Form
    {

        public Bitmap bitmap = null;
        public Bitmap newBitmap = null;

        public Form1()
        {
            InitializeComponent();
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {

            bitmap = new Bitmap(openFileDialog1.FileName);
            pictureBox1.Image = bitmap;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //openFileDialog1.ShowDialog();
            bitmap = new Bitmap("contur1.png");
            for(int x = 1; x < bitmap.Width; ++x)
                for (int y = 1; y < bitmap.Height; ++y)
                {
                    Color c = bitmap.GetPixel(x, y);
                    bitmap.SetPixel(x, y, (c.R < 100) ? Color.Black : Color.White);
                }
            pictureBox1.Image = bitmap;

        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            if (bitmap == null) return;
            MouseEventArgs mouseEvent = (MouseEventArgs) e;

            label1.Text = "(" + mouseEvent.X + "; " + mouseEvent.Y + ")";

            startPoint = new Point(mouseEvent.X, mouseEvent.Y);

            if(bitmap.GetPixel(startPoint.X, startPoint.Y) == bgColor) return;

            scan(mouseEvent.X, mouseEvent.Y);

            pictureBox1.Image = new Bitmap(newBitmap, new Size(bitmap.Width, bitmap.Height));
        }

        List<Point> conturList;

        Point startPoint;

        Color bgColor = Color.White;

        Color conturColor;

        Point downDirection = new Point(0, 1);
        Point downLeftDirection = new Point(-1, 1);
        Point leftDirection = new Point(-1, 0);
        Point leftUpDirection = new Point(-1, -1);
        Point upDirection = new Point(0, -1);
        Point upRightDirection = new Point(1, -1);
        Point rightDirection = new Point(1, 0);
        Point downRightDirection = new Point(1, 1);

        private Point rotate(Point point)
        {
            if (point == downDirection)
                return downLeftDirection;
            else if (point == downLeftDirection)
                return leftDirection;
            else if (point == leftDirection)
                return leftUpDirection;
            else if (point == leftUpDirection)
                return upDirection;
            else if (point == upDirection)
                return upRightDirection;
            else if (point == upRightDirection)
                return rightDirection;
            else if (point == rightDirection)
                return downRightDirection;
            else
                return downDirection;
        }

        private Point pointByDirection(Point point, Point direction )
        {
            return new Point(point.X + direction.X, point.Y + direction.Y);
        }

        private bool isContur(Point point, Point direction)
        {
            if (bitmap.GetPixel(point.X, point.Y) != conturColor) return false;
            
            Point n = rotate(direction);
            Point n2 = rotate(n);

            int res = 0;

            if (bitmap.GetPixel(point.X + direction.X, point.Y + direction.Y) != conturColor) ++res;
            if (bitmap.GetPixel(point.X + n.X, point.Y + n.Y) != conturColor) ++res;
            if (bitmap.GetPixel(point.X + n2.X, point.Y + n2.Y) != conturColor) ++res;
            return res > 0;
        }

        private void scan(int clickX, int clickY)
        {
            bgColor = bitmap.GetPixel(1, 1);
            newBitmap = new Bitmap(bitmap);
            Point direction = downDirection;
            Point nextDirection = rotate(direction);

            Point currentPoint = startPoint;

            conturColor = bitmap.GetPixel(clickX, clickY);

            int counter = 0;
            int pixCounter = 0;

            Point p = currentPoint;
            Point prevP = p;

            while(bitmap.GetPixel(p.X + nextDirection.X, p.Y + nextDirection.Y) == conturColor)
            {
                prevP = p;
                p = pointByDirection(p, nextDirection);
            }

            startPoint = p;
            currentPoint = startPoint;

            //Console.WriteLine("" + currentPoint.X + "; " + currentPoint.Y);

            while (true)
            {
                //if (counter > 40000) break;

                newBitmap.SetPixel(currentPoint.X, currentPoint.Y, Color.YellowGreen);
                newBitmap.SetPixel(currentPoint.X - 1, currentPoint.Y, Color.YellowGreen);
                newBitmap.SetPixel(currentPoint.X + 1, currentPoint.Y, Color.YellowGreen);
                newBitmap.SetPixel(currentPoint.X, currentPoint.Y + 1, Color.YellowGreen);
                newBitmap.SetPixel(currentPoint.X, currentPoint.Y - 1, Color.YellowGreen);

                Point checkPoint = pointByDirection(currentPoint, direction);
                if (checkPoint == startPoint) break;
                //Console.WriteLine("" + checkPoint.X + "; " + checkPoint.Y + ": " + bitmap.GetPixel(checkPoint.X, checkPoint.Y).ToString());
                if (!isContur(checkPoint, nextDirection))
                {
                    //Console.WriteLine("S!" + !isContur(checkPoint, nextDirection) + "   " + (bitmap.GetPixel(checkPoint.X, checkPoint.Y) == bgColor));
                    direction = rotate(direction);
                    nextDirection = rotate(direction);
                    //Console.WriteLine("Change direction " + direction.X + "  " + direction.Y);
                } else
                {
                    currentPoint = checkPoint;
                    ++pixCounter;
                    
                }
                ++counter;
            }

            Console.WriteLine("Changed " + pixCounter + " pixels");
            



        }
    }
}
