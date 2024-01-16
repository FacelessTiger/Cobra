using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Cobra
{
    public class Input
    {
        public static bool IsKeyDown(KeyCode keycode)
        {
            return InternalCalls.Input_IsKeyDown(keycode);
        }

        public static bool IsMouseButtonDown(MouseCode mousecode)
        {
            return InternalCalls.Input_IsMouseButtonDown(mousecode);
        }

        public static float GetMouseX()
        {
            return InternalCalls.Input_GetMouseX();
        }

        public static float GetMouseY()
        {
            return InternalCalls.Input_GetMouseY();
        }
    }
}
