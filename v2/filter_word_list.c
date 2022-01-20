#include <stdio.h>
#include <hunspell.h>

int main(int argc, char** argv) {
  Hunhandle* h = Hunspell_create("/Library/Spelling/en_US.aff", "/Library/Spelling/en_US.dic");
  char* word = NULL;
  size_t cap = 0;
  ssize_t len;
  while (1) {
    len = getline(&word, &cap, stdin);
    if (len < 0) break;
    word[len-1] = '\0';
    if (len-1 == 5 && 'a' <= word[0] && 'z' >= word[0] && Hunspell_spell(h, word)) {
      printf("%s\n", word);
    }
  }
  return 0;
}
