#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hunspell.h>

int word_contains(char* word, char c) {
  for (int i = 0; i < 5; i++) {
    if (word[i] == c)
      return 1;
  }
  return 0;
}

void print_diff(char* word, char* guess) {
  printf("  ");
  for (int i = 0; i < 5; i++) {
    if (word[i] == guess[i]) {
      printf("^");
    } else if (word_contains(word, guess[i])) {
      printf("?");
    } else {
      printf("x");
    }
  }
  printf("\n");
}

#define NUM_WORDS 1894

char words[NUM_WORDS * 5];

void read_words() {
  FILE* f = fopen("words", "r");
  for (int i = 0; fgets(&words[i], 10, f) != NULL; i += 5) {}
  fclose(f);
}

char* random_word() {
  return &words[5*(arc4random() % NUM_WORDS)];
}

char* guess = NULL;
size_t cap = 0;
ssize_t len;

int keep_going() {
  printf("do you want play another game (y/n) ");
  fflush(stdin);
  len = getline(&guess, &cap, stdin);
  if (len-1 == 1 && guess[0] == 'y')
    return 1;
  return 0;
}

int main(int argc, char** argv) {
  Hunhandle* h = Hunspell_create("/Library/Spelling/en_US.aff", "/Library/Spelling/en_US.dic");
  read_words();
  char* word;
  do {
    word = random_word();
    while (1) {
      printf("> ");
      fflush(stdin);
      len = getline(&guess, &cap, stdin);
      guess[len-1] = '\0';
      if (len-1 == 1 && guess[0] == 'q') {
        printf("the word was %.5s\n", word);
        break;
      } else if (len-1 != 5) {
        printf("not a five-letter word\n");
      } else if (!Hunspell_spell(h, guess)) {
        printf("not in the dictionary\n");
      } else if (strncmp(word, guess, 5) == 0) {
        printf("you got it!\n");
        break;
      } else {
        print_diff(word, guess);
      }
    }
  } while (keep_going());
  return 0;
}
