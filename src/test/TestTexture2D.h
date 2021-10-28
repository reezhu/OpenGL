#pragma once
#include "Test.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
namespace test{
	class TestTexture2D:public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();
		void OnUpdate(float deltaTime)override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr < IndexBuffer> m_IndexBuffer;
		std::unique_ptr < Shader> m_Shader;
		std::unique_ptr < Texture> m_Texture;
		glm::mat4 m_proj, m_view;
		glm::vec3 m_translationA, m_translationB;
	};
}

