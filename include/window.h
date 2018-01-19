#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void window_setup(void);
void window_update(void);
void window_draw(void);
void window_poll_events(void);
int window_should_close(void);
void window_exit(void);
void window_key_cb(
    GLFWwindow* w,
    int key,
    int scancode,
    int action,
    int mods
);

#endif
