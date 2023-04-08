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

    public class CameraController : Entity
    {
        // Exposed
        public float MouseSensitivity = 10;
        public float MoveSpeed = 13;

        //

        CameraComponent fixedCamera;
        CameraComponent trackCamera;
        CameraComponent fpsCamera;

        TransformComponent beginTransform;
        TransformComponent endTransform;
        TransformComponent fpsTransform;

        CameraType cameraType;

        Vector2 cursorPosLastFrame = new Vector2(0, 0);
        Vector2 cursorDelta = new Vector2(0, 0);
        float pitchAngle = 0.0f;
        float yawAngle = 0.0f;

        void OnCreate()
        {
            fixedCamera = FindEntityByName("FixedCamera")?.GetComponent<CameraComponent>();
            trackCamera = FindEntityByName("TrackCamera")?.GetComponent<CameraComponent>();
            fpsCamera = FindEntityByName("FPSCamera")?.GetComponent<CameraComponent>();

            fixedCamera?.SetAsRuntimeCamera();
            cameraType = CameraType.Fixed;

            beginTransform = FindEntityByName("TrackBegin")?.GetComponent<TransformComponent>();
            endTransform = FindEntityByName("TrackEnd")?.GetComponent<TransformComponent>();
            fpsTransform = FindEntityByName("FPSCamera")?.GetComponent<TransformComponent>();
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

            }            
        }

        void OnUpdateEditor(float ts)
        {

        }

        void InputDetect()
        {
            if (Input.IsKeyDown(KeyCode.D3))
            {
                fixedCamera?.SetAsRuntimeCamera();
                cameraType = CameraType.Fixed;
            }
            else if (Input.IsKeyDown(KeyCode.D4))
            {
                trackCamera?.SetAsRuntimeCamera();
                cameraType = CameraType.Track;
            }
            else if (Input.IsKeyDown(KeyCode.D5))
            {
                fpsCamera?.SetAsRuntimeCamera();
                cameraType= CameraType.FPS;
                cursorPosLastFrame = Input.GetCursorPos();
            }
        }

        void FPSCameraUpdate(float ts)
        {
            // Rotation
            Vector2 pos = Input.GetCursorPos();

            cursorDelta = pos - cursorPosLastFrame;
            cursorPosLastFrame = pos;

            float cursorX = cursorDelta.X * MouseSensitivity * ts;
            float cursorY = cursorDelta.Y * MouseSensitivity * ts;
            yawAngle += cursorX;
            pitchAngle += cursorY;

            if (pitchAngle > 90f)
                pitchAngle = 90f;
            else if (pitchAngle < -90f)
                pitchAngle = -90f;
            if (yawAngle < 0f) yawAngle += 360f;

            
            Vector3 eulerAngles = Vector3.Up * -cursorX + Vector3.Right * cursorY;
            fpsTransform.Rotate(ref eulerAngles);

            // Translation
            float horizontal = Input.IsKeyDown(KeyCode.A) ? 1f : (Input.IsKeyDown(KeyCode.D) ? -1f : 0f);
            float vertical = Input.IsKeyDown(KeyCode.W) ? -1f : (Input.IsKeyDown(KeyCode.S) ? 1f : 0f);

            Vector3 forward = fpsTransform.Forward();
            Vector3 right = fpsTransform.Right();

            Vector3 deltaTranslation = (forward * vertical + right * horizontal) * MoveSpeed * ts;
            fpsTransform.Translate(ref deltaTranslation);
        }
    }
}
