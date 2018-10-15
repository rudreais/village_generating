#ifndef VILL_GEN_H_
#define VILL_GEN_H

#pragma once

#include <SFML/Graphics.h>

typedef struct {
	float min;
	float max;
} interval2f;

typedef struct {
	unsigned int nb_slices;
	interval2f lens;
	interval2f childs;
} generator_props;

typedef struct node_params node;

struct node_params {
	node *parent;
	sfVector2f coord;
};

#endif /* VILL_GEN_H_ */
