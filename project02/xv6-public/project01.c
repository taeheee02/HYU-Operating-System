#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    printf(1, "My student id is 2022044566\n"); 
    printf(1, "My pid is %d\n", getpid());

    int gpid = getgpid(); 
    if(gpid < 0)
        printf(1, "Error: NO grandparent pid\n");
    else
        printf(1, "My gpid is %d\n", gpid);

    exit();
}
