#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <queue>
#include <utility>

extern int rows;         // The count of rows of the game map.
extern int columns;      // The count of columns of the game map.
extern int total_mines;  // The count of mines of the game map.

char current_map[65][65];
bool search_state[65][65];  // Used in local search ,if the block is visited, set it to true.
std::queue<std::pair<int, int> > visit_queue;
int virtual_map[65][65];  // Used in local search ,after each step update the mine state.

// You MUST NOT use any other external variables except for rows, columns and total_mines.

/**
 * @brief The definition of function Execute(int, int, bool)
 *
 * @details This function is designed to take a step when player the client's (or player's) role, and the implementation
 * of it has been finished by TA. (I hope my comments in code would be easy to understand T_T) If you do not understand
 * the contents, please ask TA for help immediately!!!
 *
 * @param r The row coordinate (0-based) of the block to be visited.
 * @param c The column coordinate (0-based) of the block to be visited.
 * @param type The type of operation to a certain block.
 * If type == 0, we'll execute VisitBlock(row, column).
 * If type == 1, we'll execute MarkMine(row, column).
 * If type == 2, we'll execute AutoExplore(row, column).
 * You should not call this function with other type values.
 */
void Execute(int r, int c, int type);

/**
 * @brief The definition of function InitGame()
 *
 * @details This function is designed to initialize the game. It should be called at the beginning of the game, which
 * will read the scale of the game map and the first step taken by the server (see README).
 */
int MarkedMineCounter(int r, int c) {
  int count = 0;
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      int next_r = r + i, next_c = c + j;
      if (next_r < 0 || next_r >= rows || next_c < 0 || next_c >= columns) {
        continue;
      }
      if (current_map[next_r][next_c] == '@') {
        count++;
      }
    }
  }
  return count;
}
int PileCounter(int r, int c) {
  int count = 0;
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      int next_r = r + i, next_c = c + j;
      if (next_r < 0 || next_r >= rows || next_c < 0 || next_c >= columns) {
        continue;
      }
      if (current_map[next_r][next_c] == '?') {
        count++;
      }
    }
  }
  return count;
}

void InitGame() {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      current_map[i][j] = 0;
    }
  }
  int first_row, first_column;
  std::cin >> first_row >> first_column;
  Execute(first_row, first_column, 0);
}

/**
 * @brief The definition of function ReadMap()
 *
 * @details This function is designed to read the game map from stdin when playing the client's (or player's) role.
 * Since the client (or player) can only get the limited information of the game map, so if there is a 3 * 3 map as
 * above and only the block (2, 0) has been visited, the stdin would be
 *     ???
 *     12?
 *     01?
 */
void ReadMap() {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      std::cin >> current_map[i][j];
    }
  }
}

/**
 * @brief The definition of function Decide()
 *
 * @details This function is designed to decide the next step when playing the client's (or player's) role. Open up your
 * mind and make your decision here!
 */
bool SillyLogic() {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < columns; ++j) {
      if (current_map[i][j] <= '0' || current_map[i][j] > '9') {
        continue;
      }
      int marked_mine_count = MarkedMineCounter(i, j), pile_count = PileCounter(i, j);
      if (!pile_count) {
        continue;
      }
      // If there's no pile, no need to operate.
      if (marked_mine_count == current_map[i][j] - '0') {
        Execute(i, j, 2);
        return true;
      }
      // All mines are marked,do autoclick.
      if (pile_count + marked_mine_count == current_map[i][j] - '0') {
        for (int next_r = i - 1; next_r <= i + 1; next_r++) {
          for (int next_c = j - 1; next_c <= j + 1; next_c++) {
            if (next_r < 0 || next_r >= rows || next_c < 0 || next_c >= columns) {
              continue;
            }
            if (current_map[next_r][next_c] == '?') {
              Execute(next_r, next_c, 1);
              return true;
            }
          }
        }
      }  // Pile number equals mine number, do markmine.
    }
  }
  return false;
}
void LuckyGuess() {
  srand(time(NULL));
  for (int i = 1; i <= 500; ++i) {
    int r = rand() % rows, c = rand() % columns;
    if (current_map[r][c] == '?') {
      for (int next_r = r - 1; next_r <= r + 1; next_r++) {
        for (int next_c = c - 1; next_c <= c + 1; next_c++) {
          if (next_r < 0 || next_r >= rows || next_c < 0 || next_c >= columns) {
            continue;
          }
          if (current_map[next_r][next_c] <= '0' || current_map[next_r][next_c] > '9') {
            continue;
          }
          double left_mine = current_map[next_r][next_c] - '0' - MarkedMineCounter(next_r, next_c);
          double left_pile = PileCounter(next_r, next_c);
          if (0.75 - left_mine / left_pile < 0) {
            Execute(r, c, 1);
            return;
          }
          if (0.3 - left_mine / left_pile > 0) {
            Execute(r, c, 0);
            return;
          }
        }
      }
    }
  }
  while (1) {
    int r = rand() % rows, c = rand() % columns;
    if (current_map[r][c] == '?') {
      Execute(r, c, 0);
      return;
    }
  }
}

/*
void Influence(int r,int c){
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      int next_r = r + i, next_c = c + j;
      if (next_r < 0 || next_r >= rows || next_c < 0 || next_c >= columns) {
        continue;
      }
      if (current_map[next_r][next_c] >='0'&&current_map[next_r][next_c]<='9'&&!search_state[next_r][next_c]) {
        visit_queue.push(std::make_pair(next_r, next_c));
      }
    }
  }
}
//Influence the blocks around after a mine is virtually set.
void SearchMachine(){
  while(!visit_queue.empty()){
    int r=visit_queue.front().first,c=visit_queue.front().second;

  }
}
//
bool LocalSearch(){
  for(int i=0;i<rows;++i){
    for(int j=0;j<columns;++j){
      if(current_map[i][j]>'0'&&current_map[i][j]<='9'&&!search_state[i][j]){
          visit_queue.push(std::make_pair(i,j));
          SearchMachine();
      }
    }
  }
  return false;
}
*/
void Decide() {
  bool state = false;
  state = SillyLogic();
  if (!state) {
    LuckyGuess();
  }
  // if(!state){
  //   LocalSearch();
  // }
  // while (true) {
  //   Execute(0, 0);
  // }
}

#endif