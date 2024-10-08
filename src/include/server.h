#ifndef SERVER_H
#define SERVER_H

#include <cstdlib>
#include <iostream>

/*
 * You may need to define some global variables for the information of the game map here.
 * Although we don't encourage to use global variables in real cpp projects, you may have to use them because the use of
 * class is not taught yet. However, if you are member of A-class or have learnt the use of cpp class, member functions,
 * etc., you're free to modify this structure.
 */
int rows;         // The count of rows of the game map. You MUST NOT modify its name.
int columns;      // The count of columns of the game map. You MUST NOT modify its name.
int total_mines;  // The count of mines of the game map. You MUST NOT modify its name. You should initialize this
                  // variable in function InitMap. It will be used in the advanced task.
int game_state;  // The state of the game, 0 for continuing, 1 for winning, -1 for losing. You MUST NOT modify its name.
int visit_count;  // Blocks which already been visited.
int marked_mine_count;

char initial_map[65][65];
bool visit_state[65][65];
int mine_count[65][65];

/**
 * @brief The definition of function InitMap()
 *
 * @details This function is designed to read the initial map from stdin. For example, if there is a 3 * 3 map in which
 * mines are located at (0, 1) and (1, 2) (0-based), the stdin would be
 *     3 3
 *     .X.
 *     ...
 *     ..X
 * where X stands for a mine block and . stands for a normal block. After executing this function, your game map
 * would be initialized, with all the blocks unvisited.
 */
int MineCounter(int r, int c) {
  int count = 0;
  if (initial_map[r][c] != '.') {
    return 0;
  }

  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      int next_r = r + i, next_c = c + j;
      if (next_r < 0 || next_r >= rows || next_c < 0 || next_c >= columns) {
        continue;
      }
      if (initial_map[next_r][next_c] != '.') {
        count++;
      }
    }
  }
  return count;
}

void InitMap() {
  std::cin >> rows >> columns;
  total_mines = 0;
  visit_count = 0;
  marked_mine_count = 0;

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {  // initialize the map and the visit state
      std::cin >> initial_map[i][j];
      if (initial_map[i][j] == 'X') {
        total_mines++;
      }
      visit_state[i][j] = 0;
    }
  }

  for (int i = 0; i < rows; ++i) {  // counting the mine number in each block
    for (int j = 0; j < columns; ++j) {
      mine_count[i][j] = MineCounter(i, j);
    }
  }
}

/**
 * @brief The definition of function VisitBlock(int, int)
 *
 * @details This function is designed to visit a block in the game map. We take the 3 * 3 game map above as an example.
 * At the beginning, if you call VisitBlock(0, 0), the return value would be 0 (game continues), and the game map would
 * be
 *     1??
 *     ???
 *     ???
 * If you call VisitBlock(0, 1) after that, the return value would be -1 (game ends and the players loses) , and the
 * game map would be
 *     1X?
 *     ???
 *     ???
 * If you call VisitBlock(0, 2), VisitBlock(2, 0), VisitBlock(1, 2) instead, the return value of the last operation
 * would be 1 (game ends and the player wins), and the game map would be
 *     1@1
 *     122
 *     01@
 *
 * @param r The row coordinate (0-based) of the block to be visited.
 * @param c The column coordinate (0-based) of the block to be visited.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void VisitBlock(int r, int c) {
  if (r < 0 || r >= rows || c < 0 || c >= columns || visit_state[r][c]) {
    return;
  }  // Tests of operation viability.

  visit_state[r][c] = true;

  switch (initial_map[r][c]) {
    case '.': {
      visit_count++;
      if (mine_count[r][c]!=0) {
        break;
      }
      for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
          int next_r = r + i, next_c = c + j;
          if (next_r < 0 || next_r >= rows || next_c < 0 || next_c >= columns) {
            continue;
          }  // In case of visiting invalid array.
          if (initial_map[next_r][next_c] == '.' && !visit_state[next_r][next_c]) {
            VisitBlock(next_r, next_c);
          }
        }
      }
      break;
    }
    // Simulating the auto visit proceed.
    case 'X': {
      game_state = -1;
      break;
    }
    default: {
      break;
    }
  }
  if (visit_count == rows * columns - total_mines && !game_state) {
    game_state = 1;
  }
}

/**
 * @brief The definition of function MarkMine(int, int)
 *
 * @details This function is designed to mark a mine in the game map.
 * If the block being marked is a mine, show it as "@".
 * If the block being marked isn't a mine, END THE GAME immediately. (NOTE: This is not the same rule as the real
 * game) And you don't need to
 *
 * For example, if we use the same map as before, and the current state is:
 *     1?1
 *     ???
 *     ???
 * If you call MarkMine(0, 1), you marked the right mine. Then the resulting game map is:
 *     1@1
 *     ???
 *     ???
 * If you call MarkMine(1, 0), you marked the wrong mine(There's no mine in grid (1, 0)).
 * The game_state would be -1 and game ends immediately. The game map would be:
 *     1?1
 *     X??
 *     ???
 * This is different from the Minesweeper you've played. You should beware of that.
 *
 * @param r The row coordinate (0-based) of the block to be marked.
 * @param c The column coordinate (0-based) of the block to be marked.
 *
 * @note You should edit the value of game_state in this function. Precisely, edit it to
 *    0  if the game continues after visit that block, or that block has already been visited before.
 *    1  if the game ends and the player wins.
 *    -1 if the game ends and the player loses.
 *
 * @note For invalid operation, you should not do anything.
 */
