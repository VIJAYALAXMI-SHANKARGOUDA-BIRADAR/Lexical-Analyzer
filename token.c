#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "keywords.h"
#include "token.h"
#define RESET_COLOR "\033[0m"
#define GREEN_COLOR "\033[32m"  // Green text color
#define RED_COLOR "\033[31m"

int paren_count = 0;  // for ()
int curly_count = 0;  // for {}
int inside_string = 0;
int inside_char = 0;

int inside_for_paren = 0;

void printHeader()
{
    printf("\n");
    printf("................................................................\n");
    printf(GREEN_COLOR "LEXICAL ANALYZER => DONE BY VIJAYALAXMI SHANKARGOUDA BIRADAR" RESET_COLOR "\n");
    printf("................................................................\n");
}

FILE *fp;

int check_invalid_inc_dec(const char* num) {
    int next_char1 = fgetc(fp);
    int next_char2 = fgetc(fp);
    if ((next_char1 == '+' && next_char2 == '+') || (next_char1 == '-' && next_char2 == '-')) {
        printf(RED_COLOR "%-18s : %s%c%c (Invalid increment/decrement on literal)" RESET_COLOR "\n", "Error", num, next_char1, next_char2);
        return 1;
    } else {
        if (next_char2 != EOF) fseek(fp, -1, SEEK_CUR);
        if (next_char1 != EOF) fseek(fp, -1, SEEK_CUR);
        return 0;
    }
}

int openFile(const char *filename)
{


    // validation for .c file
    int len = strlen(filename);
    // check last 2 characters
    if (len < 2 || filename[len - 2] != '.' || filename[len - 1] != 'c')
    {
        printf("It should be .c extension file\n");
        return 1;
    }

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("cannot open file!\n");
        return 1;
    }
    return 0;
}

