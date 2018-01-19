#include <window.h>
#include <stdio.h>
#include <machine.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ZOOM 8

uint8_t tex_data[DISP_HEIGHT][DISP_WIDTH];
GLFWwindow* window;

const int keymap[16] = {
    GLFW_KEY_X,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_Q,
    GLFW_KEY_W,
    GLFW_KEY_E,
    GLFW_KEY_A,
    GLFW_KEY_S,
    GLFW_KEY_D,
    GLFW_KEY_Z,
    GLFW_KEY_C,
    GLFW_KEY_4,
    GLFW_KEY_R,
    GLFW_KEY_F,
    GLFW_KEY_V,
};

void window_setup(void)
{
    glfwInit();

    window = glfwCreateWindow(
        DISP_WIDTH * ZOOM,
        DISP_HEIGHT * ZOOM,
        "chip-seven",
        NULL,
        NULL
    );

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, window_key_cb);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    GLuint tex, fb;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glGenFramebuffers(1, &fb);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);
    glFramebufferTexture2D(
        GL_READ_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        tex,
        0
    );

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void window_update(void)
{
    window_draw();
    window_poll_events();
}

void window_draw(void)
{
    for(size_t byte_i = 0; byte_i < DISP_SIZE; byte_i++){
        for(size_t bit_i = 0; bit_i < 8; bit_i++){
            size_t x = 8 * (byte_i % DISP_SIZE_X) + bit_i;
            size_t y = (DISP_HEIGHT - 1) - (byte_i / DISP_SIZE_X);

            uint8_t disp_bit = (disp[byte_i] >> (7 - bit_i)) & 1;

            tex_data[y][x] = disp_bit ? 0xFF : 0;
        }
    }

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        64, 32,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE_3_3_2,
        tex_data
    );

    glBlitFramebuffer(
        0, 0, 64, 32,
        0, 0, 64*ZOOM, 32*ZOOM,
        GL_COLOR_BUFFER_BIT,
        GL_NEAREST
    );

    glfwSwapBuffers(window);
}

void window_poll_events(void)
{
    glfwPollEvents();
}

int window_should_close(void)
{
    return glfwWindowShouldClose(window);
}

void window_exit(void)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void window_key_cb(
    GLFWwindow* w,
    int key,
    int scancode,
    int action,
    int mods
)
{
    (void)w;
    (void)scancode;
    (void)mods;

    for(size_t i = 0; i < 16; i++){
        if(key == keymap[i]){
            if(action == GLFW_PRESS){
                keys |= 1 << i;
            }else if(action == GLFW_RELEASE){
                keys &= ~(1 << i);
            }

            return;
        }
    }
}
