#include <utils.h>

char *trimnwhitespace(char *str, size_t n) {
  //assert_param(str && n>0);
  char *end;

  // Trim leading space
  size_t i = 0;
  while (isspace((unsigned char)*str)) {
    str++;
    i++;
    if (i > n) {
      // handle the case when the string is oferflown with all spaces and there
      // is no \0 to mark end of string
      //break;
      return NULL;
    }
  }

  if (*str == 0) // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char)*end))
    end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}