using System.Runtime.InteropServices;
using System;

namespace Cobra
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

        public Vector3 Scale
        {
            get
            {
                InternalCalls.TransformComponent_GetScale(Entity.ID, out Vector3 scale);
                return scale;
            }
            set
            {
                InternalCalls.TransformComponent_SetScale(Entity.ID, ref value);
            }
        }

        public Matrix4 GetTransform()
        {
            return new Matrix4(InternalCalls.TransformComponent_GetTransform(Entity.ID));
        }
    }

    public class SpriteRendererComponent : Component
    {
        public Vector4 Color
        {
            get
            {
                InternalCalls.SpriteRendererComponent_GetColor(Entity.ID, out Vector4 color);
                return color;
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetColor(Entity.ID, ref value);
            }
        }

        public float TilingFactor
        {
            get
            {
                InternalCalls.SpriteRendererComponent_GetTilingFactor(Entity.ID, out float tilingFactor);
                return tilingFactor;
            }
            set
            {
                InternalCalls.SpriteRendererComponent_SetTilingFactor(Entity.ID, ref value);
            }
        }
    }

    public class CircleRendererComponent : Component
    {
        public Vector4 Color
        {
            get
            {
                InternalCalls.CircleRendererComponent_GetColor(Entity.ID, out Vector4 color);
                return color;
            }
            set
            {
                InternalCalls.CircleRendererComponent_SetColor(Entity.ID, ref value);
            }
        }

        public float Thickness
        {
            get
            {
                InternalCalls.CircleRendererComponent_GetThickness(Entity.ID, out float thickness);
                return thickness;
            }
            set
            {
                InternalCalls.CircleRendererComponent_SetThickness(Entity.ID, ref value);
            }
        }

        public float Fade
        {
            get
            {
                InternalCalls.CircleRendererComponent_GetFade(Entity.ID, out float fade);
                return fade;
            }
            set
            {
                InternalCalls.CircleRendererComponent_SetFade(Entity.ID, ref value);
            }
        }
    }

    public class CameraComponent : Component
    {
        public Matrix4 GetProjection()
        {
            return new Matrix4(InternalCalls.CameraComponent_GetProjection(Entity.ID));
        }
    }

    public class Rigidbody2DComponent : Component
    {
        public enum BodyType { Static = 0, Dynamic, Kinematic };

        public Vector2 LinearVelocity
        {
            get
            {
                InternalCalls.Rigidbody2DComponent_GetLinearVelocity(Entity.ID, out Vector2 velocity);
                return velocity;
            }
        }

        public BodyType Type
        {
            get => InternalCalls.Rigidbody2DComponent_GetType(Entity.ID);
            set => InternalCalls.Rigidbody2DComponent_SetType(Entity.ID, value);
        }

        public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
        }

        public void ApplyLinearImpulse(Vector2 impulse, bool wake)
        {
            InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
        }
    }

    public class TextComponent : Component
    {
        public string Text
        {
            get => InternalCalls.TextComponent_GetText(Entity.ID);
            set => InternalCalls.TextComponent_SetText(Entity.ID, value);
        }

        public float Kerning
        {
            get
            {
                InternalCalls.TextComponent_GetKerning(Entity.ID, out float kerning);
                return kerning;
            }
            set
            {
                InternalCalls.TextComponent_SetKerning(Entity.ID, ref value);
            }
        }

        public float LineSpacing
        {
            get
            {
                InternalCalls.TextComponent_GetLineSpacing(Entity.ID, out float lineSpacing);
                return lineSpacing;
            }
            set
            {
                InternalCalls.TextComponent_SetLineSpacing(Entity.ID, ref value);
            }
        }

        public Vector4 Color
        {
            get
            {
                InternalCalls.TextComponent_GetColor(Entity.ID, out Vector4 color);
                return color;
            }
            set
            {
                InternalCalls.TextComponent_SetColor(Entity.ID, ref value);
            }
        }
    }
}
