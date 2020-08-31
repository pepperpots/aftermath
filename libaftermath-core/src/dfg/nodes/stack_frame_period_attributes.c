/**
 * Author: Richard Neill <richard.neill@manchester.ac.uk>
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

#include "stack_frame_period_attributes.h"
#include <aftermath/core/hierarchy.h>

int am_dfg_stack_frame_period_attributes_node_process(struct am_dfg_node* n)
{
	// If there was no valid input stack frame period, then we write an empty string to buffer

	struct am_dfg_port* pin = &n->ports[0];
	struct am_dfg_port* pnames = &n->ports[1];
	struct am_stack_frame_period** nodes;
	size_t old_num_samples = 0;
	char* info;
	size_t nin;

	char message[120] = "";
	info = strdup(message);

	if(!am_dfg_port_is_connected(pin) || !am_dfg_port_is_connected(pnames))
		return 0;

	if((nin = pin->buffer->num_samples) > 0){

		nodes = pin->buffer->data;

		if(am_dfg_port_activated(pnames)) {
			old_num_samples = pnames->buffer->num_samples;

			for(size_t i = 0; i < nin; i++) {

				if(nodes[i] != NULL){
					// replace our empty info message

					sprintf(message, "Period %lu/%lu for stack frame %p executing '%s' at depth %lu",
						nodes[i]->rank+1, // because zero indexed
						nodes[i]->stack_frame->num_periods,
						nodes[i]->stack_frame,
						nodes[i]->stack_frame->function_symbol->name,
						nodes[i]->stack_frame->depth);

					free(info);
					if(!(info = strdup(message)))
						goto out_err;
				}

			}
		}

	}


	if(am_dfg_buffer_write(pnames->buffer, 1, &info))
		goto out_err_free;

	return 0;

out_err_free:
	free(info);
out_err:
	am_dfg_buffer_resize(pnames->buffer, old_num_samples);
	return 1;
}
