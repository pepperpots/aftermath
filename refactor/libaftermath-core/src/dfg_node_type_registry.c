/**
 * Author: Andi Drebes <andi@drebesium.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
 * USA.
 */

#include <aftermath/core/dfg_node_type_registry.h>
#include <string.h>

/*
 * Add a node type to the registry
 */
void am_dfg_node_type_registry_add(struct am_dfg_node_type_registry* reg,
				   struct am_dfg_node_type* t)
{
	list_add(&t->list, &reg->types);
}

/*
 * Find a node type by name. Returns NULL if the node type cannot be found.
 */
struct am_dfg_node_type*
am_dfg_node_type_registry_lookup(struct am_dfg_node_type_registry* reg,
				 const char* name)
{
	struct am_dfg_node_type* t;

	am_dfg_node_type_registry_for_each_type(reg, t)
		if(strcmp(t->name, name) == 0)
			return t;

	return NULL;
}

/* Initialize a registry */
void am_dfg_node_type_registry_init(struct am_dfg_node_type_registry* reg,
				    long flags)
{
	INIT_LIST_HEAD(&reg->types);
	reg->flags = flags;
}

/* Destroy a registry */
void am_dfg_node_type_registry_destroy(struct am_dfg_node_type_registry* reg)
{
	struct am_dfg_node_type* t;
	struct am_dfg_node_type* tmp;

	if(reg->flags & AM_DFG_NODE_TYPE_REGISTRY_DESTROY_TYPES) {
		am_dfg_node_type_registry_for_each_type_safe(reg, t, tmp) {
			am_dfg_node_type_destroy(t);
			free(t);
		}
	}
}