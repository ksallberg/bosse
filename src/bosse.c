#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLU
#define TRAIL 15

#define XAM 50
#define YAM 50
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

  float x = tile->x;
  float y = tile->y;

  glBegin(GL_POLYGON);
  if(tile->color==0) {
    glColor4f(0, 1, 0, 1.0f);
  } else {
    glColor4f(0, 0, 1, 1.0f);
  }
  glVertex3f(x+SIDE, y+SIDE, 0);
  glVertex3f(x, y+SIDE, 0);
  glVertex3f(x, y, 0);
  glVertex3f(x+SIDE, y, 0);
  glEnd();
  glFlush();
}

void mk_terrain1(int i, int j) {
  int way = rand() % 50;
  int way2 = way%4;

  if(way == 4) {
    return;
  } else if(i < 0) {
    return;
  } else if(j < 0) {
    return;
  } else if(i >= XAM) {
    return;
  } else if(j >= YAM) {
    return;
  }

  tiles[i][j].color = 0;

  if(way2 == 3) {
    mk_terrain1(i-1, j);
  } else if(way2 == 2) {
    mk_terrain1(i, j-1);
  } else if(way2 == 1) {
    mk_terrain1(i+1, j);
  } else if(way2 == 0) {
    mk_terrain1(i, j+1);
  }
}

void mk_terrain() {
  int i = YAM/2;
  int j = XAM/2;
  tiles[i][j].color = 0;
  for(int x = 0; x < 100; x++) {
    mk_terrain1(i, j);
  }
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
    mk_terrain();
  } else if (key == GLFW_KEY_ESCAPE) {
    printf("Hej hej inte space\n");
  }
}

int main(int argc, char** argv) {

  GLFWwindow* window;
  int i = 0;
  int windowWidth = XAM * SIDE;
  int windowHeight = YAM * SIDE;

  double previousTime = glfwGetTime();
  int frameCount = 0;

  srand(time(NULL));

  /* Initialize the library */
  if ( !glfwInit() ) {
     return -1;
  }

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(windowWidth, windowHeight, "tiles", NULL, NULL);

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
      int col = 1;
      Tile t = {j*SIDE,i * SIDE, col};
      tiles[i][j] = t;
    }
  }

  mk_terrain();

  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    int width, height;

    // Measure speed
    double currentTime = glfwGetTime();
    frameCount++;
    // If a second has passed.
    if ( currentTime - previousTime >= 1.0 )
    {
        frameCount = 0;
        previousTime = currentTime;
    }

    glfwGetFramebufferSize(window, &width, &height);
    glClear(GL_COLOR_BUFFER_BIT);

    for(int i = 0; i < YAM; i ++) {
      for(int j = 0; j < XAM; j ++) {
        /* int r = rand() % 100; */
        /* if (r == 2) { */
        /*   tiles[i][j].color = 0; */
        /* } */
        draw_spot(&tiles[i][j]);
      }
    }

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
