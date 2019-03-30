#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLU
#define TRAIL 15

#define XAM 25
#define YAM 25
#define SIDE 10

#include <GLFW/glfw3.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

typedef struct Tile {
  float x;
  float y;
  int color;
  /* float a; */
} Tile;

static Tile tiles[YAM][XAM];

void draw_spot(Tile *tile) {

  float x = SIDE + tile->x;
  float y = SIDE + tile->y;

  glBegin(GL_POLYGON);
  if(tile->color==0) {
    glColor4f(0, 1, 0, 1.0f);
  } else {
    glColor4f(0, 0, 1, 1.0f);
  }
  glVertex3f(x+SIDE, y+SIDE, 0);
  glVertex3f(x-SIDE, y+SIDE, 0);
  glVertex3f(x-SIDE, y-SIDE, 0);
  glVertex3f(x+SIDE, y-SIDE, 0);
  glEnd();
  glFlush();
}

static void key_callback(GLFWwindow *window,
                         int key,
                         int scancode,
                         int action,
                         int mods) {
  if (key == GLFW_KEY_SPACE) {
    for(int i = 0; i < YAM; i ++) {
      for(int j = 0; j < XAM; j ++) {
        tiles[i][j].color=1;
      }
    }
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

  srand(time(NULL));

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

  /* Initialize tiles */
  for(int i = 0; i < YAM; i ++) {
    for(int j = 0; j < XAM; j ++) {
      int col = (i + j) % 2;
      Tile t = {j * SIDE * 2, i * SIDE * 2, col};
      tiles[i][j] = t;
    }
  }

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

    for(int i = 0; i < YAM; i ++) {
      for(int j = 0; j < XAM; j ++) {
        int r = rand() % 100;
        if (r == 2) {
          tiles[i][j].color = 0;
        }
        draw_spot(&tiles[i][j]);
      }
    }

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
