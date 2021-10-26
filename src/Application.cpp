#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
            std::cout << "glew not ok!" << std::endl;
    }
    else {
        std::cout << glGetString(GL_VERSION) << std::endl;
    }
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
        GLCall(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb,layout);


       
        IndexBuffer ib(indices, 6);

        //正交投影变换，可以把物体映射到屏幕的平面，注意ortho不包含近大远小，用于2d画面（横板卷轴？）
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));//camera视角变换

        Shader shader = Shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform1i("u_Texture", 0);
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        
        shader.Unbind();

        
        Texture texture("res/textures/logo.png");
        texture.Bind();
        
        va.Unbind();
        vb.Unbind();
        ib.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        glm::vec3 translationA(400, 400, 0);
        glm::vec3 translationB(200, 200, 0);

        float r = 0.0f;
        float increment = 0.05;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);//模型本身的变换
                glm::mat4 mvp = proj * view * model;
                shader.Bind();//设置uniform前需要编译，写在loop里面会影响性能，但是现在为了演示写到了这里
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }
            {
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);//模型本身的变换
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;
            r += increment;
            {
                
                ImGui::SliderFloat3("Translate A", &translationA.x, 0.0f, 960.0f);
                ImGui::SliderFloat3("Translate B", &translationB.x, 0.0f, 960.0f);
                
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}