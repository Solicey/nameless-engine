using System;
using System.Runtime.CompilerServices;

namespace TestNamespace
{
    public class TestClass
    {
        public float MyPublicFloatVar = 5.0f;
        public void PrintFloatVar()
        {
            Console.WriteLine("MyPublicFloatVar = {0:F}", MyPublicFloatVar);
            //PrintString("PrintString");
        }

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //extern static void PrintString(string s);
    }
}