#include <iostream>
#include <filesystem>
#include <random>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "rendering/renderer2d.h"
#include "rendering/texture.h"
#include "rendering/font.h"
#include "math/rectangle.h"

#define FLAPPY_BIRD_JUMP_VELOCITY_Y -5.0f
#define FLAPPY_BIRD_JUMP_ROTATION -20.0f

#define PILLAR_INTERVAL 514.5f
#define PILLAR_GAP 250.0f

void center_window(GLFWwindow *window)
{
    int window_width, window_height;
    glfwGetWindowSize(window, &window_width, &window_height);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwSetWindowPos(window, (mode->width - window_width) / 2, (mode->height - window_height) / 2);
}

struct PillarSet
{
    glm::vec2 offset;
    bool point_gathered;
};

float animate_float(float from_value, float to_value, float complete_at, float progress)
{
    float progress_normalized = progress / complete_at;

    if (progress_normalized >= 1)
        return to_value;

    float progress_value = (abs(to_value - from_value)) * (to_value > from_value ? progress_normalized : 1 - progress_normalized);

    return progress_value;
}

void flappy_bird_jump(float &flappy_bird_velocity_y, float &flappy_bird_rotation)
{
    flappy_bird_velocity_y = FLAPPY_BIRD_JUMP_VELOCITY_Y;
    flappy_bird_rotation = FLAPPY_BIRD_JUMP_ROTATION;
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        std::cerr << "ERROR: failed to initialize GLFW.\n";
        return -1;
    }

    window = glfwCreateWindow(1712, 963, "Flappy Bird", NULL, NULL);

    if (!window)
    {
        std::cerr << "ERROR: failed to create a window.\n";
        glfwTerminate();
        return -1;
    }

    center_window(window);

    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (!gladLoadGL())
    {
        std::cerr << "ERROR: failed to initialize glad.\n";
        return -1;
    }

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> dist(-250, 250);

    renderer::initialize(glm::vec2(1920.0f, 1080.0f));

    Font font = font_new("resources/fonts/OpenSans-Medium.ttf", 72);

    Texture flappy_bird = texture_new("resources/textures/flappy_bird.png");
    Texture flappy_bird_background = texture_new("resources/textures/flappy_bird_background.png");
    Texture flappy_bird_pillar = texture_new("resources/textures/flappy_bird_pillar.png");

    std::vector<PillarSet> pillar_sets;

    for (int i = 0; i < 4; i++)
    {
        pillar_sets.push_back(
            PillarSet
            {
                .offset = glm::vec2(600.0f + (renderer::render_size().x - flappy_bird_pillar.size.x) - (PILLAR_INTERVAL * i), dist(rng)),
                .point_gathered = false
            }
        );
    }

    float flappy_bird_y = renderer::render_size().y / 2.0f - flappy_bird.size.y / 2.0f;
    float flappy_bird_velocity_y = 1;
    float flappy_bird_rotation = 0;

    int points = 0;

    bool started = false;
    bool failed = false;

    // todo:
    //       none

    bool space_pressed = false;
    bool space_pressed_in_previous_frame = false;
    bool space_just_pressed = false;

    float fail_time;

    float previous_time;
    
    while (!glfwWindowShouldClose(window))
    {
        float current_time = glfwGetTime();
        float delta_time = (current_time - previous_time) * 240;
        previous_time = current_time;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // INPUT
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            break;
        }

        space_pressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        space_just_pressed = !space_pressed_in_previous_frame && space_pressed;

        space_pressed_in_previous_frame = space_pressed;

        // UPDATE
        int window_width, window_height;
        glfwGetWindowSize(window, &window_width, &window_height);

        glViewport(0, 0, window_width, window_height);

        if (!started)
        {
            if (space_just_pressed)
            {
                started = true;
                flappy_bird_jump(flappy_bird_velocity_y, flappy_bird_rotation);
            }
        }
        else
        {
            for (PillarSet &pillar_set : pillar_sets)
            {
                if (!failed)
                {
                    pillar_set.offset.x -= 1.5f;
                }

                if (!failed && !pillar_set.point_gathered && 300.0f + flappy_bird.size.x / 2.0f > pillar_set.offset.x + flappy_bird_pillar.size.x / 2.0f)
                {
                    points++;
                    pillar_set.point_gathered = true;
                }

                if (pillar_set.offset.x + flappy_bird_pillar.size.x <= 0)
                {
                    pillar_set.offset.x = renderer::render_size().x;
                    pillar_set.offset.y = dist(rng);
                    pillar_set.point_gathered = false;
                }

                Rectangle flappy_bird_rectangle = Rectangle
                {
                    .position = glm::vec2(300.0f, flappy_bird_y),
                    .size = flappy_bird.size
                };

                if (
                    !failed &&
                    (
                        rectangles_intersect(
                            flappy_bird_rectangle,
                            Rectangle
                            {
                                .position = glm::vec2(pillar_set.offset.x, -flappy_bird_pillar.size.y + renderer::render_size().y / 2.0f - PILLAR_GAP / 2.0f + pillar_set.offset.y),
                                .size = flappy_bird_pillar.size
                            }
                        ) ||
                        rectangles_intersect(
                            flappy_bird_rectangle,
                            Rectangle
                            {
                                .position = glm::vec2(pillar_set.offset.x, renderer::render_size().y / 2.0f + PILLAR_GAP / 2.0f + pillar_set.offset.y),
                                .size = flappy_bird_pillar.size
                            }
                        )
                    )
                )
                {
                    fail_time = glfwGetTime();
                    failed = true;
                    flappy_bird_jump(flappy_bird_velocity_y, flappy_bird_rotation);
                }
            }

            flappy_bird_rotation += 0.357f * delta_time;

            if (flappy_bird_y < renderer::render_size().y)
            {
                flappy_bird_velocity_y += 0.0881f * delta_time;
                flappy_bird_y += flappy_bird_velocity_y * delta_time;
            }
            else
            {
                flappy_bird_rotation = 0.0f;
            }

            if (space_just_pressed && !failed)
            {
                flappy_bird_jump(flappy_bird_velocity_y, flappy_bird_rotation);
            }

            if (!failed && flappy_bird_y + flappy_bird.size.y > renderer::render_size().y)
            {
                fail_time = glfwGetTime();
                failed = true;
                flappy_bird_jump(flappy_bird_velocity_y, flappy_bird_rotation);
            }
        }

        // RENDER
        renderer::immediate_quad(glm::vec2(0.0f), flappy_bird_background);

        for (const PillarSet &pillar_set : pillar_sets)
        {
            renderer::immediate_quad(glm::vec2(pillar_set.offset.x, -flappy_bird_pillar.size.y + renderer::render_size().y / 2.0f - PILLAR_GAP / 2.0f + pillar_set.offset.y), flappy_bird_pillar, glm::vec2(1.0f), 180.0f);
            renderer::immediate_quad(glm::vec2(pillar_set.offset.x,                              renderer::render_size().y / 2.0f + PILLAR_GAP / 2.0f + pillar_set.offset.y), flappy_bird_pillar);
        }

        renderer::immediate_quad(glm::vec2(300.0f, flappy_bird_y), flappy_bird, flappy_bird_rotation);

        float points_text_y = !failed ? 200.0f : renderer::render_size().y / 2.0f - measure_text(font, std::to_string(points)).y / 2.0f - measure_text(font, std::to_string(points)).y - 10.0f;

        if (started)
            renderer::text(glm::vec2(renderer::render_size().x / 2.0f - measure_text(font, std::to_string(points)).x / 2.0f, points_text_y), font, glm::vec4(1.0f), std::to_string(points));

        if (failed)
        {
            std::string_view game_over_text = "game over";
            renderer::text(renderer::render_size() / 2.0f - measure_text(font, game_over_text) / 2.0f, font, glm::vec4(0.8f, 0.05f, 0.3f, 1.0f), game_over_text);

            float progress = glfwGetTime() - fail_time;
            float flash_alpha = animate_float(0.8f, 0.0f, 0.4f, progress);

            if (flash_alpha != 0)
                renderer::immediate_quad(glm::vec2(0.0f), renderer::render_size(), glm::vec4(1.0f, 1.0f, 1.0f, flash_alpha));
        }

        if (!started)
        {
            std::string_view start_text = "press space to start the game";
            renderer::text(renderer::render_size() / 2.0f - measure_text(font, start_text) / 2.0f, font, glm::vec4(1.0f), start_text);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}
