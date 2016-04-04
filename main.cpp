// g++ -O3 -std=c++11 main.cpp -o gcc-8-queen
// or
// clang++ -O3 -std=c++11 main.cpp -o clang-8-queen
//
// Copyrighted by hb4ch, 10142130156 ECNU CS dept.

#include <cassert>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>

int N = 8;

uint64_t file_1 = 0xff;
uint64_t file_2 = file_1 << 8;
uint64_t file_3 = file_2 << 8;
uint64_t file_4 = file_3 << 8;
uint64_t file_5 = file_4 << 8;
uint64_t file_6 = file_5 << 8;
uint64_t file_7 = file_6 << 8;
uint64_t file_8 = file_7 << 8;

uint64_t bit_mask[8];

// Optional stuff to play with in case I'm bored and wanna write a chess
// program.
const uint64_t full = 0xFFFFFFFFFFFFFFFF;

int solu_count = 0;

using namespace std;

struct Bitboard {
  uint64_t A;
};

set<vector<vector<int>>> solution;
set<uint64_t> bit_solution;

void print_list(vector<int> &list, int n);
void bfs();
void gen_board(vector<int> &slots, vector<vector<int>> &board);
int bit_locate(Bitboard *bb, int x, int y);
void bit_set(Bitboard *bb, int x, int y, int value);
void bit_print_board(Bitboard bb);
bool bit_iden(Bitboard a, Bitboard b);
void print_board(vector<vector<int>> &board);
inline int q_count(vector<vector<int>> &board);
inline void BIT_MASK(Bitboard *mask, int x, int y);
inline void MASK(vector<vector<int>> &mask, int x, int y);
int bit_legit(Bitboard bb, int x, int y);

bool is_solution(const vector<vector<int>> &board, const vector<int> &slots) {
  vector<int> row(N, 0);
  vector<vector<int>> mask(N, row);

  MASK(mask, 0, slots[0]);

  for (int i = 0; i < N - 1; i++) {
    if (mask[i + 1][slots[i + 1]] == -1)
      return false;
    MASK(mask, i + 1, slots[i + 1]);
  }

  return true;
}

void gen_board(vector<int> &slots, vector<vector<int>> &board) {
  for (int i = 0; i < N; i++) {
    board[i][slots[i]] = 1;
  }
}

void print_list(vector<int> &list, int n) {
  for (int i = 0; i < N; i++) {
    cout << list[i] << " ";
  }
  cout << endl;
}

void bfs() {
  // an iterative approach. should it be faster.
  vector<int> slots(N, 0);

  int index = 0;
  while (true) {

    vector<vector<int>> current;
    vector<int> row(N, 0);

    for (int i = 0; i < N; i++)
      current.push_back(row);

    gen_board(slots, current);

    // cout << "slots" << endl;
    // print_list(slots, N);

    if (is_solution(current, slots)) {
      solution.insert(current);
      print_board(current);
    }

    slots[0]++;
    while (slots[index] == N) {
      // Overflow, we're done
      if (index == N - 1)
        return;

      slots[index++] = 0;
      slots[index]++;
    }
    index = 0;
  }
}

int bit_locate(Bitboard *bb, int x, int y) {
  x = 7 - x;
  y = 7 - y;
  uint64_t t = bb->A;
  t = t >> y;
  t = t >> (8 * x);
  return t & 1;
}

void bit_set(Bitboard *bb, int x, int y, int value) {
  x = 7 - x;
  y = 7 - y;
  uint64_t t = 0x0000000000000001; //<< (8ULL * x);
  for (int i = 0; i < x; i++)
    t = t << 8;
  // t = t << (8*x);
  // TODO: It doesn't work for no reason ???.
  // have to use this stupid loop which is slowing everything down.
  // losing the point of using bitboard anyway.
  // but it's faster than the array approach by a small margin, miraclously.
  t = t << y;

  if (value)
    bb->A |= t;
  else
    bb->A &= (~t);
}

void bit_print_board(Bitboard bb) {
  int *t = new int[N * N];
  int t_i = 63;
  for (int i = 0; i < 64; i++) {
    t[i] = bb.A & 1;
    bb.A = bb.A >> 1;
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      printf("%d ", t[t_i--]);
    }
    printf("\n");
  }
  printf("\n");

  delete t;
}

bool bit_iden(Bitboard a, Bitboard b) { return a.A == b.A; }

bool iden(vector<vector<int>> &a, vector<vector<int>> &b) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (a[i][j] != b[i][j])
        return false;

  return true;
}

