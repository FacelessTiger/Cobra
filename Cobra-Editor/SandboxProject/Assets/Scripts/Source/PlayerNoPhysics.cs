using System;
using Ellis;

namespace Sandbox
{
    public class PlayerNoPhysics : Entity
    {
        void OnCreate()
        {
            Console.WriteLine($"Player.OnCreate - {ID}");
        }

        void OnUpdate(float ts)
        {
            float translationSpeed = 10.0f;
            float rotationSpeed = 5.0f;
            float scaleSpeed = 4.0f;

            Vector3 velocity = Vector3.Zero;
            Vector3 rotation = Vector3.Zero;
            Vector3 scale = Vector3.Zero;

            if (Input.IsKeyDown(KeyCode.W))
                velocity.Y = 1.0f;
            else if (Input.IsKeyDown(KeyCode.S))
                velocity.Y = -1.0f;
            if (Input.IsKeyDown(KeyCode.A))
                velocity.X = -1.0f;
            else if (Input.IsKeyDown(KeyCode.D))
                velocity.X = 1.0f;

            if (Input.IsKeyDown(KeyCode.Q))
                rotation.Z += 1.0f;
            else if (Input.IsKeyDown(KeyCode.E))
                rotation.Z -= 1.0f;

            if (Input.IsKeyDown(KeyCode.I))
                scale.Y += 1.0f;
            else if (Input.IsKeyDown(KeyCode.K))
                scale.Y -= 1.0f;
            if (Input.IsKeyDown(KeyCode.J))
                scale.X += 1.0f;
            else if (Input.IsKeyDown(KeyCode.L))
                scale.X -= 1.0f;

            velocity *= translationSpeed;
            rotation *= rotationSpeed;
            scale *= scaleSpeed;

            Translation += velocity * ts;
            Rotation += rotation * ts;
            Scale += scale * ts;
        }
    }
}