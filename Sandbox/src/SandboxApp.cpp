#include <Ellis.h>
#include <Platform/OpenGL/OpenGLShader.h>

#include <imgui/imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Ellis::Layer
{
private:
	Ellis::ShaderLibrary m_ShaderLibrary;
	Ellis::Ref<Ellis::VertexArray> m_VertexArray;
	Ellis::Ref<Ellis::Shader> m_Shader;
	
	Ellis::Ref<Ellis::VertexArray> m_SquareVA;
	Ellis::Ref<Ellis::Shader> m_FlatColorShader;

	Ellis::Ref<Ellis::Texture2D> m_Texture, m_ChernoLogoTexture;

	Ellis::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;

	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray = Ellis::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Ellis::Ref<Ellis::VertexBuffer> vertexBuffer = Ellis::VertexBuffer::Create(vertices, sizeof(vertices));
		Ellis::BufferLayout layout = {
			{ Ellis::ShaderDataType::Float3, "a_Position" },
			{ Ellis::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Ellis::Ref<Ellis::IndexBuffer> indexBuffer = Ellis::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Ellis::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ellis::Ref<Ellis::VertexBuffer> squareVB = Ellis::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ Ellis::ShaderDataType::Float3, "a_Position" },
			{ Ellis::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ellis::Ref<Ellis::IndexBuffer> squareIB = Ellis::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader = Ellis::Shader::Create("VertexPosColor", vertexSource, fragmentSource);

		std::string flatColorShaderVertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatColorShaderFragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = Ellis::Shader::Create("FlatColor", flatColorShaderVertexSource, flatColorShaderFragmentSource);
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Ellis::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = Ellis::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Ellis::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Ellis::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Ellis::Timestep ts) override
	{
		if (Ellis::Input::IsKeyPressed(EL_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Ellis::Input::IsKeyPressed(EL_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Ellis::Input::IsKeyPressed(EL_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Ellis::Input::IsKeyPressed(EL_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Ellis::Input::IsKeyPressed(EL_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		else if (Ellis::Input::IsKeyPressed(EL_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Ellis::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Ellis::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Ellis::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Ellis::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Ellis::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Ellis::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Ellis::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		Ellis::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		//Ellis::Renderer::Submit(m_Shader, m_VertexArray);
		Ellis::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Ellis::Event& event) override
	{
		
	}
};

class Sandbox : public Ellis::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Ellis::Application* Ellis::CreateApplication()
{
	return new Sandbox();
}