void print_board(vector<vector<int>> &board) {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%d ", board[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

inline int q_count(vector<vector<int>> &board) {
  int n = 0;
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      n += board[i][j];

  return n;
}

inline void BIT_MASK(Bitboard *mask, int x, int y) {
  for (int i = 0; i < N; i++)
    bit_set(mask, x, i, 1);
  for (int i = 0; i < N; i++)
    bit_set(mask, i, y, 1);
  for (int i = 0; x + i < N && y + i < N; i++)
    bit_set(mask, x + i, y + i, 1);
  for (int i = 0; x - i >= 0 && y - i >= 0; i++)
    bit_set(mask, x - i, y - i, 1);
  for (int i = 0; x - i >= 0 && y + i < N; i++)
    bit_set(mask, x - i, y + i, 1);
  for (int i = 0; x + i < N && y - i >= 0; i++)
    bit_set(mask, x + i, y - i, 1);
}

inline void MASK(vector<vector<int>> &mask, int x, int y) {
  for (int i = 0; i < N; i++)
    mask[x][i] = -1;
  for (int i = 0; i < N; i++)
    mask[i][y] = -1;
  for (int i = 0; x + i < N && y + i < N; i++)
    mask[x + i][y + i] = -1;
  for (int i = 0; x - i >= 0 && y - i >= 0; i++)
    mask[x - i][y - i] = -1;
  for (int i = 0; x - i >= 0 && y + i < N; i++)
    mask[x - i][y + i] = -1;
  for (int i = 0; x + i < N && y - i >= 0; i++)
    mask[x + i][y - i] = -1;
}

int bit_legit(Bitboard *bb, int x, int y) {
  Bitboard mask;
  mask.A = 0;

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (bit_locate(bb, i, j) == 1)
        BIT_MASK(&mask, i, j);

  return (bit_locate(&mask, x, y) == 1) ? 0 : 1;
}

int legit(vector<vector<int>> &board, int x, int y) {
  vector<vector<int>> mask;
  vector<int> row(N);
  for (int i = 0; i < N; i++)
    mask.push_back(row);
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      mask[i][j] = 0;

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (board[i][j] == 1)
        MASK(mask, i, j);

  return (mask[x][y] == -1) ? 0 : 1;
}

void bit_backtrack(Bitboard bb, int x, int y, int depth) {
  bit_set(&bb, x, y, 1);

  if (__builtin_popcountll(bb.A) == 8) {

    if (bit_solution.count(bb.A) == 0) {
      bit_print_board(bb);
      bit_solution.insert(bb.A);
      printf("solution_count = %lu\n", bit_solution.size());
    }

    return;
  }

  // if(depth == 8) return;

  Bitboard mask;
  mask.A = 0;

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      if (bit_locate(&bb, i, j) == 1)
        BIT_MASK(&mask, i, j);

  if (mask.A == full)
    return;

  for (int i = 0; i < N; i++)
    // for (int j = 0; j < N; j++)
    if (y + 1 < N && bit_locate(&mask, i, y + 1) != 1)
      bit_backtrack(bb, i, y + 1, depth + 1);
}

void backtrack(vector<vector<int>> board, int x, int y) {
  board[x][y] = 1;
  if (q_count(board) == N) {
    if (solution.count(board) == 0) {
      print_board(board);
      solution.insert(board);
      printf("solution_count = %lu\n", solution.size());
    }
    return;
  }

  for (int i = 0; i < N; i++)
    // for (int j = 0; j < N; j++)
    if (y + 1 < N && legit(board, i, y + 1))
      backtrack(board, i, y + 1);
}

void bit_bt(Bitboard bb) {
  assert(N == 8);
  for (int i = 0; i < N; i++)
    // for(int j = 0; j < N; j++)
    bit_backtrack(bb, i, 0, 0);
}

void bt(vector<vector<int>> board) {
  for (int i = 0; i < N; i++)
    // for(int j = 0; j < N; j++)
    backtrack(board, i, 0);
}

int main() {
  cin >> N;
  // assert(N > 3);
  vector<vector<int>> board;
  vector<int> row(N);
  for (int i = 0; i < N; i++)
    board.push_back(row);

  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      board[i][j] = 0;
  // initializing array-board.

  Bitboard bb;
  bb.A = 0x0000000000000000ULL;

  bfs();
  // bt(board);
  // bit_bt(bb);
  cout << "total " << solution.size() << " solutions." << endl;

  return 0;
}
