#ifndef VILL_GEN_H_
#define VILL_GEN_H

#pragma once

#include <SFML/Graphics.h>
#include <stdint.h>

typedef struct {
	float min;
	float max;
} interval2f;

typedef struct {
	int min;
	int max;
} interval2i;

typedef struct {
	uint32_t min;
	uint32_t max;
} interval2ui;

typedef struct {
	interval2f distrib_bending;
	interval2f distrib_distance_scale;
	interval2ui distrib_childs;
	size_t total_nb_slices;
} generator_init_params;

typedef struct {
	interval2f distrib_bending;
	interval2f distrib_distance_scale;
	interval2ui distrib_childs;
	size_t nb_parents_per_slice;
	size_t nb_childs_per_parent;
	size_t count_slices;
} generator_exec_params;

typedef struct {
	sfVideoMode mode;
	sfRenderWindow *window;
} render_props;

typedef struct house_node house_node;
struct house_node {
	house_node *parent;
	float alpha;
	float distance;
	sfVector2f coords;
	sfVector2f dims;
};

typedef struct town {
	size_t count_houses;
	house_node* houses;
} town;

#endif /* VILL_GEN_H_ */
