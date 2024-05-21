class Camera {
public:
    glm::mat4 proj;
    glm::mat4 view;
    glm::vec2 pos;

    void set_proj() {
        proj = glm::ortho(0.0f, 32.0f*40.0f, 0.0f, 32.0f*21.0f, 0.0f, 100.0f);
    }

    void set_view() {
        glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
        view = glm::lookAt(
            glm::vec3(pos.x, pos.y, 20.0f),
            camera_front + glm::vec3(pos.x, pos.y, 0.0f),
            camera_up
        );
    }
};
