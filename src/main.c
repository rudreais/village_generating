#include <stdio.h>
#include <stdlib.h>
#include "vill_gen.h"

void render_houses(render_props *rendering)
{
	sfEvent event;
	sfCircleShape *shape = sfCircleShape_create();
	float radius = 100;
	sfVector2f origin = {rendering->mode.width / 2 - radius, rendering->mode.height / 2 - radius};

	sfCircleShape_setFillColor(shape, sfRed);
	sfCircleShape_setPosition(shape, origin);
	sfCircleShape_setRadius(shape, radius);
	while (sfRenderWindow_isOpen(rendering->window)) {
		while (sfRenderWindow_pollEvent(rendering->window, &event))
			if (event.type == sfEvtClosed)
				sfRenderWindow_close(rendering->window);
		sfRenderWindow_clear(rendering->window, sfWhite);
		// printing circles
		// machin(rendering,
		sfRenderWindow_drawCircleShape(rendering->window, shape, NULL);
		sfRenderWindow_display(rendering->window);
	}
}

void init_generator_props(generator_props *props)
{
	*props = (generator_props) {
		.nb_slices = 5,
		.nb_houses = -1,
		.lens = {
			.min = 1.0f,
			.max = 5.0f
		},
		.bending = {
			.min = 1.0f,
			.max = 5.0f
		},
		.childs = {
			.min = 4,
			.max = 6
		}
	};
}

node **run_generator(generator_props *props)
{
	node main_node = {.parent = NULL, .coord = {.x = 0, .y = 0}};
	node **nodes = NULL;
	unsigned int parents_per_slices = 1;
	unsigned int childs_per_slices;
	int count = 0;

	// slices
	for (unsigned int i = 0; i < props->nb_slices; i++) {
		printf("slice=%d\n---------\n", i);
		childs_per_slices = 0;
		// parents
		for (unsigned int j = 0, tmp_childs = 0; j < parents_per_slices; j++) {
			tmp_childs = (rand() % (props->childs.max - props->childs.min)) + props->childs.min;
			childs_per_slices += tmp_childs;
			nodes = realloc(nodes, sizeof(node *) * (count + tmp_childs + 1));
			nodes[count + tmp_childs] = NULL;
			// childs
			for (unsigned int k = 0; k < tmp_childs; k++) {
				nodes[count] = malloc(sizeof(node));
				nodes[count++]->size = (float) count;
				printf("parents=%d\nchild=%d\n", j, k);
			}
		}
		parents_per_slices = childs_per_slices;
		if (props->childs.min > 1)
			props->childs.min--;
		if (props->childs.max > 1)
			props->childs.max--;
	}
	props->graph_size = count;
	return nodes;
}

int main(void)
{
	generator_props props;
	render_props rendering;
	sfVideoMode mode = {1024, 1024, 32};
	node **nodes;

	rendering.mode = mode;
	// rendering.window = sfRenderWindow_create(mode, "Village Generation", sfClose, NULL);
	init_generator_props(&props);
	nodes = run_generator(&props);
	printf("nb_slices=%d\n", props.nb_slices);
	for (int i = 0; nodes[i] != NULL; i++)
		printf("%f\n", nodes[i]->size);
	printf("%d\n", props.graph_size);
	return 0;
}
