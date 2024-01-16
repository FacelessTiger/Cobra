using System.Runtime.InteropServices;
using System.Runtime.Serialization.Formatters;

namespace Cobra
{
    public class Matrix4
    {
        private float[] m_Array;

        public Matrix4(float[] array)
        {
            m_Array = array;
        }

        public float this[int x, int y]
        {
            get => m_Array[y * 4 + x];
            set => m_Array[y * 4 + x] = value;
        }

        public Matrix4 Inverse()
        {
            return new Matrix4(InternalCalls.Matrix4_Inverse(m_Array));
        }

        public static Matrix4 operator *(Matrix4 left, Matrix4 right)
        {
            return new Matrix4(InternalCalls.Matrix4_MultiplyMat4Mat4(left.m_Array, right.m_Array));
        }

        public static Vector4 operator *(Matrix4 left, Vector4 right)
        {
            return InternalCalls.Matrix4_MultiplyMat4Vec4(left.m_Array, right);
        }

        public override string ToString()
        {
            string ret = "[";
            ret +=       this[0, 0] + ", " + this[0, 1] + ", " + this[0, 2] + ", " + this[0, 3] + "],\n";
            ret += "[" + this[1, 0] + ", " + this[1, 1] + ", " + this[1, 2] + ", " + this[1, 3] + "],\n";
            ret += "[" + this[2, 0] + ", " + this[2, 1] + ", " + this[2, 2] + ", " + this[2, 3] + "],\n";
            ret += "[" + this[3, 0] + ", " + this[3, 1] + ", " + this[3, 2] + ", " + this[3, 3] + "],\n";
            return ret;
        }

        public Vector3 GetTranslation()
        {
            return InternalCalls.Matrix4_GetTranslation(m_Array);
        }

        public Vector3 GetScale()
        {
            return InternalCalls.Matrix4_GetScale(m_Array);
        }
    }
}
