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
        public void GetBoneTransformation(Int32 boneIndex, out Vector3 translation, out Vector3 rotation)
        {
            InternalCalls.ModelRendererComponent_GetBoneLocalTransformation(Entity.ID, boneIndex, out translation, out rotation);
        }

        public void SetBoneTransformation(Int32 boneIndex, ref Vector3 translation, ref Vector3 rotation)
        {
            InternalCalls.ModelRendererComponent_SetBoneLocalTransformation(Entity.ID, boneIndex, ref translation, ref rotation);
        }
    }

}
