using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace NL
{
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Entity_FindEntityByName(string name);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object GetScriptInstance(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(ulong entityID, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(ulong entityID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetForward(ulong entityID, out Vector3 forward);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetRight(ulong entityID, out Vector3 right);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_Translate(ulong entityID, ref Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_Rotate(ulong entityID, ref Vector3 eulerAngles);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode keycode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_GetCursorPos(out Vector2 pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ModelRendererComponent_RecalculateFinalBoneMatrices(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ModelRendererComponent_RotateBone(ulong entityID, Int32 boneId, ref Vector3 eulerAngles);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void CameraComponent_SetAsRuntimeCamera(ulong entityID);
    }
}
