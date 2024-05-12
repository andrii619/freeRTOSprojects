#include <utils.h>

char *trimnwhitespace(char *str, size_t n)
{
    // assert_param(str && n>0);
    char *end = NULL;
    int meow;

    // Trim leading space
    size_t current_char_index = 0;
    while (isspace((unsigned char)*str))
    {
        str++;
        current_char_index++;
        if (current_char_index > n)
        {
            // handle the case when the string is oferflown with all spaces and there
            // is no \0 to mark end of string
            // break;
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