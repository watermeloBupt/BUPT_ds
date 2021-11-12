#include "add.h"
#include <string.h>

struct MyStruct a[20];
void set_a(int k, char nam[])
{
    a[0].a = k;
    strcpy(a[0].name, nam);
}
