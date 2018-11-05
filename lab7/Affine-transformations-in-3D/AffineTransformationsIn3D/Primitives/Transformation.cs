
using System;

namespace AffineTransformationsIn3D.Primitives
{
    public class Transformation
    {
        private double[,] matrix = new double[4,4];

        public double[,] Matrix { get { return matrix; } }

        public Transformation()
        {
            matrix = Identity().matrix;
        }

        public Transformation(double[,] matrix)
        {
            this.matrix = matrix;
        }

        public static Transformation RotateX(double angle)
        {
            double cos = Math.Cos(angle);
            double sin = Math.Sin(angle);
            return new Transformation(
                new double[,]
                {
                    { 1, 0, 0, 0 },
                    { 0, cos, -sin, 0 },
                    { 0, sin, cos, 0 },
                    { 0, 0, 0, 1 }
                });
        }

        public static Transformation RotateY(double angle)
        {
            double cos = Math.Cos(angle);
            double sin = Math.Sin(angle);
            return new Transformation(
                new double[,]
                {
                    { cos, 0, sin, 0 },
                    { 0, 1, 0, 0 },
                    { -sin, 0, cos, 0 },
                    { 0, 0, 0, 1 }
                });
        }

        public static Transformation RotateZ(double angle)
        {
            double cos = Math.Cos(angle);
            double sin = Math.Sin(angle);
            return new Transformation(
                new double[,]
                {
                    { cos, -sin, 0, 0 },
                    { sin, cos, 0, 0 },
                    { 0, 0, 1, 0 },
                    { 0, 0, 0, 1 }
                });
        }

        public static Transformation Scale(double fx, double fy, double fz)
        {
            return new Transformation( 
                new double[,] { 
                    { fx, 0, 0, 0 },
                    { 0, fy, 0, 0 },
                    { 0, 0, fz, 0 },
                    { 0, 0, 0, 1 }
                });
        }
        
        public static Transformation Translate(double dx, double dy, double dz)
        {
            return new Transformation(
                new double[,]
                {
                    { 1, 0, 0, 0 },
                    { 0, 1, 0, 0 },
                    { 0, 0, 1, 0 },
                    { dx, dy, dz, 1 },
                });
        }

        public static Transformation Identity()
        {
            return new Transformation(
                new double[,] {
                    { 1, 0, 0, 0 },
                    { 0, 1, 0, 0 },
                    { 0, 0, 1, 0 },
                    { 0, 0, 0, 1 }
                });
        }


        public static Transformation ReflectX()
        {
            return Scale(-1, 1, 1);
        }

        public static Transformation ReflectY()
        {
            return Scale(1, -1, 1);
        }

        public static Transformation ReflectZ()
        {
            return Scale(1, 1, -1);
        }

        public static Transformation OrthogonalProjection()
        {
            return Identity();
        }

        public static Transformation RotateAroundLine(Point3D a, Point3D b, double angle)
        {
            var dx = b.X - a.X;
            var dy = b.Y - a.Y;
            var dz = b.Z - a.Z;
            var angleY = -Math.Atan2(dz, dx);
            var angleZ = Math.Atan2(dy, dx);
            return Translate(-a.X, -a.Y, -a.Z)
                * RotateZ(angleZ)
                * RotateY(angleY)
                * RotateX(angle)
                * RotateY(-angleY)
                * RotateZ(-angleZ)
                * Translate(a.X, a.Y, a.Z);
        }

        public static Transformation operator *(Transformation t1, Transformation t2)
        {
            double[,] matrix = new double[4, 4];
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                {
                    matrix[i, j] = 0;
                    for (int k = 0; k < 4; ++k)
                        matrix[i, j] += t1.matrix[i, k] * t2.matrix[k, j];
                }
            return new Transformation(matrix);
        }
    }
}
