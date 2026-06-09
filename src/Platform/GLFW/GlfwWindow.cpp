module;

#include <GLFW/glfw3.h>
#include <tracy/Tracy.hpp>

module Nodens.Window;

import Nodens.Events;
import Nodens.GraphicsContext;
import Nodens.Log;
import Nodens.OpenGLContext;
import std;

namespace Nodens
{

class GlfwWindow : public Window
{
public:
    GlfwWindow(const WindowProps& props) { Init(props); }

    ~GlfwWindow() override {}

    void OnUpdate() override
    {
        ZoneScoped;

        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    unsigned int GetWidth() const override { return m_Data.Width; }
    unsigned int GetHeight() const override { return m_Data.Height; }

    void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
    void SetVSync(bool enabled) override
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool IsVSync() const override { return m_Data.VSync; }

    void* GetNativeWindow() const override { return m_Window; }

private:
    void Init(const WindowProps& props);
    void Shutdown();

private:
    GLFWwindow*      m_Window  = nullptr;
    GraphicsContext* m_Context = nullptr;

    struct WindowData
    {
        std::string  Title;
        unsigned int Width;
        unsigned int Height;
        bool         VSync;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char* description)
{
    CoreLogger().error("GLFW Error ({}): {}", error, description);
}

Window* Window::Create(const WindowProps& props)
{
    return new GlfwWindow(props);
}

void GlfwWindow::Init(const WindowProps& props)
{
    ZoneScoped;

    m_Data.Title  = props.Title;
    m_Data.Width  = props.Width;
    m_Data.Height = props.Height;
    m_Data.VSync  = props.VSync;

    CoreLogger().info("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized)
    {
        // TODO: glfwTerminate on system shutdown
        int succes = glfwInit();
        if (!succes)
            FatalCore("Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);

        // Set OpenGL version to 4.6
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

        glfwWindowHint(GLFW_SAMPLES, 4);

        // Use Core Profile
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

    m_Context = new OpenGLContext(m_Window);
    m_Context->Init();

    glfwSetWindowUserPointer(m_Window, &m_Data);
    // This function assigns the WindowData struct to the GLFWwindow object.
    // This allows us to retrieve the WindowData from the GLFWwindow* by
    // using the function glfwGetWindowUserPointer(GLFWwindow*) and casting
    // it to WindowData.

    SetVSync(props.VSync);

    //----------------------------------------------------------------------------
    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window,
                              [](GLFWwindow* window, int width, int height)
                              {
                                  WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                                  data.Width       = width;
                                  data.Height      = height;

                                  WindowResizeEvent event(width, height);
                                  data.EventCallback(event);
                                  // EventCallback is a void function that recieves an Event&.
                                  // So this line is executing the callback function assigned by
                              });

    glfwSetWindowCloseCallback(m_Window,
                               [](GLFWwindow* window)
                               {
                                   WindowData&      data = *(WindowData*)glfwGetWindowUserPointer(window);
                                   WindowCloseEvent event;
                                   data.EventCallback(event);
                               });

    glfwSetKeyCallback(m_Window,
                       [](GLFWwindow* window, int key, int scancode, int action, int mods)
                       {
                           WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                           switch (action)
                           {
                           case GLFW_PRESS:
                           {
                               KeyPressedEvent event(key, 0);
                               data.EventCallback(event);
                               break;
                           }
                           case GLFW_RELEASE:
                           {
                               KeyReleasedEvent event(key);
                               data.EventCallback(event);
                               break;
                           }
                           case GLFW_REPEAT:
                           {
                               KeyPressedEvent event(key, 1);
                               data.EventCallback(event);
                               break;
                           }
                           }
                       });

    glfwSetMouseButtonCallback(m_Window,
                               [](GLFWwindow* window, int button, int action, int mods)
                               {
                                   WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                                   switch (action)
                                   {
                                   case GLFW_PRESS:
                                   {
                                       MouseButtonPressedEvent event(button);
                                       data.EventCallback(event);
                                       break;
                                   }
                                   case GLFW_RELEASE:
                                   {
                                       MouseButtonReleasedEvent event(button);
                                       data.EventCallback(event);
                                       break;
                                   }
                                   }
                               });

    glfwSetScrollCallback(m_Window,
                          [](GLFWwindow* window, double xOffset, double yOffset)
                          {
                              WindowData&        data = *(WindowData*)glfwGetWindowUserPointer(window);
                              MouseScrolledEvent event((float)xOffset, (float)yOffset);
                              data.EventCallback(event);
                          });

    glfwSetCursorPosCallback(m_Window,
                             [](GLFWwindow* window, double xPos, double yPos)
                             {
                                 WindowData&     data = *(WindowData*)glfwGetWindowUserPointer(window);
                                 MouseMovedEvent event((float)xPos, (float)yPos);
                                 data.EventCallback(event);
                             });
}

void GlfwWindow::Shutdown()
{
    glfwDestroyWindow(m_Window);
}
} // namespace Nodens
