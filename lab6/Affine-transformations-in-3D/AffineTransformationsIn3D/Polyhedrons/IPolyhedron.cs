using System;
using System.Collections.Generic;
using System.Drawing;
using AffineTransformationsIn3D.Primitives;

namespace AffineTransformationsIn3D.Polyhedrons
{
    interface IPolyhedron : IPrimitive
    {
        Point3D Center { get; }
    }
}
