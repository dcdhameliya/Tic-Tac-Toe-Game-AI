#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// The size of the Tic Tac Toe board
const int BOARD_SIZE = 3;

// The possible values in the Tic Tac Toe board
const char PLAYER_X = 'X';
const char PLAYER_O = 'O';
const char EMPTY = '-';

// A struct representing a move
struct Move {
  int row, col;
};

// A struct representing a game state
struct GameState {
  char board[BOARD_SIZE][BOARD_SIZE];
  char player;

  GameState() {
    for (int i = 0; i < BOARD_SIZE; i++) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        board[i][j] = EMPTY;
      }
    }
    player = PLAYER_X;
  }

  bool operator == (const GameState & other) const {
    for (int i = 0; i < BOARD_SIZE; i++) {
      for (int j = 0; j < BOARD_SIZE; j++) {
        if (board[i][j] != other.board[i][j]) {
          return false;
        }
      }
    }
    return true;
  }
};

// Check if the game is over (i.e., if there is a winner or the board is full)
bool game_over(const GameState & state) {
  // Check rows
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (state.board[i][0] != EMPTY && state.board[i][0] == state.board[i][1] && state.board[i][1] == state.board[i][2]) {
      return true;
    }
  }

  // Check columns
  for (int j = 0; j < BOARD_SIZE; j++) {
    if (state.board[0][j] != EMPTY && state.board[0][j] == state.board[1][j] && state.board[1][j] == state.board[2][j]) {
      return true;
    }
  }

  // Check diagonals
  if (state.board[0][0] != EMPTY && state.board[0][0] == state.board[1][1] && state.board[1][1] == state.board[2][2]) {
    return true;
  }
  if (state.board[0][2] != EMPTY && state.board[0][2] == state.board[1][1] && state.board[1][1] == state.board[2][0]) {
    return true;
  }

  // Check if the board is full
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (state.board[i][j] == EMPTY) {
        return false;
      }
    }
  }

  // If we get here, the board is full and there is no winner
  return true;
}

// Evaluate the game state
int evaluate(const GameState & state) {
  // Check rows
  for (int i = 0; i < BOARD_SIZE; i++) {
    if (state.board[i][0] != EMPTY && state.board[i][0] == state.board[i][1] && state.board[i][1] == state.board[i][2]) {
      if (state.board[i][0] == PLAYER_X) {
        return 1;
      } else {
        return -1;
      }
    }
  }

  // Check columns
  for (int j = 0; j < BOARD_SIZE; j++) {
    if (state.board[0][j] != EMPTY && state.board[0][j] == state.board[1][j] && state.board[1][j] == state.board[2][j]) {
      if (state.board[0][j] == PLAYER_X) {
        return 1;
      } else {
        return -1;
      }
    }
  }

  // Check diagonals
  if (state.board[0][0] != EMPTY && state.board[0][0] == state.board[1][1] && state.board[1][1] == state.board[2][2]) {
    if (state.board[0][0] == PLAYER_X) {
      return 1;
    } else {
      return -1;
    }
  }
  if (state.board[0][2] != EMPTY && state.board[0][2] == state.board[1][1] && state.board[1][1] == state.board[2][0]) {
    if (state.board[0][2] == PLAYER_X) {
      return 1;
    } else {
      return -1;
    }
  }

  // If we get here, the board is not full and there is no winner
  return 0;
}

// Get all possible moves from a given game state
vector < Move > get_possible_moves(const GameState & state) {
  vector < Move > moves;
  for (int i = 0; i < BOARD_SIZE; i++) {
    for (int j = 0; j < BOARD_SIZE; j++) {
      if (state.board[i][j] == EMPTY) {
        Move move;
        move.row = i;
        move.col = j;
        moves.push_back(move);
      }
    }
  }
  return moves;
}

// Make a move in a given game state
GameState make_move(const GameState & state,
  const Move & move) {
  GameState new_state = state;
  new_state.board[move.row][move.col] = state.player;
  if (state.player == PLAYER_X) {
    new_state.player = PLAYER_O;
  } else {
    new_state.player = PLAYER_X;
  }
  return new_state;
}

// Undo a move in a given game state
GameState undo_move(const GameState & state,
  const Move & move) {
  GameState new_state = state;
  new_state.board[move.row][move.col] = EMPTY;
  if (state.player == PLAYER_X) {
    new_state.player = PLAYER_O;
  } else {
    new_state.player = PLAYER_X;
  }
  return new_state;
}

// The alpha-beta pruning minimax algorithm
int minimax(const GameState & state, int depth, int alpha, int beta, bool maximize) {
  if (depth == 0 || game_over(state)) {
    return evaluate(state);
  }

  if (maximize) {
    int max_value = numeric_limits < int > ::min();
    for (const Move & move: get_possible_moves(state)) {
      GameState new_state = make_move(state, move);
      int value = minimax(new_state, depth - 1, alpha, beta, false);
      max_value = max(max_value, value);
      alpha = max(alpha, value);
      if (beta <= alpha) {
        break;
      }
    }
    return max_value;
  } else {
    int min_value = numeric_limits < int > ::max();
    for (const Move & move: get_possible_moves(state)) {
      GameState new_state = make_move(state, move);
      int value = minimax(new_state, depth - 1, alpha, beta, true);
      min_value = min(min_value, value);
      beta = min(beta, value);
      if (beta <= alpha) {
        break;
      }
    }
    return min_value;
  }
}

// The main function that uses the minimax algorithm with alpha-beta pruning to find the best move
Move find_best_move(const GameState & state, int depth) {
  int best_value = numeric_limits < int > ::min();
  Move best_move;
  for (const Move & move: get_possible_moves(state)) {
    GameState new_state = make_move(state, move);
    int value = minimax(new_state, depth - 1, numeric_limits < int > ::min(), numeric_limits < int > ::max(), false);
    if (value > best_value) {
      best_value = value;
      best_move = move;
    }
  }
  return best_move;
}


// Print the current state of the board
void print_board(const GameState& state) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (state.board[i][j] == PLAYER_X) {
                cout << "X ";
            } else if (state.board[i][j] == PLAYER_O) {
                cout << "O ";
            } else {
                cout << "- ";
            }
        }
        cout << endl;
    }
}

// The main function that plays the game
int main() {
  GameState state;
  state.player = PLAYER_X;
  while (!game_over(state)) {
    if (state.player == PLAYER_X) {
      cout << "Player X's turn" << endl;
      Move move = find_best_move(state, 8);
      state = make_move(state, move);
      print_board(state);
    } else {
      cout << "Player O's turn" << endl;
      int row, col;
      cout << "Enter row and column (0-2): ";
      cin >> row >> col;
      Move move = {
        row,
        col
      };

      state = make_move(state, move);
    
      print_board(state);
    }
  }
  int winner = evaluate(state);
  if (winner == 1) {
    cout << "Player X wins!" << endl;
  } else if (winner == -1) {
    cout << "Player O wins!" << endl;
  } else {
    cout << "It's a tie!" << endl;
  }
  return 0;
}