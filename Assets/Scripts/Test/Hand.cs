using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using NL;

namespace Homework1
{
    public struct RotateInfo
    {
        public float xCur, xDst;
        public float yCur, yDst;
        public float zCur, zDst;
    }

    public enum Pose
    {
        Ye, Ok, Rap
    };

    public class Hand : Entity
    {
        public Int32 metacarpals;

        public Int32 thumb_proximal_phalange;
        public Int32 thumb_intermediate_phalange;
        public Int32 thumb_distal_phalange;

        public Int32 index_proximal_phalange;
        public Int32 index_intermediate_phalange;
        public Int32 index_distal_phalange;

        public Int32 middle_proximal_phalange;
        public Int32 middle_intermediate_phalange;
        public Int32 middle_distal_phalange;

        public Int32 ring_proximal_phalange;
        public Int32 ring_intermediate_phalange;
        public Int32 ring_distal_phalange;

        public Int32 pinky_proximal_phalange;
        public Int32 pinky_intermediate_phalange;
        public Int32 pinky_distal_phalange;

        public Single anim_play_speed = 1.0f;

        ModelRendererComponent model;

        private Vector3 xAxis = new Vector3(1.0f, 0.0f, 0.0f);
        private Vector3 yAxis = new Vector3(0.0f, 1.0f, 0.0f);
        private Vector3 zAxis = new Vector3(0.0f, 0.0f, 1.0f);
        private Vector3 zero = new Vector3(0.0f, 0.0f, 0.0f);

        RotateInfo[] rotations = new RotateInfo[16];

        private const float pi = 3.1415926f;

        void OnCreate()
        {
            // Console.WriteLine($"Player.OnCreate - {ID}");

            model = GetComponent<ModelRendererComponent>();

            Init();

            SetNewPose(Pose.Ye);
        }

        void OnUpdateRuntime(float ts)
        {
            Rotate(ts);
            //model.RotateBone(thumb_intermediate_phalange, ref xAxis, ts);
            model.RecalculateFinalBoneMatrices();
        }

        void OnUpdateEditor(float ts)
        {

        }
        void Init()
        {
            for (int i = 0; i < 16; i++)
            {
                rotations[i].xCur = rotations[i].xDst = 0.0f;
                rotations[i].yCur = rotations[i].yDst = 0.0f;
                rotations[i].zCur = rotations[i].zDst = 0.0f;
                 
                model.RotateBone(i, ref zero);
            }
            model.RecalculateFinalBoneMatrices();
        }

        void Rotate(float ts)
        {
            float angle = anim_play_speed * ts;
            for (int i = 0; i < 16; i++)
            {
                float dt = rotations[i].xDst - rotations[i].xCur;
                float co = dt > 0 ? 1 : -1;
                if (Math.Abs(dt) > 1e-2)
                {
                    if (Math.Abs(dt) < angle)
                    {
                        rotations[i].xCur += dt;
                    }
                    else
                    {
                        rotations[i].xCur += angle * co;
                    }
                }

                dt = rotations[i].yDst - rotations[i].yCur;
                co = dt > 0 ? 1 : -1;
                if (Math.Abs(dt) > 1e-2)
                {
                    if (Math.Abs(dt) < angle)
                    {
                        rotations[i].yCur += dt;
                    }
                    else
                    {
                        rotations[i].yCur += angle * co;
                    }
                }

                dt = rotations[i].zDst - rotations[i].zCur;
                co = dt > 0 ? 1 : -1;
                if (Math.Abs(dt) > 1e-2)
                {
                    if (Math.Abs(dt) < angle)
                    {
                        rotations[i].zCur += dt;
                    }
                    else
                    {
                        rotations[i].zCur += angle * co;
                    }
                }

                Vector3 eulerAngles = new Vector3(rotations[i].xCur, rotations[i].yCur, rotations[i].zCur);
                model.RotateBone(i, ref eulerAngles);
            }
        }

        void SetNewPose(Pose pose)
        {
            switch (pose)
            {
                case Pose.Ye:
                    {
                        rotations[ring_proximal_phalange].yDst = 5;
                        rotations[ring_proximal_phalange].zDst = 70;
                        rotations[ring_intermediate_phalange].zDst = 70;
                        rotations[ring_distal_phalange].zDst = 70;

                        rotations[pinky_proximal_phalange].yDst = 15;
                        rotations[pinky_proximal_phalange].zDst = 70;
                        rotations[pinky_intermediate_phalange].zDst = 70;
                        rotations[pinky_distal_phalange].zDst = 70;

                        rotations[middle_proximal_phalange].yDst = 10;
                        rotations[index_proximal_phalange].yDst = -10;
                    }
                    break;
            }
        }
    }
}
