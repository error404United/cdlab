#include <stdio.h>
#include <string.h>
#include <ctype.h>

char line[100];

int is_operator(char c) {
    switch (c) {
        case '+': case '-': case '*': case '/': case '=':
            printf("%c - Operator\n", c);
            return 1;
    }
    return 0;
}

int is_delimiter(char c) {
    switch (c) {
        case '{': case '}': case '(': case ')': case '[': case ']':
        case ',': case ';':
            printf("%c - Delimiter\n", c);
            return 1;
    }
    return 0;
}

int is_keyword(char buffer[]) {
    char keywords[32][10] = {
        "auto","break","case","char","const","continue","default","do",
        "double","else","enum","extern","float","for","goto","if","int",
        "long","register","return","short","signed","sizeof","static",
        "struct","switch","typedef","union","unsigned","void",
        "volatile","while"
    };
    for (int i = 0; i < 32; ++i) {
        if (strcmp(keywords[i], buffer) == 0)
            return 1;
    }
    return 0;
}

int is_integer(char buffer[]) {
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit(buffer[i]))
            return 0;
    }
    return 1;
}

int is_float(char buffer[]) {
    int dot_count = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i] == '.') {
            dot_count++;
            if (dot_count > 1) return 0;
        } else if (!isdigit(buffer[i]))
            return 0;
    }
    return dot_count == 1;
}

int main() {
    FILE *f = fopen("input.txt", "r");
    if (!f) {
        printf("Error: Cannot open file.\n");
        return 1;
    }

    int inside_comment = 0; // Track if we're inside /* ... */

    while (fgets(line, sizeof(line), f)) {
        int len = strlen(line);

        // Remove newline
        if (line[len - 1] == '\n') line[len - 1] = '\0';

        // Handle multi-line comment
        if (inside_comment) {
            for (int i = 0; i < len - 1; i++) {
                if (line[i] == '*' && line[i + 1] == '/') {
                    inside_comment = 0;
                    memmove(line, line + i + 2, len - i - 1); // remove comment part
                    break;
                }
            }
            if (inside_comment) continue; // still inside comment
        }

        // Remove single-line comment content
        for (int i = 0; i < len - 1; i++) {
            if (line[i] == '/' && line[i + 1] == '/') {
                line[i] = '\0'; // truncate before comment
                break;
            }
            if (line[i] == '/' && line[i + 1] == '*') {
                inside_comment = 1;
                line[i] = '\0'; // cut line before comment starts
                break;
            }
        }

        char token[100];
        int index = 0;
        token[0] = '\0';

        for (int i = 0; line[i] != '\0'; i++) {
            if (is_operator(line[i]) || is_delimiter(line[i]) ||
                line[i] == ' ' || line[i] == '\t') {

                if (token[0] != '\0') {
                    if (is_keyword(token))
                        printf("%s - Keyword\n", token);
                    else if (is_float(token))
                        printf("%s - Float\n", token);
                    else if (is_integer(token))
                        printf("%s - Integer\n", token);
                    else
                        printf("%s - Identifier\n", token);

                    token[0] = '\0';
                    index = 0;
                }
            } else {
                token[index++] = line[i];
                token[index] = '\0';
            }
        }

        // Final token at end of line
        if (token[0] != '\0') {
            if (is_keyword(token))
                printf("%s - Keyword\n", token);
            else if (is_float(token))
                printf("%s - Float\n", token);
            else if (is_integer(token))
                printf("%s - Integer\n", token);
            else
                printf("%s - Identifier\n", token);
        }
    }

    fclose(f);
    return 0;
}

