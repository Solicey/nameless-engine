using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using NL;

namespace Homework1
{
    public class Hand : Entity
    {
        ModelRendererComponent Model;
        TransformComponent ModelTransform;

        public Int32 MaxCCDIKIteration = 30;
        public Single eps = 0.01f;

        public Int32 ThumbTipBoneId = -1;
        public Single ThumbTipOffset = 1.0f;

        private Int32 ThumbTipBoneId_LastFrame = -1;
        private Int32 ThumbChainId = -1; 
        TransformComponent ThumbFingerTargetTransform;
        

        void OnCreate()
        {
            Model = GetComponent<ModelRendererComponent>();
            ModelTransform = GetComponent<TransformComponent>();

            ThumbFingerTargetTransform = FindEntityByName("ThumbFingerTarget")?.GetComponent<TransformComponent>();
            
        }

        void OnUpdateRuntime(float ts)
        {
            
        }

        void OnUpdateEditor(float ts)
        {
            if (ThumbTipBoneId != ThumbTipBoneId_LastFrame && ThumbTipBoneId != -1)
            {
                ThumbChainId = Model.CreateBoneChain(ThumbTipBoneId);
                Model.InitializeBoneChainLocalOffsetAndRotation(ThumbChainId, ThumbTipOffset);

                Model.InverseKinematicsCCD(ThumbChainId, ModelTransform.Translation, ThumbFingerTargetTransform.Translation, MaxCCDIKIteration, eps);
                Model.RecalculateTransformationMatrices(ThumbChainId);

                Model.CalculateFinalBoneMatrices();
            }            

            UpdateVariables();
        }

        // For variables changing detection
        void UpdateVariables()
        {
            ThumbTipBoneId_LastFrame = ThumbTipBoneId;
        }
    }
}
