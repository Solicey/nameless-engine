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
        private struct Bone
        {

        };

        public Int32 ThumbFingerTipParentId = -1;

        TransformComponent ThumbFingerTip;
        TransformComponent ThumbFingerTarget;

        void OnCreate()
        {
            ThumbFingerTip = FindEntityByName("ThumbFingerTip")?.GetComponent<TransformComponent>();
            ThumbFingerTarget = FindEntityByName("ThumbFingerTarget")?.GetComponent<TransformComponent>();
        }

        void OnUpdateRuntime(float ts)
        {
            
        }

        void OnUpdateEditor(float ts)
        {
            
        }
    }
}
