/*----------------------------------------------------------------------------
 Note: the code in this file is not to be shared with anyone or posted online.
 (c) Rida Bazzi and Adam Doupe, 2016
 ----------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define KEYWORDS  5
#define RESERVED  26

char current_token[MAX_TOKEN_LENGTH];
int token_length;
token_type t_type;
int line = 1;


static int active_token = 0;
/*static char *reserved[] =
    {   "",
        "IF",
        "WHILE",
        "DO",
        "THEN",
        "PRINT",
        "+",
        "-",
        "/",
        "*",
        "=",
        ":",
        ",",
        ";",
        "[",
        "]",
        "(",
        ")",
        "<>",
        ">",
        "<",
        "<=",
        ">=",
        ".",
        "NUM",
        "ID",
        "ERROR"
    };

*/
static void skip_space()
{
    char c;

    c = getchar();
    line += (c == '\n');
    while (!feof(stdin) && isspace(c))
    {
        c = getchar();
        line += (c == '\n');
    }
    // return character to input buffer if eof is not reached
    if (!feof(stdin))
    {
        ungetc(c,stdin);
    }
}

/*static int is_keyword(char *s)
{
    int i;

    for (i = 1; i <= KEYWORDS; i++)
    {
        if (strcmp(reserved[i],s) == 0)
        {
            return i;
        }
    }
    return 0;
}*/

/*static token_type scan_number()
{
    char c;

    c = getchar();
    if (isdigit(c))
    {
        if (c == '0')
        {
            current_token[token_length] = c;
            token_length++;
        }
        else
        {
            while (isdigit(c) && token_length < MAX_TOKEN_LENGTH)
            {
                current_token[token_length] = c;
                token_length++;
                c = getchar();
            }
            ungetc(c, stdin);
        }
        current_token[token_length] = '\0';
        return NUM;
    }
    else
    {
        return ERROR;
    }
}
*/
static token_type scan_id_keyword()
{
    char c;
    token_type the_type;

    c = getchar();
    if (isalpha(c))
    {
        while (isalnum(c) && token_length < MAX_TOKEN_LENGTH)
        {
            current_token[token_length] = c;
            token_length++;
            c = getchar();
        }
        current_token[token_length] = '\0';
        ungetc(c, stdin);
        //k = is_keyword(current_token);
            the_type = ID;
        
        //else
        //{
         ///   the_type = (token_type) k;
        //}
        return the_type;
    }
    else
    {
        return ERROR;
    }
}

token_type getToken()
{
    char c;

    if (active_token)
    {
        active_token = 0;
        return t_type;
    }
    skip_space();
    token_length = 0;
    current_token[0] = '\0';
    c = getchar();
    switch (c)
    {
        case '-':
            c = getchar();
            if (c == '>')
            {
                t_type = ARROW;
                strcpy(current_token,"->");
            }
            else
            {
                ungetc(c, stdin);
                t_type = ERROR;
            }
            return t_type;
       case '#':
            c = getchar();
            if (c == '#')
            {
                t_type = DOUBLE_HASH;                
                strcpy(current_token,"##");
            }
            else
            {
                ungetc(c, stdin);
                t_type = HASH;
                strcpy(current_token,"#");
            }            
            return t_type;
       default:
            if (isalpha(c)) // token is ID
            {
                ungetc(c, stdin);
                t_type = scan_id_keyword();
            }
            else if (c == EOF)
            {
                t_type = END_OF_FILE;
            }
            else if (isdigit(c))
            {   c=getchar();
                while(!isspace(c))
                {
                    c=getchar();
                }
                t_type = ERROR;
            }
            else
            {
               t_type = ERROR;
            }
            return t_type;
    }
}

void ungetToken()
{
    active_token = 1;
}

/*
 * Write your code in a SEPARATE FILE, do NOT edit this file.
 *
 *
 * If using C, include lexer.h in your code and compile and
 * link your code like this:
 *
 * $ gcc -Wall lexer.c your_file.c
 *
 *
 *
 * If using C++, include lexer.h like this in your code:
 *
 * extern "C" {
 *     #include "lexer.h"
 * }
 *
 * And compile and link your code like this:
 *
 * $ gcc -Wall -c lexer.c
 * $ g++ -Wall lexer.o your_file.cpp
 *
 * The first command just compiles the C code. The second
 * command compiles your C++ code and links it with the
 * compiled lexer.
 *
 */
