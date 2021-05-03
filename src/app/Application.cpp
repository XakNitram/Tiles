#include "pch.hpp"
#include "GLDebug.hpp"
#include "Common.hpp"
#include "Core/Window.hpp"
#include "World/Landscape.hpp"

#include <chrono>
#include <fstream>

using namespace lwvl::debug;
using namespace std::chrono;


class Application {
    Window m_window;

public:
    Application(uint32_t width, uint32_t height) : m_window(width, height, "Tiles") {}

    int run() {
        GLEventListener listener(
            [](
                Source source, Type type,
                Severity severity, unsigned int id, int length,
                const char *message, const void *userState
            ) {
                std::cout << "[OpenGL] " << message << std::endl;
            }
        );

        const float aspect = static_cast<float>(m_window.config.width) / static_cast<float>(m_window.config.height);
        world::Landscape landscape(aspect);

        auto secondStart = high_resolution_clock::now();
        for (uint32_t frameCount = 0; !m_window.shouldClose(); frameCount++) {
            Window::update();
            while (std::optional<Event> possible = m_window.pollEvent()) {
                if (!possible.has_value()) {
                    continue;
                }

                Event &concrete = possible.value();
                if (concrete.type == Event::Type::KeyRelease) {
                    KeyboardEvent &key_event = std::get<KeyboardEvent>(concrete.event);
                    if (key_event.key == GLFW_KEY_ESCAPE) {
                        m_window.shouldClose(true);
                    }
                }
            }

            lwvl::clear();
            landscape.draw();
            m_window.swapBuffers();

            if ((frameCount & 63) == 0) {
                auto secondEnd = high_resolution_clock::now();
                auto duration = 0.000001 * static_cast<double>(
                    duration_cast<microseconds>(secondEnd - secondStart).count()
                );

                double fps = static_cast<double>(frameCount) / duration;
                std::cout << "Average framerate for last " << frameCount << " frames : " << fps << std::endl;

                // Reset time variables.
                secondStart = high_resolution_clock::now();
                frameCount = 0;
            }
        }

        return 0;
    }
};


int main() {
    try {
        Application app(384, 384);
        return app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}
