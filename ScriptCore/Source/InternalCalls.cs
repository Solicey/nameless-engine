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
        internal extern static bool Input_IsKeyDown(KeyCode keycode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ModelRendererComponent_GetBoneLocalTransformation(ulong entityID, Int32 boneId, out Vector3 outTranslation, out Vector3 outRotation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ModelRendererComponent_SetBoneLocalTransformation(ulong entityID, Int32 boneId, ref Vector3 outTranslation, ref Vector3 outRotation);
    }
}
