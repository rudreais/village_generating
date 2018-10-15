#include <stdio.h>
#include "vill_gen.h"

void init_generator_props(generator_props *props)
{
	*props = (generator_props) {
		.nb_slices = 5,
		.lens = {
			.min = 1,
			.max = 5
		},
		.childs = {
			.min = 4,
			.max = 6
		}
	};
}

void run_generator(generator_props *props)
{
	(void) props;
}

void render_houses(void)
{
}

int main(void)
{
	node main_node = {.parent = NULL, .coord = {.x = 4, .y = 4}};
	generator_props props;
	
	(void) main_node;
	init_generator_props(&props);
	printf("nb_slices=%d\n", props.nb_slices);
	return 0;
}
