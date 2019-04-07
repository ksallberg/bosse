#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLU
#define TRAIL 15

#define XAM 50
#define YAM 50
#define SIDE 10
#define CITIES 4

#include <GLFW/glfw3.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

typedef struct Tile {
  float x;
  float y;
  int color; // 0 is grass, 1 is water, 2 is city
  int city_id;
  struct Tile *over;
  struct Tile *below;
  struct Tile *left;
  struct Tile *right;
  /* float a; */
} Tile;

static Tile tiles[YAM][XAM];

void draw_spot(Tile *tile) {

  float x = tile->x;
  float y = tile->y;

  glBegin(GL_POLYGON);
  if(tile->color==0) {
    glColor4f(0, 1, 0, 0.5f);
  } else if (tile->color==10) {
    glColor4f(0, 0, 0, 1.0f);
  } else if (tile->color==11) {
    glColor4f(255, 0, 0, 1.0f);
  } else if (tile->color==12) {
    glColor4f(255, 255, 0, 1.0f);
  } else if (tile->color==13) {
    glColor4f(0, 255, 255, 1.0f);
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
  int cit = 0;
  tiles[i][j].color = 0;
  for(int x = 0; x < 100; x++) {
    mk_terrain1(i, j);
  }

  while(cit < CITIES) {
    int xtile = rand() % XAM;
    int ytile = rand() % YAM;

    if(tiles[ytile][xtile].color == 0) {
      tiles[ytile][xtile].color = 10+cit;
      cit++;
      tiles[ytile][xtile].city_id = cit;
    }
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

  /* When all tiles are created, we can start making refs between them */
  for(int i = 0; i < YAM; i++) {
    for(int j = 0; j < XAM; j ++) {
      Tile *t = &tiles[i][j];

      if(i-1 < 0) {
        t->over = NULL;
      } else {
        t->over = &tiles[i-1][j];
      }

      if(i+1 >= YAM) {
        t->below = NULL;
      } else {
        t->below = &tiles[i+1][j];
      }

      if(j-1 < 0) {
        t->left = NULL;
      } else {
        t->left = &tiles[i][j-1];
      }

      if(j+1 >= XAM) {
        t->right = NULL;
      } else {
        t->right = &tiles[i][j+1];
      }
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

    Tile *rand_tile = &tiles[rand() % YAM][rand() % XAM];
    if(rand_tile->over != NULL) {
      if(rand_tile->over->color >= 10 && rand_tile->color==0) {
        rand_tile->color=rand_tile->over->color;
        rand_tile->city_id = rand_tile->over->city_id;
      }
    }
    if(rand_tile->below != NULL) {
      if(rand_tile->below->color >= 10 && rand_tile->color==0) {
        rand_tile->color=rand_tile->below->color;
        rand_tile->city_id = rand_tile->below->city_id;
      }
    }
    if(rand_tile->left != NULL) {
      if(rand_tile->left->color >= 10 && rand_tile->color==0) {
        rand_tile->color=rand_tile->left->color;
        rand_tile->city_id = rand_tile->left->city_id;
      }
    }
    if(rand_tile->right != NULL) {
      if(rand_tile->right->color >= 10 && rand_tile->color==0) {
        rand_tile->color=rand_tile->right->color;
        rand_tile->city_id = rand_tile->right->city_id;
      }
    }

    for(int i = 0; i < YAM; i ++) {
      for(int j = 0; j < XAM; j ++) {
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
