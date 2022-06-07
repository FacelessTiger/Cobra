#include <Ellis.h>
#include <imgui/imgui.h>

class ExampleLayer : public Ellis::Layer
{
private:
	std::shared_ptr<Ellis::VertexArray> m_VertexArray;
	std::shared_ptr<Ellis::Shader> m_Shader;

	std::shared_ptr<Ellis::VertexArray> m_SquareVA;
	std::shared_ptr<Ellis::Shader> m_BlueShader;

	Ellis::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;

	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 180.0f;
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Ellis::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Ellis::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Ellis::VertexBuffer::Create(vertices, sizeof(vertices)));
		Ellis::BufferLayout layout = {
			{ Ellis::ShaderDataType::Float3, "a_Position" },
			{ Ellis::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Ellis::IndexBuffer> indexBuffer;
		indexBuffer.reset(Ellis::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Ellis::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Ellis::VertexBuffer> squareVB;
		squareVB.reset(Ellis::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Ellis::ShaderDataType::Float3, "a_Position" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Ellis::IndexBuffer> squareIB;
		squareIB.reset(Ellis::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Shader.reset(new Ellis::Shader(vertexSource, fragmentSource));

		std::string blueShaderVertexSource = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string blueShaderFragmentSource = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new Ellis::Shader(blueShaderVertexSource, blueShaderFragmentSource));
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
		Ellis::Renderer::Submit(m_BlueShader, m_SquareVA);
		Ellis::Renderer::Submit(m_Shader, m_VertexArray);
		Ellis::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		
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