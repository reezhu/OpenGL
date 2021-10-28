#include "TestTexture2D.h"
#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
    test::TestTexture2D::TestTexture2D()
        :m_proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
            m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),            
            m_translationA(200, 200, 0), m_translationB(400, 400, 0)
	{
        float positions[] = {
         -50.0f, -50.0f, 0.0f, 0.0f,
          50.0f, -50.0f, 1.0f, 0.0f,
          50.0f,  50.0f, 1.0f, 1.0f,
         -50.0f,  50.0f, 0.0f, 1.0f,
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        
        m_VAO = std::make_unique<VertexArray>();
       
        m_VertexBuffer= std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Texture= std::make_unique<Texture>("res/textures/logo.png");
        m_Shader->SetUniform1i("u_Texture",0);

	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}
	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f,0.0f,0.0f,0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        Renderer renderer;
        m_Texture->Bind();
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationA);//模型本身的变换
            glm::mat4 mvp = m_proj * m_view * model;
            m_Shader->Bind();//设置uniform前需要编译，写在loop里面会影响性能，但是现在为了演示写到了这里
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO,* m_IndexBuffer, *m_Shader);
        }
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationB);//模型本身的变换
            glm::mat4 mvp = m_proj * m_view * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);
            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}
	void TestTexture2D::OnImGuiRender()
	{
        ImGui::SliderFloat3("Translate A", &m_translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translate B", &m_translationB.x, 0.0f, 960.0f);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
