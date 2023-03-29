﻿using System;
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
        public void RecalculateFinalBoneMatrices()
        {
            InternalCalls.ModelRendererComponent_RecalculateFinalBoneMatrices(Entity.ID);
        }

        public void RotateBone(Int32 boneId, ref Vector3 eulerAngles)
        {
            InternalCalls.ModelRendererComponent_RotateBone(Entity.ID, boneId, ref eulerAngles);
        }

    }

}
