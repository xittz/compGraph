using System;
using System.Collections.Generic;
using System.Drawing;
using AffineTransformationsIn3D.Primitives;

namespace AffineTransformationsIn3D.Polyhedrons
{
    class Tetrahedron : IPolyhedron
    {
        // первые три точки - основание тетраэдра, четвертая точка - его вершина
        private List<Point3D> points = new List<Point3D>();

        private List<Polygon> polygons = new List<Polygon>();

        public List<Point3D> Points { get { return points; } }
        public List<Polygon> Polygons { get { return polygons; } }

        public Point3D Center
        {
            get
            {
                Point3D p = new Point3D(0, 0, 0);
                for (int i = 0; i < 4; i++)
                {
                    p.X += Points[i].X;
                    p.Y += Points[i].Y;
                    p.Z += Points[i].Z;
                }
                p.X /= 4;
                p.Y /= 4;
                p.Z /= 4;
                return p;
            }
        }

        public Tetrahedron(double size) 
        {
            double h = Math.Sqrt(2.0 / 3.0) * size;
            points = new List<Point3D>();

            points.Add(new Point3D(-size / 2, 0, h / 3));
            points.Add(new Point3D(0, 0, -h * 2 / 3));
            points.Add(new Point3D(size / 2, 0, h / 3));
            points.Add(new Point3D(0, h, 0));

            // Основание тетраэдра
            polygons.Add(new Polygon(new Point3D[] { points[0], points[1], points[2] }));
            // Левая грань
            polygons.Add(new Polygon(new Point3D[] { points[1], points[3], points[0] }));
            // Правая грань
            polygons.Add(new Polygon(new Point3D[] { points[2], points[3], points[1] }));
            // Передняя грань
            polygons.Add(new Polygon(new Point3D[] { points[0], points[3], points[2] }));
        }

        public void Apply(Transformation t)
        {
            foreach (var point in Points)
                point.Apply(t);
        }

        public void Draw(Graphics g, Transformation projection, int width, int height)
        {
            foreach (var facet in Polygons)
                facet.Draw(g, projection, width, height);
        }
    }
}
