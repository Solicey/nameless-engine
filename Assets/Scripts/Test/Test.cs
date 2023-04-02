using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using NL;

namespace Test
{
    public class Test : Entity
    {
        private TransformComponent m_Transform;

        public float Speed;
        public float Time = 5.0f;

        void OnCreate()
        {
            // Console.WriteLine($"Player.OnCreate - {ID}");

            m_Transform = GetComponent<TransformComponent>();
            
        }

        void OnUpdateRuntime(float ts)
        {
            Time += ts;
            // Console.WriteLine($"Player.OnUpdate: {ts}");

            float speed = Speed;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            velocity *= speed * ts;

            Vector3 translation = m_Transform.Translation;
            translation += velocity * ts;
            m_Transform.Translation = translation;
        }


        void OnUpdateEditor(float ts)
        {
            // Console.WriteLine($"Player.OnUpdateEditor: {ts}");
        }
    }
}
