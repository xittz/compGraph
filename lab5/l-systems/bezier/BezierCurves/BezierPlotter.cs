using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Windows.Input;

namespace BezierCurves
{
    public class CPoint
    {
        private PointF point = new PointF(0, 0);
        public float X { get { return point.X; } set { point.X = value; } }
        public float Y { get { return point.Y; } set { point.Y = value; } }
        public CPoint() { }
        public CPoint(PointF p) { point = p; }
        public CPoint(float x, float y) { X = x; Y = y; }
        public static implicit operator CPoint(PointF p) { return new CPoint(p); }
        public static implicit operator PointF(CPoint p) { return p.point; }
        public static implicit operator CPoint(Point p) { return new CPoint(p); }
        public static CPoint operator *(CPoint a, float f) { return new CPoint(a.X * f, a.Y * f); }
        public static CPoint operator *(float f, CPoint a) { return a * f; }
        public static CPoint operator +(CPoint a, CPoint b) { return new CPoint(a.X + b.X, a.Y + b.Y); }
        public static CPoint operator -(CPoint a, CPoint b) { return a + (-1) * b; }
    }

    public class BezierPlotter : UserControl
    {
        protected interface IState
        {
            void OnMouseDown(MouseEventArgs e);
            void OnMouseUp(MouseEventArgs e);
            void OnMouseMove(MouseEventArgs e);
        }

        protected abstract class BaseState : IState
        {
            protected BezierPlotter plotter;

            public BaseState(BezierPlotter plotter) { this.plotter = plotter; }

            public virtual void OnMouseDown(MouseEventArgs e) { }
            public virtual void OnMouseMove(MouseEventArgs e) { }
            public virtual void OnMouseUp(MouseEventArgs e) { }
        }

        protected class InitialState : BaseState
        {
            private float SELECTION_RADIUS = 12;

            public InitialState(BezierPlotter plotter) : base(plotter) { }

            private int FindNearestPoint(CPoint c)
            {
                var minDistance = float.MaxValue;
                var minIndex = -1;
                for (int i = 0; i < plotter.points.Count; ++i)
                {
                    var p = plotter.points[i];
                    var distance = Math.Abs(p.X - c.X) + Math.Abs(p.Y - c.Y);
                    if (minDistance < distance) continue;
                    minDistance = distance;
                    minIndex = i;
                }
                return minIndex;
            }

            public override void OnMouseDown(MouseEventArgs e)
            {
                if (ModifierKeys.HasFlag(Keys.Control) || ModifierKeys.HasFlag(Keys.Shift))
                {
                    if (plotter.points.Count == 0) return;
                    int nearestPointIndex = FindNearestPoint(e.Location);
                    var nearestPoint = plotter.points[nearestPointIndex];
                    if (SELECTION_RADIUS < Math.Abs(nearestPoint.X - e.X) ||
                        SELECTION_RADIUS < Math.Abs(nearestPoint.Y - e.Y))
                        return;
                    if (ModifierKeys.HasFlag(Keys.Shift))
                        plotter.points.RemoveRange(nearestPointIndex / 3 * 3, 3);
                    else
                        plotter.currentState = new MovingNodeState(plotter, nearestPointIndex);
                }
                else
                {
                    for (int i = 0; i < 3; ++i)
                        plotter.points.Add(e.Location);
                    plotter.currentState = new MovingNodeState(plotter, plotter.points.Count - 1);
                }
                plotter.Invalidate();
            }
        }

        protected class MovingNodeState : BaseState
        {
            private int nodeIndex;

            public MovingNodeState(BezierPlotter plotter, int nodeIndex) : base(plotter)
            {
                this.nodeIndex = nodeIndex;
            }

