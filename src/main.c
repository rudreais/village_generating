#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "vill_gen.h"

void render_town(render_props *rprops, const town *input_town)
{
	sfEvent event;
	sfRectangleShape *shape = sfRectangleShape_create();
	const sfVector2f origin = {rprops->mode.width / 2, rprops->mode.height / 2};
	sfRectangleShape_setFillColor(shape, sfRed);

	sfRenderWindow_clear(rprops->window, sfWhite);
	for(size_t i=0; i < input_town->count_houses; i++) {
		sfVector2f globalPos = (sfVector2f) {
			.x = origin.x + input_town->houses[i].coords.x - input_town->houses[i].dims.x,
			.y = origin.y + input_town->houses[i].coords.y - input_town->houses[i].dims.y
		};
		printf("=== House n°%zu\n", i);
		printf("pos %f %f\n", input_town->houses[i].coords.x, input_town->houses[i].coords.y);
		printf("dims %f %f\n", input_town->houses[i].dims.x, input_town->houses[i].dims.y);
		sfRectangleShape_setPosition(shape, globalPos);
		sfRectangleShape_setSize(shape, input_town->houses[i].dims);
		sfRenderWindow_drawRectangleShape(rprops->window, shape, NULL);
	}
	sfRenderWindow_display(rprops->window);

	while (sfRenderWindow_isOpen(rprops->window))
	{
		while (sfRenderWindow_pollEvent(rprops->window, &event))
			if (event.type == sfEvtClosed)
				sfRenderWindow_close(rprops->window);
	}
	// TODO: free(...)
}

void computeHouseCoords(generator_exec_params* ep, const house_node* parent, house_node* current) 
{
	float distance_scale = ( (float)rand() / (float)RAND_MAX * ( ep->distrib_distance_scale.max - ep->distrib_distance_scale.min) ) + ep->distrib_distance_scale.min;
	current->distance = parent->distance * distance_scale;

	float angle_deviation = ( (float)rand() / (float)RAND_MAX * ( ep->distrib_bending.max - ep->distrib_bending.min) ) + ep->distrib_bending.min;
	current->alpha = parent->alpha + angle_deviation;

	// convert polar coordinates to euclidean coordinates
	current->coords.x = parent->coords.x + (float)cos(current->alpha) * current->distance;
	current->coords.y = parent->coords.y + (float)sin(current->alpha) * current->distance;
}

void init_generator_props(generator_init_params *props)
{
	*props = (generator_init_params) {
		.distrib_bending = {
			.min = 0.0f,
			.max = M_PI
		},
		.distrib_distance_scale = {
			.min = 0.9,
			.max = 1.3
		},
		.distrib_childs = {
			.min = 4,
			.max = 6
		},
		.distrib_dims_scale = {
			.min = 0.7,
			.max = 1.1
		},
		.total_nb_slices = 3
	};
}

void run_generator(const generator_init_params *ip, town* output_town)
{
	// Init generator parameters (simple copy for now)
	generator_exec_params ep;
	ep.distrib_bending = ip->distrib_bending;
	ep.distrib_distance_scale = ip->distrib_distance_scale;
	ep.distrib_dims_scale = ip->distrib_dims_scale;
	ep.distrib_childs = ip->distrib_childs;

	output_town->count_houses = 1;
	output_town->houses = malloc(sizeof(house_node));
	output_town->houses[0] = (house_node) {
		.parent = NULL,
		.alpha = 0.0,
		.distance = 60.0,
		.coords = {.x = 0, .y = 0},
		.dims = {.x = 30.0f, .y = 30.0f}
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
				float dims_scale_x = ( (float)rand() / (float)RAND_MAX * ( ep.distrib_dims_scale.max - ep.distrib_dims_scale.min) ) + ep.distrib_dims_scale.min;
				float dims_scale_y = ( (float)rand() / (float)RAND_MAX * ( ep.distrib_dims_scale.max - ep.distrib_dims_scale.min) ) + ep.distrib_dims_scale.min;
				output_town->houses[output_town->count_houses] = (house_node) {
					.parent = &output_town->houses[current_parent_idx],
					.dims = { 
						.x = output_town->houses[current_parent_idx].dims.x * dims_scale_x,
					       	.y = output_town->houses[current_parent_idx].dims.y * dims_scale_y
					}
				};
				computeHouseCoords(&ep, &output_town->houses[current_parent_idx], &output_town->houses[output_town->count_houses]);
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
