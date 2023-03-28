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


        // IK
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ModelRendererComponent_CalculateFinalBoneMatrices(ulong entityID);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static int ModelRendererComponent_CreateBoneChain(ulong entityID, Int32 tipBoneId);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ModelRendererComponent_InitializeBoneChainLocalOffsetAndRotation(ulong entityID, Int32 chainId, Single tipLocalOffset);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool ModelRendererComponent_InverseKinematicsCCD(ulong entityID, Int32 chainId, Vector3 modelWorldPosition, Vector3 targetWorldPosition, Int32 maxCCDIKIteration, Single eps);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void ModelRendererComponent_RecalculateTransformationMatrices(ulong entityID, Int32 chainId);

        // IK
    }
}
