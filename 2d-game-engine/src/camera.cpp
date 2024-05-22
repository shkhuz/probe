class Camera {
public:
    SDL_Window* window;

    glm::mat4 proj;
    glm::mat4 view;
    glm::vec2 pos;

    Camera(SDL_Window* window) {
        this->window = window;
    }

    void set_proj() {
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.0f, 100.0f);
    }

    void set_view() {
        glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
        view = glm::lookAt(
            glm::vec3(pos.x, pos.y, 20.0f),
            glm::vec3(pos.x, pos.y, -1.0f),
            camera_up
        );
    }
};
