#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_sdl2.h"
#include "../imgui/imgui_impl_opengl3.h"

#include <stdio.h>

#include "shader.cpp"
#include "camera.cpp"

const char* vertex_src =
    "#version 330\n"
    "layout(location = 0) in vec3 position;\n"
    "layout(location = 1) in vec4 color;\n"
    "layout(location = 2) in vec2 texcoords;\n"
    "uniform mat4 proj;\n"
    "uniform mat4 view;\n"
    "out vec4 fvertex_color;\n"
    "out vec2 ftexcoords;\n"
    "void main() {\n"
    "   gl_Position = proj * view * vec4(position, 1.0);\n"
    "   fvertex_color = color;\n"
    "   ftexcoords = texcoords;\n"
    "}\n"
    ""
    ;

const char* fragment_src =
    "#version 330\n"
    "in vec4 fvertex_color;\n"
    "in vec2 ftexcoords;\n"
    "out vec4 out_color;\n"
    "uniform sampler2D tex;\n"
    "void main() {\n"
    "   out_color = texture(tex, ftexcoords);\n"
    "}\n"
    ""
    ;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window* window = SDL_CreateWindow(
        "OpenGL",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        360,
        SDL_WINDOW_OPENGL
    );

    SDL_GLContext glctx = SDL_GL_CreateContext(window);
    printf("%s\n", glGetString(GL_VERSION));

    glewExperimental = true;
    glewInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplSDL2_InitForOpenGL(window, glctx);
    ImGui_ImplOpenGL3_Init();

    SDL_Surface* png = IMG_Load("res/img.png");
    if (!png) {
        fprintf(stderr, "Unable to load png: %s\n", IMG_GetError());
        return 1;
    }

    float size = 300.0f;
    float vertices[] = {
        0.0f, size, 0.0f,      1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
        size, size, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f,
        size, 0.0f, 0.0f,      0.0f, 0.0f, 1.0f, 1.0f,    1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,        1.0f, 0.0f, 0.0f, 1.0f,    0.0f, 1.0f,
    };

    int vert_indexes[] = {
        3, 2, 1,
        1, 0, 3,
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vertices_vbo;
    glGenBuffers(1, &vertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    GLuint ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(vert_indexes),
        vert_indexes,
        GL_STATIC_DRAW
    );

    int position_floats = 3;
    int color_floats = 4;
    int texcoord_floats = 2;

    int total_floats = position_floats + color_floats + texcoord_floats;
    int stride = total_floats*sizeof(float);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        stride,
        0
    );
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        stride,
        (void*)(position_floats*sizeof(float))
    );
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        stride,
        (void*)((position_floats + color_floats)*sizeof(float))
    );
    glBindVertexArray(0);

    ShaderProgram shader;
    if (!shader.init_from_strings(vertex_src, fragment_src)) return 1;

    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        png->w,
        png->h,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        png->pixels
    );
    SDL_FreeSurface(png);

    Camera camera(window);
    camera.set_proj();
    camera.set_view();

    float begin_time = SDL_GetTicks()/1000.0f;
    float end_time = begin_time;
    float dt = -1.0f;

    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            ImGui_ImplSDL2_ProcessEvent(&e);
            if (e.type == SDL_QUIT) running = false;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        camera.pos.x -= dt * 50.0f;
        camera.set_view();
        camera.set_proj();
        ImGui::Text("%f", 1.0f/dt);

        shader.use();
        shader.upload_mat4("proj", camera.proj);
        shader.upload_mat4("view", camera.view);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
        end_time = SDL_GetTicks()/1000.0f;
        dt = end_time - begin_time;
        begin_time = end_time;
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    IMG_Quit();
    SDL_Quit();
}
