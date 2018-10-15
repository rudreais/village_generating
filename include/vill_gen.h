#ifndef VILL_GEN_H_
#define VILL_GEN_H

#pragma once

#include <SFML/Graphics.h>

typedef struct {
	float min;
	float max;
} interval2f;

typedef struct {
	int min;
	int max;
} interval2i;

typedef struct {
	unsigned int nb_slices;
	int nb_houses;
	interval2f lens;
	interval2f bending;
	interval2i childs;
	int graph_size;
} generator_props;

typedef struct {
	sfVideoMode mode;
	sfRenderWindow *window;
} render_props;

typedef struct node_params node;

struct node_params {
	node *parent;
	sfVector2f coord;
	float size;
};

#endif /* VILL_GEN_H_ */
