using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using NL;

namespace HW1
{
    public struct RotateInfo
    {
        public float xCur, xDst;
        public float yCur, yDst;
        public float zCur, zDst;

        public float xSpeed;
        public float ySpeed;
        public float zSpeed;
    }

    public enum Pose
    {
        Clear, Ye, Ok, Rap
    };

    public class Hand : Entity
    {
        public Single anim_play_time = 1.0f;

        public Single anim_rot_speed = 30.0f;

        public Vector3 metacarpals_rot = new Vector3(0.0f, 0.0f, 0.0f);

        public Vector3 thumb_proximal_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 thumb_intermediate_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 thumb_distal_rot = new Vector3(0.0f, 0.0f, 0.0f);

        public Vector3 index_proximal_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 index_intermediate_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 index_distal_rot = new Vector3(0.0f, 0.0f, 0.0f);

        public Vector3 middle_proximal_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 middle_intermediate_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 middle_distal_rot = new Vector3(0.0f, 0.0f, 0.0f);

        public Vector3 ring_proximal_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 ring_intermediate_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 ring_distal_rot = new Vector3(0.0f, 0.0f, 0.0f);

        public Vector3 pinky_proximal_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 pinky_intermediate_rot = new Vector3(0.0f, 0.0f, 0.0f);
        public Vector3 pinky_distal_rot = new Vector3(0.0f, 0.0f, 0.0f);

        private Int32 metacarpals = 0;

        private Int32 thumb_proximal_phalange = 1;
        private Int32 thumb_intermediate_phalange = 3;
        private Int32 thumb_distal_phalange = 2;

        private Int32 index_proximal_phalange = 6;
        private Int32 index_intermediate_phalange = 9;
        private Int32 index_distal_phalange = 8;

        private Int32 middle_proximal_phalange = 5;
        private Int32 middle_intermediate_phalange = 11;
        private Int32 middle_distal_phalange = 10;

        private Int32 ring_proximal_phalange = 4;
        private Int32 ring_intermediate_phalange = 13;
        private Int32 ring_distal_phalange = 12;

        private Int32 pinky_proximal_phalange = 7;
        private Int32 pinky_intermediate_phalange = 15;
        private Int32 pinky_distal_phalange = 14;

        ModelRendererComponent model;

        private Vector3 xAxis = new Vector3(1.0f, 0.0f, 0.0f);
        private Vector3 yAxis = new Vector3(0.0f, 1.0f, 0.0f);
        private Vector3 zAxis = new Vector3(0.0f, 0.0f, 1.0f);
        private Vector3 zero = new Vector3(0.0f, 0.0f, 0.0f);

        RotateInfo[] rotations = new RotateInfo[16];

        void OnCreate()
        {
            model = GetComponent<ModelRendererComponent>();
            Init();
            // SetNewPose(Pose.Ye);
        }

        // Runtime模式的Update
        void OnUpdateRuntime(float ts)
        {
            InputDetect();
            RotateRuntime(ts);
            model.RecalculateFinalBoneMatrices();
        }

