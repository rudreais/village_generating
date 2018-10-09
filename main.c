#include <stdio.h>
#include "vill_gen.h"

int main(void)
{
	node main = {.parent = NULL, .coord = {.x = 4, .y = 4}};
	
	printf("%f\n%f\n", main.coord.x, main.coord.y);
	printf("dsl je me couche maintenant\n");
	return 0;
}
