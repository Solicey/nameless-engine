using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NL
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }

        public static Vector2 GetCursorPos()
        {
            Vector2 pos;
            InternalCalls.Input_GetCursorPos(out pos);
            return pos;
        }
    }
}