            public override void OnMouseMove(MouseEventArgs e)
            {
                if (1 == nodeIndex % 3)
                {
                    var delta = e.Location - plotter.points[nodeIndex];
                    plotter.points[nodeIndex - 1] += delta;
                    plotter.points[nodeIndex    ] += delta;
                    plotter.points[nodeIndex + 1] += delta;
                }
                else
                {
                    var oppositeArmIndex = 0 == nodeIndex % 3 ? nodeIndex + 2 : nodeIndex - 2;
                    var delta = e.Location - plotter.points[nodeIndex];
                    plotter.points[nodeIndex       ] += delta;
                    plotter.points[oppositeArmIndex] -= delta;
                }
                plotter.Invalidate();
            }

            public override void OnMouseUp(MouseEventArgs e)
            {
                plotter.currentState = new InitialState(plotter);
            }
        }

        private int NUMBER_OF_STEPS = 20;

        protected List<CPoint> points = new List<CPoint>();
        protected IState currentState;
        private bool showGuideLines = true;

        public BezierPlotter() : base()
        {
            currentState = new InitialState(this);
            var flags = ControlStyles.AllPaintingInWmPaint 
                      | ControlStyles.DoubleBuffer 
                      | ControlStyles.UserPaint;
            SetStyle(flags, true);
        }

        protected override void OnKeyDown(KeyEventArgs e)
        {
            base.OnKeyDown(e);
            if (e.KeyCode != Keys.Space) return;
            showGuideLines = !showGuideLines;
            Invalidate();
        }

        protected override void OnMouseDown(MouseEventArgs e)
        {
            base.OnMouseDown(e);
            currentState.OnMouseDown(e);
        }

        protected override void OnMouseUp(MouseEventArgs e)
        {
            base.OnMouseUp(e);
            currentState.OnMouseUp(e);
        }

        protected override void OnMouseMove(MouseEventArgs e)
        {
            base.OnMouseMove(e);
            currentState.OnMouseMove(e);
        }

        private static Pen boldPen = new Pen(Color.Black, 2);

        private void DrawBezier4(Graphics g, CPoint p0, CPoint p1, CPoint p2, CPoint p3)
        {
            var prev = p0;
            for (int i = 1; i < NUMBER_OF_STEPS; ++i)
            {
                var t = (float)i / (NUMBER_OF_STEPS - 1);
                var q0 = p0 * (1 - t) + p1 * t;
                var q1 = p1 * (1 - t) + p2 * t;
                var q2 = p2 * (1 - t) + p3 * t;
                var r0 = q0 * (1 - t) + q1 * t;
                var r1 = q1 * (1 - t) + q2 * t;
                var  b = r0 * (1 - t) + r1 * t;
                g.DrawLine(boldPen, prev, b);
                prev = b;
            }
        }

        private void DrawDiamond(Graphics g, PointF p, float s)
        {
            PointF[] diamond = new PointF[4];
            diamond[0] = new PointF(p.X, p.Y - s);
            diamond[1] = new PointF(p.X + s, p.Y);
            diamond[2] = new PointF(p.X, p.Y + s);
            diamond[3] = new PointF(p.X - s, p.Y);
            g.FillPolygon(Brushes.White, diamond);
            g.DrawPolygon(Pens.Black, diamond);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            base.OnPaint(e);
            if (0 == points.Count) return;
            var g = e.Graphics;
            g.Clear(SystemColors.Control);
            if (3 < points.Count)
                for (int i = 1; i < points.Count - 3; i += 3)
                    DrawBezier4(g, points[i], points[i + 1], points[i + 2], points[i + 3]);
            for (int i = 0; i < points.Count / 3; ++i)
            {
                var l = points[3 * i];
                var p = points[3 * i + 1];
                var r = points[3 * i + 2];
                if (showGuideLines) g.DrawLine(Pens.Brown, l, r);
                if (showGuideLines) DrawDiamond(g, l, 3);
                DrawDiamond(g, p, 4);
                if (showGuideLines) DrawDiamond(g, r, 3);
            }
        }
    }
}