void MarkMine(int r, int c) {
  if(visit_state[r][c])return;
  switch (initial_map[r][c]) {
    case 'X': {
      initial_map[r][c] = '@';
      visit_state[r][c] = true;
      marked_mine_count++;
      break;
    }
      // Change the visit state so that this block won't be involved in VisitBlock operation.
    case '.': {
      game_state = -1;
      initial_map[r][c]='X';
      visit_state[r][c] = true;
      break;
    }
    default: {
      break;
    }
  }
}
/**
 * @brief The definition of function AutoExplore(int, int)
 *
 * @details This function is designed to auto-visit adjacent blocks of a certain block.
 * See README.md for more information
 *
 * For example, if we use the same map as before, and the current map is:
 *     ?@?
 *     ?2?
 *     ??@
 * Then auto explore is available only for block (1, 1). If you call AutoExplore(1, 1), the resulting map will be:
 *     1@1
 *     122
 *     01@
 * And the game ends (and player wins).
 */
void AutoExplore(int r, int c) {
  if (! visit_state[r][c]  || initial_map[r][c] != '.') {
    return;
  }
  bool auto_explore_available = false;
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      int next_r = r + i, next_c = c + j;
      if (next_r < 0 || next_r >= rows || next_c < 0 || next_c >= columns) {
        continue;
      }
      if (initial_map[next_r][next_c] == 'X') {
        auto_explore_available = true;
      }
    }
  }
  if (!auto_explore_available) {
    for (int i = -1; i <= 1; ++i) {
      for (int j = -1; j <= 1; ++j) {
        int next_r = r + i, next_c = c + j;
        if (next_r < 0 || next_r >= rows || next_c < 0 || next_c >= columns) {
          continue;
        }
        VisitBlock(next_r, next_c);
      }
    }
  }
}
/**
 * @brief The definition of function ExitGame()
 *
 * @details This function is designed to exit the game.
 * It outputs a line according to the result, and a line of two integers, visit_count and marked_mine_count,
 * representing the number of blocks visited and the number of marked mines taken respectively.
 *
 * @note If the player wins, we consider that ALL mines are correctly marked.
 */
void ExitGame() {
  if (game_state == 1) {
    std::cout << "YOU WIN!\n" << visit_count << " " << total_mines;
  } else {
    std::cout << "GAME OVER!\n" << visit_count << " " << marked_mine_count;
  }
  exit(0);  // Exit the game immediately
}

/**
 * @brief The definition of function PrintMap()
 *
 * @details This function is designed to print the game map to stdout. We take the 3 * 3 game map above as an
 * example. At the beginning, if you call PrintMap(), the stdout would be
 *    ???
 *    ???
 *    ???
 * If you call VisitBlock(2, 0) and PrintMap() after that, the stdout would be
 *    ???
 *    12?
 *    01?
 * If you call VisitBlock(0, 1) and PrintMap() after that, the stdout would be
 *    ?X?
 *    12?
 *    01?
 * If the player visits all blocks without mine and call PrintMap() after that, the stdout would be
 *    1@1
 *    122
 *    01@
 * (You may find the global variable game_state useful when implementing this function.)
 *
 * @note Use std::cout to print the game map, especially when you want to try the advanced task!!!
 */

void PrintMap() {
  if (game_state == 1) {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < columns; ++j) {
        if (initial_map[i][j] == '.') {
          std::cout << mine_count[i][j];
        } else {
          std::cout << '@';
        }
      }
      std::cout << "\n";
    }
  } else {
    for (int i = 0; i < rows; ++i) {
      for (int j = 0; j < columns; ++j) {
        if (visit_state[i][j] ) {
          if (initial_map[i][j] == '.' ) {
            std::cout << mine_count[i][j];
          } else {
            std::cout << initial_map[i][j];
          }
        } else {
          std::cout << "?";
        }
      }
      std::cout << "\n";
    }
  }
}
#endif
