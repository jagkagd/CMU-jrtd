#include "utils.h"


int print_detail(char *buf, int size)
{
    int i;
    for (i = 0; i < size; i++)
    {
        switch (buf[i])
        {
        case '\0':
            fprintf(stdout, "\\0");
            break;
        case '\n':
            fprintf(stdout, "\\n");
            break;
        case '\r':
            fprintf(stdout, "\\r");
            break;

            /* Etc. */

        default:
            /* Now comes the "hard" part, because not all characters here
            * are actually printable
            */
            if (isprint(buf[i]))
                fprintf(stdout, "%c", buf[i]); /* Printable character, print it as usual */
            else
                fprintf(stdout, "\\x%02x", buf[i]); /* Non-printable character, print as hex value */

            break;
        }
    }
    fprintf(stdout, "\n");
}