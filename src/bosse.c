#define GLFW_INCLUDE_GLCOREARB
#define GLFW_INCLUDE_GLU
#define GL_SILENCE_DEPRECATION

#define XAM 70
#define YAM 70
#define SIDE 10
#define CITIES 7

#define IS_CAPITAL 1
#define IS_FRIENDLY 1 << 2

#define IS_GOOD 0
#define IS_BAD 1
#define IS_WAR 2

#define GREEN 0
#define BLUE 1

#define FAST 1

#include <GLFW/glfw3.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdio.h>

typedef struct Tile {
  float x;
  float y;
  int color; // 0 is grass, 1 is water, 2 is city
  int city_id;
  int flags;
  struct Tile *over;
  struct Tile *below;
  struct Tile *left;
  struct Tile *right;
  /* float a; */
} Tile;

typedef struct Relation {
  struct Tile *from;
  struct Tile *to;
  int type; // IS_GOOD=0, IS_BAD=1, IS_WAR=2
} Relation;

static Tile tiles[YAM][XAM];
static Relation relations[100];
static Tile *capitals[CITIES];
extern int next_rel = 0;

void draw_spot(Tile *tile) {
  if(tile->color == BLUE) {
    return;
  }

  float x = tile->x;
  float y = tile->y;

  glBegin(GL_POLYGON);
  if (tile->color==0) {
    glColor4f(0, 1, 0, 0.5f);
  } else if (tile->color==10) {
    glColor4f(1, 0, 1, 1.0f);
  } else if (tile->color==11) {
    glColor4f(255, 100, 0, 0.5f);
  } else if (tile->color==12) {
    glColor4f(255, 255, 0, .4f);
  } else if (tile->color==13) {
    glColor4f(0, 34, 155, .7f);
  } else if (tile->color==14) {
    glColor4f(0, 255, 25, 1.0f);
  } else if (tile->color==15) {
    glColor4f(35, 25, 255, .4f);
  } else if (tile->color==16) {
    glColor4f(64, 35, 205, .7f);
  } else if (tile->color==17) {
    glColor4f(75, 255, 25, .4f);
  }
  glVertex3f(x+SIDE, y+SIDE, 0);
  glVertex3f(x, y+SIDE, 0);
  glVertex3f(x, y, 0);
  glVertex3f(x+SIDE, y, 0);
  glEnd();
  glFlush();

  // draw a box around the capital
  if ((tile->flags & IS_CAPITAL) == IS_CAPITAL) {
    glLineWidth(1);
    glBegin(GL_LINES);

    if((tile->flags & IS_FRIENDLY) == IS_FRIENDLY) {
      glColor3f(.0f, 1.0f, .0f);
    } else {
      glColor3f(1.0f, .0f, .0f);
    }

    glVertex3f(x+SIDE, y+SIDE, 0);
    glVertex3f(x, y+SIDE, 0);
    glVertex3f(x, y, 0);
    glVertex3f(x+SIDE, y, 0);

    glVertex3f(x, y, 0);
    glVertex3f(x, y+SIDE, 0);

    glVertex3f(x+SIDE, y+SIDE, 0);
    glVertex3f(x+SIDE, y, 0);

    glVertex3f(x, y, 0);
    glVertex3f(x+SIDE, y+SIDE, 0);

    glEnd();
    glFlush();
  }
}

void draw_legend() {

  for(int i = 0; i < CITIES; i++) {
    float x = 0;
    float y = i*SIDE;

    glBegin(GL_POLYGON);
    if (i==0) {
      glColor4f(1, 0, 1, 1.0f);
    } else if (i==1) {
      glColor4f(255, 100, 0, 0.5f);
    } else if (i==2) {
      glColor4f(255, 255, 0, .4f);
    } else if (i==3) {
      glColor4f(0, 34, 155, .7f);
    } else if (i==4) {
      glColor4f(0, 255, 25, 1.0f);
    } else if (i==5) {
      glColor4f(35, 25, 255, .4f);
    } else if (i==6) {
      glColor4f(64, 35, 205, .7f);
    } else if (i==7) {
      glColor4f(75, 255, 25, .4f);
    }
    glVertex3f(x+SIDE, y+SIDE, 0);
    glVertex3f(x, y+SIDE, 0);
    glVertex3f(x, y, 0);
    glVertex3f(x+SIDE, y, 0);
    glEnd();
  }
}

