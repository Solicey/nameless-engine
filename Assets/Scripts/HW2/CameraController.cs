using NL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HW2
{
    public enum CameraType
    {
        Fixed, Track, FPS
    }

    public enum TrackCameraState
    {
        Pause, ToEnd, ToBegin
    }

    public class CameraController : Entity
    {
        // Exposed
        public float TrackMoveSpeed = 1;

        public float FpsMouseSensitivity = 20;
        public float FpsMoveSpeed = 5;

        //

        CameraComponent fixedCamera;
        CameraComponent trackCamera;
        CameraComponent fpsCamera;

        TransformComponent trackBeginTransform;
        TransformComponent trackEndTransform;
        
        TransformComponent fpsTransform;
        TransformComponent trackCameraTransform;

        CameraType cameraType;

        Vector2 cursorPosLastFrame = new Vector2(0, 0);
        Vector2 cursorDelta = new Vector2(0, 0);
        float pitchAngle = 0.0f;
        float yawAngle = 0.0f;

        float trackProgress = 0.0f;
        Vector3 trackDirection = new Vector3();
        TrackCameraState trackState = TrackCameraState.Pause;
        Vector4 trackBeginQuat = new Vector4();
        Vector4 trackEndQuat = new Vector4();
        Vector4 trackQuatDirection = new Vector4();

        void OnCreate()
        {
            fixedCamera = FindEntityByName("FixedCamera")?.GetComponent<CameraComponent>();
            trackCamera = FindEntityByName("TrackCamera")?.GetComponent<CameraComponent>();
            fpsCamera = FindEntityByName("FPSCamera")?.GetComponent<CameraComponent>();

            trackCamera?.SetAsRuntimeCamera();
            cameraType = CameraType.Track;

            trackBeginTransform = FindEntityByName("TrackBegin")?.GetComponent<TransformComponent>();
            trackEndTransform = FindEntityByName("TrackEnd")?.GetComponent<TransformComponent>();
            fpsTransform = FindEntityByName("FPSCamera")?.GetComponent<TransformComponent>();
            trackCameraTransform = FindEntityByName("TrackCamera")?.GetComponent<TransformComponent>();

            trackDirection = trackEndTransform.Translation - trackBeginTransform.Translation;

            trackBeginQuat = trackBeginTransform.Rotation.Quaternion();
            trackEndQuat = trackEndTransform.Rotation.Quaternion();
            trackQuatDirection = trackEndQuat - trackBeginQuat;

            /*Vector3 euler = trackBeginQuat.EulerAngles();
              Console.WriteLine($"begin euler: {euler.X * 180 / Math.PI}, {euler.Y * 180 / Math.PI}, {euler.Z * 180 / Math.PI}");

              euler = trackEndQuat.EulerAngles();
              Console.WriteLine($"end euler: {euler.X * 180 / Math.PI}, {euler.Y * 180 / Math.PI}, {euler.Z * 180 / Math.PI}");
            */
        }

        void OnUpdateRuntime(float ts)
        {
            InputDetect();

            if (cameraType == CameraType.FPS)
            {
                FPSCameraUpdate(ts);
            }
            else if (cameraType == CameraType.Track)
            {
                TrackCameraUpdate(ts);
            }            
        }

        void OnUpdateEditor(float ts)
        {

        }

        void InputDetect()
        {
            /*if (Input.IsKeyDown(KeyCode.D3))
            {
                fixedCamera?.SetAsRuntimeCamera();
                cameraType = CameraType.Fixed;

                trackState = TrackCameraState.Pause;
            }*/
            if (Input.IsKeyDown(KeyCode.D3))
            {
                trackCamera?.SetAsRuntimeCamera();
                cameraType = CameraType.Track;
            }
            else if (Input.IsKeyDown(KeyCode.D4))
            {
                fpsCamera?.SetAsRuntimeCamera();
                cameraType= CameraType.FPS;
                cursorPosLastFrame = Input.GetCursorPos();

                trackState = TrackCameraState.Pause;
            }
        }

        void TrackCameraUpdate(float ts)
        {
            // ToEnd
            if (Input.IsKeyDown(KeyCode.D))
            {
                trackState = TrackCameraState.ToEnd;
            }
            // Pause
            else if (Input.IsKeyDown(KeyCode.S))
            {
                trackState = TrackCameraState.Pause;
            }
            // ToBegin
            else if (Input.IsKeyDown(KeyCode.A))
            {
                trackState = TrackCameraState.ToBegin;
            }

            if (trackState == TrackCameraState.ToEnd)
            {
                trackProgress = Math.Min(1.0f, trackProgress + TrackMoveSpeed * ts);
            }
            else if (trackState == TrackCameraState.ToBegin)
            {
                trackProgress = Math.Max(0.0f, trackProgress - TrackMoveSpeed * ts);
            }

            // Translation Lerp
            trackCameraTransform.Translation = trackBeginTransform.Translation + trackDirection * trackProgress;

            // Rotation Quaternion NLerp
            Vector4 quat = trackBeginQuat + trackQuatDirection * trackProgress;
            quat.QuatNormalize();
            Vector3 euler = quat.EulerAngles();
            trackCameraTransform.Rotation = euler;

            // Console.WriteLine($"quat: {quat.X}, {quat.Y}, {quat.Z}, {quat.W}");
            // Console.WriteLine($"euler: {euler.X * 180 / Math.PI}, {euler.Y * 180 / Math.PI}, {euler.Z * 180 / Math.PI}");
        }

        void FPSCameraUpdate(float ts)
        {
            // Rotation
            Vector2 pos = Input.GetCursorPos();

            cursorDelta = pos - cursorPosLastFrame;
            cursorPosLastFrame = pos;

            float cursorX = cursorDelta.X * FpsMouseSensitivity * ts;
            float cursorY = cursorDelta.Y * FpsMouseSensitivity * ts;
            
            yawAngle += cursorX;
            pitchAngle += cursorY;

            if (pitchAngle > 90f)
                pitchAngle = 90f;
            else if (pitchAngle < -90f)
                pitchAngle = -90f;
            if (yawAngle < 0f) yawAngle += 360f;
            fpsTransform.Rotation = new Vector3(-pitchAngle, -yawAngle, 0f);

            // fpsTransform.Rotate(fpsTransform.Up(), -cursorX);
            // fpsTransform.Rotate(fpsTransform.Right(), cursorY);

            // Translation
            float horizontal = Input.IsKeyDown(KeyCode.A) ? 1f : (Input.IsKeyDown(KeyCode.D) ? -1f : 0f);
            float vertical = Input.IsKeyDown(KeyCode.W) ? -1f : (Input.IsKeyDown(KeyCode.S) ? 1f : 0f);

            Vector3 forward = fpsTransform.Forward();
            Vector3 right = fpsTransform.Right();

            Vector3 deltaTranslation = (forward * vertical + right * horizontal) * FpsMoveSpeed * ts;
            fpsTransform.Translate(deltaTranslation);
        }
    }
}
