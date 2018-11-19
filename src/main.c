#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "vill_gen.h"

void render_town(render_props *rprops, const town *input_town)
{
	sfEvent event;
	sfRectangleShape *shape = sfRectangleShape_create();
	float radius = 100;
	const sfVector2f origin = {rprops->mode.width / 2 - radius, rprops->mode.height / 2 - radius};
	sfRectangleShape_setFillColor(shape, sfRed);

	//sfRenderWindow_clear(rprops->window, sfWhite);
	while (sfRenderWindow_isOpen(rprops->window))
	{
		while (sfRenderWindow_pollEvent(rprops->window, &event))
			if (event.type == sfEvtClosed)
				sfRenderWindow_close(rprops->window);
	
		for(size_t i=0; i < input_town->count_houses; i++) {
			sfVector2f globalPos = (sfVector2f) {
				.x = origin.x + input_town->houses[i].coords.x,
				.y = origin.y + input_town->houses[i].coords.y
			};
			sfRectangleShape_setPosition(shape, globalPos);
			sfRectangleShape_setSize(shape, input_town->houses[i].dims);
			sfRenderWindow_drawRectangleShape(rprops->window, shape, NULL);
			sfRenderWindow_display(rprops->window);
		}
	}
	// TODO: free(...)
}

void init_generator_props(generator_init_params *props)
{
	*props = (generator_init_params) {
		.distrib_lens = {
			.min = 1.0f,
			.max = 5.0f
		},
		.distrib_bending = {
			.min = 1.0f,
			.max = 5.0f
		},
		.distrib_childs = {
			.min = 4,
			.max = 6
		},
		.total_nb_slices = 3
	};
}

void run_generator(const generator_init_params *ip, town* output_town)
{
	// Init generator parameters (simple copy for now)
	generator_exec_params ep;
	ep.distrib_bending = ip->distrib_bending;
	ep.distrib_lens = ip->distrib_lens;
	ep.distrib_childs = ip->distrib_childs;

	output_town->count_houses = 1;
	output_town->houses = malloc(sizeof(town));
	output_town->houses[0] = (house_node) {
		.parent = NULL, 
		.coords = {.x = 0, .y = 0},
		.dims = {.x = 0.2f, .y = 0.2f}
	};
	ep.count_slices = 1;
	ep.nb_parents_per_slice = 1;

	size_t current_parent_idx = 0;
	for (size_t i = 0; i < ip->total_nb_slices; i++) // slices
	{ 
		printf("##### slice n°%zu #####\n", i);
		ep.nb_childs_per_parent = (rand() % (ep.distrib_childs.max - ep.distrib_childs.min)) + ep.distrib_childs.min;
		for (size_t j = 0; j < ep.nb_parents_per_slice; j++) // parents 
		{ 
			printf("current_parent_idx=%zu\n", current_parent_idx);
			printf("nb_childs_per_parent=%zu\n", ep.nb_childs_per_parent);
			size_t alloc_size = sizeof(house_node) * (output_town->count_houses + ep.nb_childs_per_parent * ep.nb_parents_per_slice);
			output_town->houses = realloc(output_town->houses, alloc_size);

			for (size_t k = 0; k < ep.nb_childs_per_parent; k++) // childs
			{
				output_town->houses[output_town->count_houses] = (house_node) {
					.parent = &output_town->houses[current_parent_idx],
					.coords = { .x=0.0f , .y=0.0f },
					.dims = { .x=0.2f , .y=0.2f }
				};
				output_town->count_houses++;
			}
			current_parent_idx++;
			printf("--------------\n");
		}
		ep.nb_parents_per_slice = ep.nb_parents_per_slice * ep.nb_childs_per_parent;
		ep.distrib_childs.max -= (ep.distrib_childs.max > 1) ? 1 : 0;
		ep.distrib_childs.min -= (ep.distrib_childs.min > 1) ? 1 : 0;
	}
}

int main(void)
{
	// initialize random number generator
	srand(time(NULL));

	render_props rprops;
	rprops.mode = (sfVideoMode) {512, 512, 32};
	rprops.window = sfRenderWindow_create(rprops.mode, "Village Generation", sfClose, NULL);
	
	generator_init_params gprops; town my_town;
	init_generator_props(&gprops);
	run_generator(&gprops, &my_town);
	render_town(&rprops, &my_town);

	// TODO: free(...)

	return 0;
}