void draw_spot_x(Tile *tile) {

  float x = tile->x;
  float y = tile->y;

  glBegin(GL_POLYGON);
  glColor4f(0, 0, 0, 1.0f);
  glVertex3f(x+SIDE, y+SIDE, 0);
  glVertex3f(x, y+SIDE, 0);
  glVertex3f(x, y, 0);
  glVertex3f(x+SIDE, y, 0);
  glEnd();
  glFlush();
}

void draw_water() {

  float x = 0;
  float y = 0;

  glBegin(GL_POLYGON);
  glColor4f(0, 0, 1, 0.6f);
  glVertex3f(x+SIDE*XAM, y+SIDE*YAM, 0);
  glVertex3f(x, y+SIDE*YAM, 0);
  glVertex3f(x, y, 0);
  glVertex3f(x+SIDE*XAM, y, 0);
  glEnd();
  glFlush();
}

void draw_relation(Relation *rel) {
  float from_x = rel->from->x;
  float from_y = rel->from->y;
  float to_x = rel->to->x;
  float to_y = rel->to->y;
  float spacer = 5.0f;

  glLineWidth(3);
  if (rel->type == 0) {
    glColor4f(1.0, 1.0, 1.0, 0.5f);
  } else if(rel->type == 1) {
    glColor4f(0.0, 0.0, 0.0, 0.5f);
  } else if(rel->type == 2) {
    glColor4f(1.0, 0.0, 0.0, 0.5f);
  }

  glBegin(GL_LINES);
  glVertex2f(from_x + spacer, from_y + spacer);
  glVertex2f(to_x + spacer, to_y + spacer);
  glEnd();
}

void initialize() {
  /* Initialize tiles */
  for(int i = 0; i < YAM; i ++) {
    for(int j = 0; j < XAM; j ++) {
      int col = 1;
      Tile t = {};
      t.x=j*SIDE;
      t.y=i*SIDE;
      t.color=col;
      t.flags = 0; //initialize flags
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
}

void mk_terrain1(int i, int j) {
  int way = rand() % XAM;
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
      capitals[cit] = &tiles[ytile][xtile];
      cit++;
      tiles[ytile][xtile].city_id = cit;
      tiles[ytile][xtile].flags |= IS_CAPITAL;
      if(rand()%2 == 0) {
        tiles[ytile][xtile].flags |= IS_FRIENDLY;
      }
    }
  }
}

int cnt_col(int col_id) {
  int acc = 0;
  for(int i = 0; i < YAM; i ++) {
    for(int j = 0; j < XAM; j ++) {
      if(tiles[i][j].color == col_id) {
        acc ++;
      }
    }
  }
  return acc;
}

int create_rel(Tile *tile) {
  int rand_cit = rand() % CITIES;
  if((tile->flags & IS_CAPITAL) == IS_CAPITAL) {
    // If the capital has only one tile, it is not allowed
    int num_tiles = cnt_col(tile->color);
    int num_tiles2 = cnt_col(capitals[rand_cit]->color);
    if(num_tiles == 1 || num_tiles2 == 1) {
      return 0;
    }
    // if there is already a relation between these two capitals
    // then skip this
    for(int i = 0; i < next_rel; i ++) {
      if(relations[i].to == capitals[rand_cit] && relations[i].from == tile) {
        return 0;
      }
      if(relations[i].from == capitals[rand_cit] && relations[i].to == tile) {
        return 0;
      }
    }
    // otherwise, create a relation
    Relation r = {tile, capitals[rand_cit]};
    int friendly1 = (capitals[rand_cit]->flags & IS_FRIENDLY) == IS_FRIENDLY;
    int friendly2 = (tile->flags & IS_FRIENDLY) == IS_FRIENDLY;
    if(friendly1 && friendly2) {
      r.type = 0;
    } else {
      r.type = rand() % 2;
    }
    relations[next_rel] = r;
    next_rel ++;
    return 1;
  }
  return 0;
}

void change_rels() {
  for(int i = 0; i < next_rel; i ++) {
    if(relations[i].type == IS_WAR) {
      int chance = rand() % 1000;
      if(chance == 1) {
        relations[i].type = rand() % 2;
      }
    } else {
      int friendly1 = (relations[i].to->flags & IS_FRIENDLY) == IS_FRIENDLY;
      int friendly2 = (relations[i].from->flags & IS_FRIENDLY) == IS_FRIENDLY;
      if(!friendly1 || !friendly2) {
        relations[i].type = 1;
      }
    }
  }
}

void wipe_rels() {
  int chance = rand() % 1000;
  if(chance == 555) {
    next_rel = 0;
  }
}