        // Editor模式的Update
        void OnUpdateEditor(float ts)
        {
            RotateEditor();
            model.RecalculateFinalBoneMatrices();
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

        void RotateEditor()
        {
            //Console.WriteLine($"metacarpals - {metacarpals_rot.X}");

            model.RotateBone(metacarpals, ref metacarpals_rot);

            model.RotateBone(thumb_proximal_phalange, ref thumb_proximal_rot);
            model.RotateBone(thumb_intermediate_phalange, ref thumb_intermediate_rot);
            model.RotateBone(thumb_distal_phalange, ref thumb_distal_rot);

            model.RotateBone(index_proximal_phalange, ref index_proximal_rot);
            model.RotateBone(index_intermediate_phalange, ref index_intermediate_rot);
            model.RotateBone(index_distal_phalange, ref index_distal_rot);

            model.RotateBone(middle_proximal_phalange, ref middle_proximal_rot);
            model.RotateBone(middle_intermediate_phalange, ref middle_intermediate_rot);
            model.RotateBone(middle_distal_phalange, ref middle_distal_rot);

            model.RotateBone(ring_proximal_phalange, ref ring_proximal_rot);
            model.RotateBone(ring_intermediate_phalange, ref ring_intermediate_rot);
            model.RotateBone(ring_distal_phalange, ref ring_distal_rot);

            model.RotateBone(pinky_proximal_phalange, ref pinky_proximal_rot);
            model.RotateBone(pinky_intermediate_phalange, ref pinky_intermediate_rot);
            model.RotateBone(pinky_distal_phalange, ref pinky_distal_rot);
        }

        void RotateRuntime(float ts)
        {
            for (int i = 1; i < 16; i++)
            {
                float co = rotations[i].xDst - rotations[i].xCur > 0 ? 1.0f : -1.0f;
                float angle = Math.Min(rotations[i].xSpeed * ts, (rotations[i].xDst - rotations[i].xCur) * co);
                rotations[i].xCur += angle * co;

                co = rotations[i].yDst - rotations[i].yCur > 0 ? 1.0f : -1.0f;
                angle = Math.Min(rotations[i].ySpeed * ts, (rotations[i].yDst - rotations[i].yCur) * co);
                rotations[i].yCur += angle * co;

                co = rotations[i].zDst - rotations[i].zCur > 0 ? 1.0f : -1.0f;
                angle = Math.Min(rotations[i].zSpeed * ts, (rotations[i].zDst - rotations[i].zCur) * co);
                rotations[i].zCur += angle * co;

                Vector3 eulerAngles = new Vector3(rotations[i].xCur, rotations[i].yCur, rotations[i].zCur);
                model.RotateBone(i, ref eulerAngles);
            }

            rotations[0].xCur = (rotations[0].xCur + anim_rot_speed * ts) % 360;
            Vector3 meta = new Vector3(rotations[0].xCur, 0, 0);
            model.RotateBone(0, ref meta);
        }

        void SetNewPose(Pose pose)
        {
            switch (pose)
            {
                case Pose.Clear:
                    for (int i = 1; i < 16; i++)
                    {
                        rotations[i].xDst = rotations[i].yDst = rotations[i].zDst = 0.0f;
                    }
                    break;
                case Pose.Ye:
                    {
                        rotations[ring_proximal_phalange].yDst = 5;
                        rotations[ring_proximal_phalange].zDst = 40;
                        rotations[ring_intermediate_phalange].zDst = 70;
                        rotations[ring_distal_phalange].zDst = 70;

                        rotations[pinky_proximal_phalange].yDst = 15;
                        rotations[pinky_proximal_phalange].zDst = 40;
                        rotations[pinky_intermediate_phalange].zDst = 70;
                        rotations[pinky_distal_phalange].zDst = 70;

                        rotations[middle_proximal_phalange].yDst = 10;
                        rotations[middle_intermediate_phalange].zDst = -18.8f;

                        rotations[index_proximal_phalange].yDst = -10;
                        rotations[index_intermediate_phalange].yDst = 1.8f;
                        rotations[index_intermediate_phalange].zDst = -15.0f;

                        rotations[thumb_proximal_phalange].yDst = 16.30f;
                        rotations[thumb_proximal_phalange].zDst = 11.50f;

                        rotations[thumb_intermediate_phalange].xDst = -28.10f;
                        rotations[thumb_intermediate_phalange].yDst = 8.60f;
                        rotations[thumb_intermediate_phalange].zDst = 40.10f;

                        rotations[thumb_distal_phalange].zDst = 69.80f;
                    }
                    break;
            }

            CalculateRotateSpeed();
        }

        void CalculateRotateSpeed()
        {
            for (int i = 0; i < 16; i++)
            {
                rotations[i].xSpeed = Math.Abs((rotations[i].xDst - rotations[i].xCur) * anim_play_time);
                rotations[i].ySpeed = Math.Abs((rotations[i].yDst - rotations[i].yCur) * anim_play_time);
                rotations[i].zSpeed = Math.Abs((rotations[i].zDst - rotations[i].zCur) * anim_play_time);
            }
        }

        void InputDetect()
        {
            if (Input.IsKeyDown(KeyCode.D1))
            {
                // Console.WriteLine("Key 1 Pressed!");
                SetNewPose(Pose.Clear);
            }
            else if (Input.IsKeyDown(KeyCode.D2))
            {
                // Console.WriteLine("Key 2 Pressed!");
                SetNewPose(Pose.Ye);
            }
        }
    }
}
