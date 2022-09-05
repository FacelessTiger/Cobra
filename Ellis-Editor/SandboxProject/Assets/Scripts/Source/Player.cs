using System;
using Ellis;

namespace Sandbox
{
    public class Player : Entity
    {
        private Rigidbody2DComponent m_Rigidbody;

        public float Speed;
        public float Time = 0.0f;

        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");
            Console.WriteLine($"Player.Speed - {Speed}");

            m_Rigidbody = GetComponent<Rigidbody2DComponent>();
        }

        void OnUpdate(float ts)
        {
            Time += ts;
            Vector3 velocity = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;

            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            velocity *= Speed;
            m_Rigidbody.ApplyLinearImpulse(velocity.XY * ts, true);
        }
    }
}