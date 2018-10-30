using System.Windows.Forms;
using System.Collections.Generic;
using System;
using AffineTransformationsIn3D.Primitives;
using AffineTransformationsIn3D.Polyhedrons;

namespace AffineTransformationsIn3D
{
    public partial class Form1 : Form
    {
        private IPolyhedron tetrahedron = new Tetrahedron(0.5f);

        public Form1()
        {
            InitializeComponent();
            List<IPrimitive> scene = new List<IPrimitive>();

            // create axis points
            var a = new Point3D(0, 0, 0);
            var b = new Point3D(0.8, 0, 0);
            var c = new Point3D(0, 0.8, 0);
            var d = new Point3D(0, 0, 0.8);
            scene.Add(a);
            scene.Add(b);
            scene.Add(c);
            scene.Add(d);

            // add axis lines
            scene.Add(new Line(a, b));
            scene.Add(new Line(a, c));
            scene.Add(new Line(a, d));
            // add polyhedron
            scene.Add(tetrahedron);

            // add them to scenes
            sceneView1.Scene = scene;
            sceneView2.Scene = scene;
            sceneView3.Scene = scene;
            sceneView4.Scene = scene;

            // apply different projections
            sceneView1.Projection = Transformation.OrthogonalProjection();
            sceneView2.Projection = Transformation.OrthogonalProjection()
                * Transformation.RotateY(Math.PI / 2);
            sceneView3.Projection = Transformation.OrthogonalProjection()
                * Transformation.RotateX(-Math.PI / 2);
            sceneView4.Projection = Transformation.OrthogonalProjection()
                * Transformation.RotateY(Math.PI / 4)
                * Transformation.RotateX(-Math.PI / 4);
        }

        private void scenesRefresh()
        {
            sceneView1.Refresh();
            sceneView2.Refresh();
            sceneView3.Refresh();
            sceneView4.Refresh();
        }

        private void Scale(object sender, EventArgs e)
        {
            double x_axis = (double)numericUpDown1.Value;
            double y_axis = (double)numericUpDown2.Value;
            double z_axis = (double)numericUpDown3.Value;
            tetrahedron.Apply(Transformation.Scale(x_axis, y_axis, z_axis));
            scenesRefresh();
        }

        private void Rotate(object sender, EventArgs e)
        {
            double x_axis = (double)numericUpDown4.Value / 180 * Math.PI;
            double y_axis = (double)numericUpDown5.Value / 180 * Math.PI;
            double z_axis = (double)numericUpDown6.Value / 180 * Math.PI;

            tetrahedron.Apply(Transformation.RotateX(x_axis) * Transformation.RotateY(y_axis) * Transformation.RotateZ(z_axis));
            scenesRefresh();
        }

        private void Translate(object sender, EventArgs e)
        {
            double x_axis = (double)numericUpDown7.Value;
            double y_axis = (double)numericUpDown8.Value;
            double z_axis = (double)numericUpDown9.Value;

            tetrahedron.Apply(Transformation.Translate(x_axis, y_axis, z_axis));
            scenesRefresh();
        }

        private void Reflect(object sender, EventArgs e)
        {
            Transformation reflection;
            if (radioButton1.Checked)
                reflection = Transformation.ReflectX();
            else if (radioButton2.Checked)
                reflection = Transformation.ReflectY();
            else if (radioButton3.Checked)
                reflection = Transformation.ReflectZ();
            else throw new Exception("Something went wrong");

            tetrahedron.Apply(reflection);
            scenesRefresh();
        }

        private void RotateAroundCenter(object sender, EventArgs e)
        {
            double x_axis = (double)numericUpDown10.Value;
            double y_axis = (double)numericUpDown11.Value;
            double z_axis = (double)numericUpDown12.Value;
            Point3D p = tetrahedron.Center;

            tetrahedron.Apply(Transformation.Translate(-p.X, -p.Y, -p.Z)
                * Transformation.RotateX(x_axis / 180 * Math.PI)
                * Transformation.RotateY(y_axis / 180 * Math.PI)
                * Transformation.RotateZ(z_axis / 180 * Math.PI)
                * Transformation.Translate(p.X, p.Y, p.Z));
            scenesRefresh();
        }

        private void TranslateAroundLine(object sender, EventArgs e)
        {
            Point3D p1 = new Point3D();
            Point3D p2 = new Point3D();


            p1.X = (double)numericUpDown13.Value;
            p1.Y = (double)numericUpDown14.Value;
            p1.Z = (double)numericUpDown15.Value;

            p2.X = (double)numericUpDown16.Value;
            p2.Y = (double)numericUpDown17.Value;
            p2.Z = (double)numericUpDown18.Value;

            Point3D center = new Point3D();
            center.X += (p1.X + p2.X) / 2;
            center.Y += (p1.Y + p2.Y) / 2;
            center.Z += (p1.Z + p2.Z) / 2;

            var line = new Line(p1, p2);
            sceneView1.Scene.Add(line);

            var scalingTetrahedron = Transformation.Translate(-center.X, -center.Y, -center.Y);
            line.Apply(scalingTetrahedron);

            int x_rotation = 0;
            while (Math.Abs(line.A.Y) > 0.01)
            {
                double x_axis = 1.0 / 180 * Math.PI;
                double y_axis = 0.0 / 180 * Math.PI;
                double z_axis = 0.0 / 180 * Math.PI;
                scalingTetrahedron = Transformation.RotateX(x_axis) * Transformation.RotateY(y_axis) * Transformation.RotateZ(z_axis);

                line.Apply(scalingTetrahedron);
                x_rotation++;
            }

            int y_rotation = 0;
            while (Math.Abs(line.A.X) > 0.01)
            {
                double x_axis = 0.0 / 180 * Math.PI;
                double y_axis = 1.0 / 180 * Math.PI;
                double z_axis = 0.0 / 180 * Math.PI;
                scalingTetrahedron = Transformation.RotateX(x_axis) * Transformation.RotateY(y_axis)  * Transformation.RotateZ(z_axis);

                line.Apply(scalingTetrahedron);
                y_rotation++;
            }

            double rotate_x_axis = ((double)(360 - x_rotation % 360) / 180 * Math.PI);
            double rotate_y_axis = ((double)(360 - y_rotation % 360) / 180 * Math.PI);
            double rotate_z_axis = ((double)(360 - ((int)numericUpDown19.Value % 360)) / 180 * Math.PI);
            // ??????????????????????????????????
            var scalingTetrahedron_1 = Transformation.RotateX(rotate_x_axis) * Transformation.RotateY(rotate_y_axis) * Transformation.RotateZ(rotate_z_axis);
            tetrahedron.Apply(scalingTetrahedron);

            scalingTetrahedron = Transformation.Translate(center.X, center.Y, center.Y);
            tetrahedron.Apply(scalingTetrahedron);

            scenesRefresh();
        }
    }
}
