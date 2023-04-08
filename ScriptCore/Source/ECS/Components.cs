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

        public Vector3 Rotation
        {
            get
            {
                InternalCalls.TransformComponent_GetRotation(Entity.ID, out Vector3 rotation);
                return rotation;
            }
            set
            {
                InternalCalls.TransformComponent_SetRotation(Entity.ID, ref value);
            }
        }

        public Vector3 Forward()
        {
            Vector3 forward;
            InternalCalls.TransformComponent_GetForward(Entity.ID, out forward);
            return forward;
        }

        public Vector3 Right()
        {
            Vector3 right;
            InternalCalls.TransformComponent_GetRight(Entity.ID, out right);
            return right;
        }

        public Vector3 Up()
        {
            Vector3 up;
            InternalCalls.TransformComponent_GetUp(Entity.ID, out up);
            return up;
        }

        public void Translate(Vector3 translation)
        {
            InternalCalls.TransformComponent_Translate(Entity.ID, ref translation);
        }

        /// <summary>
        /// eulerAngles in degree
        /// </summary>
        /// <param name="eulerAngles"></param>
        public void Rotate(Vector3 rotateAxis, float angle)
        {
            InternalCalls.TransformComponent_Rotate(Entity.ID, ref rotateAxis, angle);
        }

        public void LookAt(Vector3 dest)
        {
            InternalCalls.TransformComponent_LookAt(Entity.ID, ref dest);
        }
    }

    public class ModelRendererComponent : Component
    {
        public void RecalculateFinalBoneMatrices()
        {
            InternalCalls.ModelRendererComponent_RecalculateFinalBoneMatrices(Entity.ID);
        }

        /// <summary>
        /// eulerAngles in degree
        /// </summary>
        /// <param name="boneId"></param>
        /// <param name="eulerAngles"></param>
        public void RotateBone(Int32 boneId, ref Vector3 eulerAngles)
        {
            InternalCalls.ModelRendererComponent_RotateBone(Entity.ID, boneId, ref eulerAngles);
        }

    }

    public class CameraComponent : Component
    {
        public void SetAsRuntimeCamera()
        {
            InternalCalls.CameraComponent_SetAsRuntimeCamera(Entity.ID);
        }
    }

}
