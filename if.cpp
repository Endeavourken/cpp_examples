#include <net/if.h>
#include <stdio.h>

void network_if() {
    auto index = if_nametoindex("lo");
    printf("the index of lo is %d\n", index);
}