int is_war(Tile *t1, Tile *t2) {
  for(int i = 0; i < next_rel; i ++) {
    if(relations[i].type == IS_WAR) {
      if(relations[i].to == t1 & relations[i].from == t2) {
        return 1;
      }
      if(relations[i].from == t1 && relations[i].to == t2) {
        return 1;
      }
    }
  }
  return 0;
}

Tile *capital_of(Tile *t) {
  for(int i = 0; i < CITIES; i ++) {
    if(capitals[i]->color == t->color) {
      return capitals[i];
    }
  }
  /* printf("SKA INTE KOMMA HIT!\n"); */
  return NULL;
}

// compare is the city that we compare the random tile
// against
int expand1(Tile *rand_tile, Tile *compare) {
  if(compare != NULL) {
    // If this (rand_tile)  is an empty grass tile next to a city
    if(compare->color >= 10 && rand_tile->color==GREEN) {
      rand_tile->color=compare->color;
      return 1;
    }
    if(rand_tile->color >= 10 && compare->color >= 10) {
      // war logic
      Tile *cap1 = capital_of(rand_tile);
      Tile *cap2 = capital_of(compare);
      if(is_war(cap1, cap2)) {
        int freak = rand() % 2;
        int cap1size = cnt_col(rand_tile->color);
        int cap2size = cnt_col(compare->color);
        if(freak == 0) {
          if(cap1size < cap2size) {
            if((rand_tile->flags & IS_CAPITAL) == 0) {
              rand_tile->color = compare->color;
              rand_tile->city_id = compare->city_id;
              return 1;
            }
          }
        } else {
          if(cap1size > 5 && (compare->flags & IS_CAPITAL) == 0) {
            compare->color = rand_tile->color;
            compare->city_id = rand_tile->city_id;
            return 1;
          }
        }
      }
    } else {
      return 0;
    }
  }
  return 0;
}

int expand(Tile *rand_tile) {
  Tile *compare = rand_tile->over;
  int acc;
  acc = expand1(rand_tile, rand_tile->over);
  acc += expand1(rand_tile, rand_tile->below);
  acc += expand1(rand_tile, rand_tile->left);
  acc += expand1(rand_tile, rand_tile->right);
  return acc > 0;
}

int war() {
  // for each bad relation
  for(int i = 0; i < next_rel; i ++) {
    if(relations[i].type == 1) {
      int chance = rand() % 1000;
      if(chance == 3) {
        relations[i].type = 2;
        return 1;
      }
    }
  }
  return 0;
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
        tiles[i][j].flags=0;
      }
    }
    next_rel=0;
    mk_terrain();
  } else if (key == GLFW_KEY_ESCAPE) {
    for(int i = 0; i < CITIES; i++) {
      int acc = cnt_col(capitals[i]->color);
      printf("City #%d has %d tiles.\n", i, acc);
    }
    printf("_______________\n");
  }
}

int main(int argc, char** argv) {

  GLFWwindow* window;
  int i = 0;
  int windowWidth = XAM * SIDE;
  int windowHeight = YAM * SIDE;

  double previousTime = glfwGetTime();

  srand(time(NULL));
  next_rel = 0;

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

  initialize();
  mk_terrain();

  while (!glfwWindowShouldClose(window)) {
    /* Render here */
    int width, height;

    glfwGetFramebufferSize(window, &width, &height);
    glClear(GL_COLOR_BUFFER_BIT);
    draw_water();
    Tile *rand_tile;
    rand_tile = &tiles[rand() % YAM][rand() % XAM];
    while(rand_tile->color==1) {
      rand_tile = &tiles[rand() % YAM][rand() % XAM];
    }
    create_rel(rand_tile);
    war();
    change_rels();
    /* wipe_rels(); */

    if(FAST == 1) {
      int exp = expand(rand_tile);
      int max_tries = 0;
      while(exp == 0) {
        rand_tile = &tiles[rand() % YAM][rand() % XAM];
        while(rand_tile->color==1) {
          rand_tile = &tiles[rand() % YAM][rand() % XAM];
        }
        exp = expand(rand_tile);
        max_tries ++;
        if(max_tries >= 100) {
          break;
        }
      }
    } else {
      expand(rand_tile);
    }

    for(int i = 0; i < YAM; i ++) {
      for(int j = 0; j < XAM; j ++) {
        draw_spot(&tiles[i][j]);
      }
    }

    for(int i = 0; i < next_rel; i ++) {
      draw_relation(&relations[i]);
    }

    draw_legend();
    /* draw_spot_x(rand_tile); */

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
