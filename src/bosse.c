#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLU
#define TRAIL 15

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

void draw_spot(float x0, float yy0) {

  float side = 50.0f;
  float x = side + x0;
  float y = side + yy0;

  glBegin(GL_POLYGON);
  glColor4f(1, 1, 0, 100);
  glVertex3f(x+side, y+side, 0);
  glVertex3f(x-side, y+side, 0);
  glVertex3f(x-side, y-side, 0);
  glVertex3f(x+side, y-side, 0);
  glEnd();
  glFlush();
}

static void key_callback(GLFWwindow *window,
                         int key,
                         int scancode,
                         int action,
                         int mods) {
  if (key == GLFW_KEY_SPACE) {
    printf("Hej hej space\n");
  } else if (key == GLFW_KEY_ESCAPE) {
    printf("Hej hej inte space\n");
  }
}

int main(int argc, char** argv) {

  GLFWwindow* window;
  int xpos = -1, ypos;
  int i = 0;
  int windowWidth = 1280;
  int windowHeight = 720;

  double previousTime = glfwGetTime();
  int frameCount = 0;

  /* Initialize the library */
  if ( !glfwInit() ) {
     return -1;
  }

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);

  if (!window) {
     glfwTerminate();
     return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);
  glfwSetKeyCallback(window, key_callback);
  /* Loop until the user closes the window */

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0f, windowWidth, windowHeight, 0.0f, 0.0f, 1.0f);

  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    int width, height;

    // Measure speed
    double currentTime = glfwGetTime();
    frameCount++;
    // If a second has passed.
    if ( currentTime - previousTime >= 1.0 )
    {
        // Display the frame count here any way you want.
        printf("FPS: %d\n", frameCount);

        frameCount = 0;
        previousTime = currentTime;
    }

    #ifdef __APPLE__
      usleep(14000);
    #endif

    glfwGetFramebufferSize(window, &width, &height);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_spot(100, 100);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();

    if(xpos == -1) {
      glfwGetWindowPos(window, &xpos, &ypos);
      glfwSetWindowPos(window, xpos+1, ypos);
      glfwSetWindowPos(window, xpos, ypos);
    }
  }

  glfwTerminate();
  return 0;
}