int nexttoken()
{
    if(fp == NULL) return 0;
    char num[100];
    int c, i;

    while ((c = fgetc(fp)) != EOF)
    {
        // skip whitespace
        if (isspace(c))
            continue;

        // skip preprocessor.
        /*if (c == '#')
        {
            char line[1000] = {0};
            int j = 0;
            line[j++] = '#';
            while ((c = fgetc(fp)) != EOF && c != '\n' && j < sizeof(line) - 1)
            {
                line[j++] = c;
            }
            line[j] = '\0';
            printf("%-18s : %s\n", "Preprocessor", line);
            continue;
        }*/


        // skip preprocessor.
        if (c == '#')
        {
            while ((c = fgetc(fp)) != EOF && c != '\n')
                ;

            continue;
        }

        // character constant
        if (c == '\'')
        {
            inside_char = 1;
            i = 0;
            c = fgetc(fp);

            if (c == EOF)
            {
                printf(RED_COLOR "%-18s : '(Unterminated Character constant)" RESET_COLOR "\n", "Error");
                inside_char = 0;
                return 1;
            }

            if (c == '\'')
            {
                printf(RED_COLOR "%-18s : ''(Empty character constant)" RESET_COLOR "\n", "Error");
                inside_char = 0;
                return 1;
            }

            num[i++] = c;

            c = fgetc(fp);

            // Handle escape sequences inside character constant
            if (num[0] == '\\')
            {
                // Escape sequence, expect one more char
                if (c == EOF)
                {
                    printf(RED_COLOR "%-18s : '\\ (Unterminated character constant)" RESET_COLOR "\n", "Error");
                    inside_char = 0;
                    return 1;
                }
                num[i++] = c;
                c = fgetc(fp);
            }

            if (c == '\'')
            {
                num[i] = '\0';
                printf("%-18s : '%s'\n", "Character constant", num);
                inside_char = 0; // reset
                return 1;
            }
            else
            {
                // Skip until closing quote or EOF
                while (c != '\'' && c != EOF)
                {
                    if (i < sizeof(num) - 1)
                        num[i++] = c;
                    c = fgetc(fp);
                }
                num[i] = '\0';

                if (c == '\'')
                {
                    // multi char error
                    printf(RED_COLOR "%-18s : '%s' (multi character constant)" RESET_COLOR "\n", "Error", num);
                    inside_char = 0;
                    return 0;
                }
                else
                {
                    // unterminated char error
                    printf(RED_COLOR "%-18s : '%s (Unterminated character constant)" RESET_COLOR "\n", "Error", num);
                    inside_char = 0;
                    return 0;
                }
            }
        }

        // identifier or keyword
        if (isalpha(c) || c == '_')
        {
            i = 0;
            num[i++] = c;
            while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_'))
                num[i++] = c;
            num[i] = '\0';

            if (c != EOF)
                fseek(fp, -1, SEEK_CUR);

            if (iskeyword(num))
            {
                printf("%-18s : %s\n", "Keyword", num);

                if (strcmp(num, "for") == 0)
                {
                    inside_for_paren = 1;
                }
            }
            else
                printf("%-18s : %s\n", "Identifier", num);

            return 1;
        }

        // number(integer constant,float constant)
        if (isdigit(c))
        {
            i = 0;
            num[i++] = c;

            if (c == '0')
            {
                int next = fgetc(fp);
                 // hexadecimal(0x or 0X)
                if (next == 'x' || next == 'X' )
                {
                    num[i++] = next;
                    int valid = 1;
                    while ((c = fgetc(fp)) != EOF && !isspace(c) && !strchr("{};,)", c))
                    {
                        if (!isxdigit(c))
                        {
                            valid = 0;
                        }
                        num[i++] = c;
                    }
                    num[i] = '\0';
                    if (valid && i > 2) // at least one hex digit after 0x
                    {
                        printf("%-18s : %s\n", "Integer constant", num);
                    }
                    else
                    {
                        printf(RED_COLOR "%-18s : %s(invalid hexadecimal)" RESET_COLOR "\n", "Error", num);
                        return 0;
                    }
                    if (c != EOF)
                    {
                        fseek(fp, -1, SEEK_CUR);
                    }
                    return 1;
                }


                // binary(0b or 0B)
                else if (next == 'b' || next == 'B')
                {
                    num[i++] = next;
                    int valid = 1;
                    while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_'))
                    {
                        if (c != '0' && c != '1')
                        {
                            valid = 0;
                        }
                        num[i++] = c;
                    }
                    num[i] = '\0';
                    if (valid && i > 1)
                    {
                        printf("%-18s : %s\n", "Integer constant", num);
                    }
                    else
                    {
                        printf(RED_COLOR "%-18s : %s (Invalid binary literal)" RESET_COLOR "\n", "Error", num);
                        return 0;
                    }
                    if (c != EOF)
                    {
                        fseek(fp, -1, SEEK_CUR);
                        return 1;
                    }
                }

                // octal
                else if (isdigit(next))
                {
                    num[i++] = next;
                    int valid = 1;
                    while ((c = fgetc(fp)) != EOF && isdigit(c))
                    {
                        if (c >= '8')
                            valid = 0;
                        num[i++] = c;
                    }
                    num[i] = '\0';
                    if (valid)
                    {
                        if (check_invalid_inc_dec(num)) return 0;
                        printf("%-18s : %s\n", "Integer constant", num);
                    }
                    else
                    {
                        printf(RED_COLOR "%-18s : %s (Invalid octal digit)" RESET_COLOR "\n", "Error", num);
                        return 0;
                    }
                    if (c != EOF)
                    {
                        fseek(fp, -1, SEEK_CUR);
                    }
                    return 1;
                }
                else // single "0"
                {
                    num[i] = '\0';
                    printf("%-18s : %s\n", "Integer constant", num);
                    if (next != EOF)
                        fseek(fp, -1, SEEK_CUR);
                    return 1;
                }
            }

            // decimal or float
            int isfloat = 0;   // flag to check if it is float
            int dot_count = 0; // count decimal points

            while ((c = fgetc(fp)) != EOF && (isdigit(c) || c == '.'))
            {
                if (c == '.')
                {
                    dot_count++;
                    if (dot_count > 1)
                    { // more than one dot --invalid float
                        num[i++] = c;
                        while ((c = fgetc(fp)) != EOF && (isdigit(c) || c == '.'))
                        {
                            num[i++] = c;
                        }
                        num[i] = '\0';
                        printf(RED_COLOR "%-18s : %s(Invalid float constant)" RESET_COLOR "\n", "Error", num);
                        return 0;
                    }
                    isfloat = 1; // set flag if decimal point is found
                }
                num[i++] = c;
            }

            if (c == 'f' || c == 'F')
            {
                num[i++] = c;
                num[i] = '\0';
                printf("%-18s : %s\n", "Float Constant", num);
                return 1;
            }

            // check if invalid identifier like 1num
            if (isalpha(c) || c == '_')
            {
                num[i++] = c;
                while ((c = fgetc(fp)) != EOF && (isalnum(c) || c == '_'))
                    num[i++] = c;

                num[i] = '\0';
                printf(RED_COLOR "%-18s : %s (Invalid Identifier)" RESET_COLOR "\n", "Error", num);

                return 0;
            }

            num[i] = '\0';

            if (c != EOF)
                fseek(fp, -1, SEEK_CUR);
            if (isfloat)
            {
                printf("%-18s : %s\n", "Float Constant", num);
            }
            else
            {
                printf("%-18s : %s\n", "Integer constant", num);
                return 1;
            }
        } /////////end of digit related validation////////////

        // string literal
        else if (c == '"')
        {
            inside_string = 1; // set flag
            i = 0;
            int un_close = 1; // unterminated until closed
            int escaped = 0; // flag for escape sequence

            while ((c = fgetc(fp)) != EOF)
            {
                if (escaped)
                {
                    // previous was \, so this c is escaped, add it
                    if (i < sizeof(num) - 1)
                        num[i++] = c;
                    escaped = 0;
                    continue;
                }
                if (c == '\\')
                {
                    escaped = 1; // next char is escaped
                    if (i < sizeof(num) - 1)
                        num[i++] = c;
                    continue;
                }
                if (c == '"')
                {
                    un_close = 0; // closed
                    break;
                }
                if (c == '\n' || c == '\r')
                {
                    break;
                }
                if (i < sizeof(num) - 1)
                    num[i++] = c;
                else
                    break;
            }
            num[i] = '\0';

            if (un_close)
            {
                printf(RED_COLOR "%-18s : \"%s\" (Unterminated string literal)" RESET_COLOR "\n", "Error", num);
                return 0;
            }
            else
            {
                printf("%-18s : \"%s\"\n", "String Literal", num);
            }
            inside_string = 0; // reset flag to 0

            return 1; // stop after handling string
        }

        // comments are skipped, else operators

        else if (c == '/')
        {
            int next = fgetc(fp);
            // single line comment//
            if (next == '/')
            {
                while ((c = fgetc(fp)) != EOF && c != '\n')
                    ;
                continue; // skip comment
            }

            // multi line comments/* ...*/

            else if (next == '*')
            {
                int prev = 0;
                while ((c = fgetc(fp)) != EOF)
                {
                    if (prev == '*' && c == '/')
                    {
                        break;
                    }
                    prev = c;
                }

                continue;
            } // skip comment
            else
            {
                if (next != EOF)
                    fseek(fp, -1, SEEK_CUR);
                printf("%-18s : %c\n", "Operator", c);
            }
        }

        // operators

        

        else if (strchr("+-*/=<>!%,", c))
        {
            int next = fgetc(fp);
            if ((c == '+' && next == '+') || (c == '-' && next == '-') ||
                (c == '=' && next == '=') || (c == '<' && next == '=') ||
                (c == '>' && next == '=') || (c == '!' && next == '=') ||
                (c == '*' && next == '=') || (c == '/' && next == '=') ||
                (c == '%' && next == '='))
            {
                printf("%-18s : %c%c\n", "Operator", c, next);
            }
            else
            {
                if (next != EOF)
                {
                    fseek(fp, -1, SEEK_CUR);
                }
                printf("%-18s : %c\n", "Operator", c);
            }
            return 1;
        }

        // symbols

        else if (c == '(')
        {
            paren_count++;
            if (inside_for_paren)
            {
                // keep inside for paren=1
            }
            printf("%-18s : %c\n", "Symbol", c);
            return 1;
        }
        else if (c == ')')
        {
            if (paren_count > 0)
            {
                paren_count--;
                printf("%-18s : %c\n", "Symbol", c);

                if (inside_for_paren && paren_count == 0)
                {
                    inside_for_paren = 0; // set the flag after closing
                }
            }
            else
            {
                printf(RED_COLOR "%-18s : %c (Unmatched closing parenthesis)" RESET_COLOR "\n", "Error", c);
                return 0;
            }
            return 1;
        }

        else if (c == '{')
        {
            if (!inside_string && !inside_char)
            {
                curly_count++;
            }
            printf("%-18s : %c\n", "Symbol", c);
            return 1;
        }
        else if (c == '}')
        {
            if (!inside_string && !inside_char)
            {
                if (curly_count > 0)
                {
                    curly_count--;
                    printf("%-18s : %c\n", "Symbol", c);
                }
                else
                {
                    printf(RED_COLOR "%-18s : %c (Unmatched closing brace)" RESET_COLOR "\n", "Error", c);
                    return 0;
                }
            }
            else
            {
                printf("%-18s : %c\n", "Symbol", c);
            }
            return 1;
        }

        else if (c == ';')
        {
            // check for unclosed parenthesis BEFORE semicolon
            if (!inside_for_paren && paren_count > 0)
            {
                printf(RED_COLOR "%-18s : ) (Unclosing parenthesis)" RESET_COLOR "\n", "Error");
                paren_count = 0; // reset so error won't repeat
                return 0;
            }
            printf("%-18s : %c\n", "Symbol", c);
            return 1;
        }
       /* else if (c == ',')
        {
            printf("%-18s : %c\n", "Symbol", c);
            return 1;
        }*/

        else if (c == '[' || c == ']')
        {
            printf("%-18s : %c\n", "Symbol", c);
            return 1;
        }
    }

    if (curly_count != 0)
    {
        printf(RED_COLOR "%-18s : } (Unmatched closing brace)" RESET_COLOR "\n", "Error");
        return 0;
    }

    return 0;
}