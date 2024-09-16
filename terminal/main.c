#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// ターミナルの高さを取得する関数
int get_terminal_height() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_row;
}

// ターミナルの幅を取得する関数
int get_terminal_width() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  return w.ws_col;
}

// ターミナルの中央に文字列を上下中央に表示する関数
void print_centered(const char *text) {
  int height = get_terminal_height();
  int width = get_terminal_width();
  int text_length = strlen(text);

  // 上部と下部の余白を計算
  int padding_top = (height - 1) / 2; // ターミナルの高さが奇数の場合も考慮
  int padding_bottom = height - 1 - padding_top;

  // 上部の余白を表示
  for (int i = 0; i < padding_top; i++) {
    putchar('\n');
  }

  // 横方向の中央揃えを計算
  int padding_left = (width - text_length) / 2;

  // 中央にテキストを表示
  for (int i = 0; i < padding_left; i++) {
    putchar(' ');
  }
  printf("%s\n", text);

  // 下部の余白を表示
  for (int i = 0; i < padding_bottom; i++) {
    putchar('\n');
  }
}

// ユーザからの入力を受け取る関数
char *get_user_input() {
  char *input = (char *)malloc(256 * sizeof(char)); // 最大255文字の入力を想定
  if (input == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  // カーソルの位置を記憶
  printf("\033[s"); // \033 はエスケープシーケンス、s は保存

  // 白い四角で入力欄を囲む
  printf("\033[47;30m"); // 背景色を白、文字色を黒に設定
  printf("\033[K");      // カーソル位置から行末までクリア

  printf(" Enter your input: ");
  fflush(stdout); // バッファをフラッシュしてプロンプトを表示

  int cursor_position = 0;
  char ch;
  while (1) {
    ch = getchar();
    if (ch == '\n' || ch == '\r') {
      input[cursor_position] = '\0'; // 改行文字を入力文字列から除去
      break;
    } else if (ch == 127 || ch == '\b') { // バックスペースまたはDELキー
      if (cursor_position > 0) {
        putchar('\b'); // カーソルを1文字戻す
        putchar(' ');  // 文字を消去
        putchar('\b'); // カーソルを1文字戻す
        cursor_position--;
      }
    } else if (cursor_position < 255) {
      putchar(ch);
      input[cursor_position] = ch;
      cursor_position++;
    }
    fflush(stdout);
  }
  printf("\033[u");
  printf("\033[0m");

  return input;
}

int main() {
  char *user_input;
  print_centered("Please enter your name:");
  user_input = get_user_input();
  print_centered(user_input);
  free(user_input);
  return 0;
}
