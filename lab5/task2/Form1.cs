using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Diamond_square.Primitives;
using System.Threading;

namespace Diamond_square
{
    public partial class Form1 : Form
    {
        private Graphics graphics;

        private Point2D startPoint = null;
        private Point2D endPoint = null;

        private List<Point2D> pointsForDrawing = new List<Point2D>();

        // Список точек для midpoint displacement
        LinkedList<Point2D> pointsList;

        Random rand = new Random();

        int millisecStep = 50;

        bool isStopped = false;

        private Primitive SelectedPrimitive
        {
            get
            {
                if (null == treeView1.SelectedNode) return null;
                var p = (Primitive)treeView1.SelectedNode.Tag;
                return p;
            }
            set
            {
                Redraw();
            }
        }

        public Form1()
        {
            InitializeComponent();
            pictureBox1.Image = new Bitmap(2048, 2048);
            graphics = Graphics.FromImage(pictureBox1.Image);
            graphics.Clear(Color.White);
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            MouseEventArgs mouseEvent = (MouseEventArgs)e;
            Point2D clickedPoint = Point2D.FromPoint(mouseEvent.Location);

            if (startPoint == null)
            {
                startPoint = clickedPoint;
                pointsForDrawing.Add(startPoint);
            }
            else if (endPoint == null)
            {
                endPoint = clickedPoint;
                pointsForDrawing.Add(endPoint);
            }
            else
            {
                pointsForDrawing.Clear();
                startPoint = null;
                endPoint = null;
            }

            Redraw();

            //TreeNode node = treeView1.Nodes.Add("Point (" + clickedPoint.X + ", " + clickedPoint.Y + ")");
            //node.Tag = clickedPoint;

            //pointsForDrawing.Add(clickedPoint);
            //Redraw();
        }

        private void Redraw()
        {
            graphics.Clear(Color.White);
            pointsForDrawing.ForEach((p)
                => p.Draw(graphics, p == SelectedPrimitive));
            pictureBox1.Invalidate();

        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {
            SelectedPrimitive = (Primitive)e.Node.Tag;
            Redraw();
        }

        private void treeView1_KeyDown(object sender, KeyEventArgs e)
        {
            if (Keys.Delete != e.KeyCode && Keys.Back != e.KeyCode) return;
            if (null == SelectedPrimitive) return;
            if (SelectedPrimitive is Point2D) pointsForDrawing.Remove((Point2D)SelectedPrimitive);

            treeView1.SelectedNode.Remove();
            if (null != treeView1.SelectedNode)
                SelectedPrimitive = (Primitive)treeView1.SelectedNode.Tag;
            else
                SelectedPrimitive = null;
            Redraw();
        }


        private void button1_Click(object sender, EventArgs e)
        {
            //var startPoint = (Point2D)SelectedPrimitive;

            float segmentLength;

            int maxRecDepth;

            double R;

            isStopped = false;

            if (startPoint == null)
            {
                MessageBox.Show("Выберите начальную точку");
                return;
            }

            if (endPoint == null)
            {
                MessageBox.Show("Выберите конечную точку");
                return;
            }

            if (!double.TryParse(textBox1.Text, out R))
            {
                MessageBox.Show("Неправильное значение шероховатости");
                return;
            }

            if (!int.TryParse(textBox3.Text, out maxRecDepth))
            {
                MessageBox.Show("Неправильная максимальная глубина рекурсии");
                return;
            }

            pointsList = new LinkedList<Point2D>();


            //var endPoint = new Point2D(startPoint.X + segmentLength, startPoint.Y);
            //endPoint.Draw(graphics);
            //pictureBox1.Refresh();

            //Thread.Sleep(millisecStep);

            pointsList.AddFirst(startPoint);
            pointsList.AddLast(endPoint);

            graphics.Clear(Color.White);

            (new Edge(startPoint, endPoint)).Draw(graphics);

            pictureBox1.Refresh();

            Thread.Sleep(millisecStep);

            var hl = pointsList.First;
            var hr = hl.Next;

            Thread t = new Thread(new ThreadStart(() => midpoint(hl, hr, R, maxRecDepth)  ));
            t.Start();

            DrawCurrentIteration();
        }

        private void midpoint(LinkedListNode<Point2D> leftPointNode, LinkedListNode<Point2D> rightPointNode, 
            double R, int currentIteration)
        {
            if (currentIteration == 0)
                return;

            var len = Math.Sqrt(
                (rightPointNode.Value.X - leftPointNode.Value.X) * (rightPointNode.Value.X - leftPointNode.Value.X)
                + (rightPointNode.Value.Y - leftPointNode.Value.Y) * (rightPointNode.Value.Y - leftPointNode.Value.Y)
                );

            var H = (leftPointNode.Value.Y + rightPointNode.Value.Y) / 2;

            var randPart = rand.Next((int)(-R * len), (int)(+R * len));

            H = (H + randPart) < 0 ? H : (H + randPart);

            var newPoint = new Point2D((rightPointNode.Value.X - leftPointNode.Value.X) / 2 + leftPointNode.Value.X, H);

            pointsList.AddAfter(leftPointNode, newPoint);

            if (Math.Abs(newPoint.X - leftPointNode.Value.X) < 1.0) return;

            this.Invoke( (MethodInvoker)delegate
            {
                if (isStopped) return;
                DrawCurrentIteration();
            });

            //Console.WriteLine("Iteration " + currentIteration + "  size:" + pointsList.Count + " length: " + len + "; " + newPoint.X + "; " + newPoint.Y + "rand: " + randPart);

            midpoint(leftPointNode, leftPointNode.Next, R, currentIteration - 1);
            midpoint(rightPointNode.Previous, rightPointNode, R, currentIteration - 1);
        }

        private HashSet<int> drawedItarations = new HashSet<int>();

        private void DrawCurrentIteration()
        {
            graphics.Clear(Color.White);
            var p = pointsList.First;
            while (p.Next != null)
            {
                var edge = new Edge(p.Value, p.Next.Value);
                edge.Draw(graphics);
                p = p.Next;
            }
            pictureBox1.Refresh();
            Thread.Sleep(millisecStep);
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button2_Click(object sender, EventArgs e)
        {
            isStopped = true;
        }
    }
}
