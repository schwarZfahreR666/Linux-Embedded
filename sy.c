#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void main ()
{
char my_string[] = "hello there";
my_print (my_string);
my_print2 (my_string);
}
void my_print (char *s)
{
printf ("The string is %s\n", s);
}
void my_print2 (char *s)
{
char *s2;
int size, i;
size = strlen (s);
s2 = (char*) malloc (size + 1);
for (i = 0; i < size; i++)
s2[size -1- i] = s[i];
s2[size] = '\0';
printf ("The string printed backward is %s\n", s2);
}
