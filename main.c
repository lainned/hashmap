#include "hashmap.h"
#include <stdio.h>

int main(){
    int a, b, c;
    char akey[100], bkey[100], ckey[100];
    scanf("%s", akey);
    scanf("%d", &a);
    scanf("%s", bkey);
    scanf("%d", &b);
    scanf("%s", ckey);
    scanf("%d", &c);

    hashmap* hm = hashmap_new();
    hashmap_set(hm, akey, &a);
    hashmap_set(hm, bkey, &b);
    hashmap_set(hm, ckey, &c);
    int *aval, *bval, *cval;
    aval = hashmap_get(hm, akey);
    bval = hashmap_get(hm, bkey);
    cval = hashmap_get(hm, ckey);
    printf("%d", *aval);
    printf("%d", *bval);
    printf("%d", *cval);
}