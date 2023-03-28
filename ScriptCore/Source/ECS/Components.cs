using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NL
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
    }

    public class TransformComponent : Component
    {
        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
            }
        }
    }

    public class ModelRendererComponent : Component
    {
        public void CalculateFinalBoneMatrices()
        {
            InternalCalls.ModelRendererComponent_CalculateFinalBoneMatrices(Entity.ID);
        }

        public int CreateBoneChain(Int32 tipBoneId)
        {
            return InternalCalls.ModelRendererComponent_CreateBoneChain(Entity.ID, tipBoneId);
        }

        public void InitializeBoneChainLocalOffsetAndRotation(Int32 chainId, Single tipLocalOffset)
        {
            InternalCalls.ModelRendererComponent_InitializeBoneChainLocalOffsetAndRotation(Entity.ID, chainId, tipLocalOffset);
        }

        public bool InverseKinematicsCCD(Int32 chainId, Vector3 modelWorldPosition, Vector3 targetWorldPosition, Int32 maxCCDIKIteration, Single eps)
        {
            return InternalCalls.ModelRendererComponent_InverseKinematicsCCD(Entity.ID, chainId, modelWorldPosition, targetWorldPosition, maxCCDIKIteration, eps);
        }

        public void RecalculateTransformationMatrices(Int32 chainId)
        {
            InternalCalls.ModelRendererComponent_RecalculateTransformationMatrices(Entity.ID, chainId);
        }
    }

}
