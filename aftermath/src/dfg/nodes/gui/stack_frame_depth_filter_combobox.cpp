/**
 * Author: Richard Neill <richard.neill@manchester.ac.uk>
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License version 2 as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include <iostream>
#include "stack_frame_depth_filter_combobox.h"
#include "../../../gui/widgets/StackFrameDepthFilterComboBox.h"

extern "C" {
	#include <aftermath/core/trace.h>
	#include <aftermath/core/stack_frame_array.h>
}

int am_dfg_amgui_stack_frame_depth_filter_combobox_init(struct am_dfg_node* n)
{
	struct am_dfg_amgui_stack_frame_depth_filter_combobox_node* cb = (typeof(cb))n;

	cb->widget = NULL;
	cb->widget_id = NULL;

	return 0;
}

void am_dfg_amgui_stack_frame_depth_filter_combobox_destroy(struct am_dfg_node* n)
{
	struct am_dfg_amgui_stack_frame_depth_filter_combobox_node* cb = (typeof(cb))n;

	free(cb->widget_id);
}

void am_dfg_amgui_stack_frame_depth_filter_combobox_disconnect(struct am_dfg_node* n,
						struct am_dfg_port* pi)
{
	struct am_dfg_amgui_stack_frame_depth_filter_combobox_node* cb = (typeof(cb))n;

	if(pi == &n->ports[0])
		if(cb->widget){
			cb->widget->clearDepthOptions();
		}
}

int am_dfg_amgui_stack_frame_depth_filter_combobox_process(struct am_dfg_node* n)
{

	struct am_dfg_amgui_stack_frame_depth_filter_combobox_node* cb = (typeof(cb))n;
	struct am_dfg_port* ptrace = &n->ports[AM_DFG_AMGUI_STACK_FRAME_DEPTH_FILTER_COMBOBOX_NODE_TRACE];
	struct am_dfg_port* pfiltered_trace =
		&n->ports[AM_DFG_AMGUI_STACK_FRAME_DEPTH_FILTER_COMBOBOX_NODE_FILTERED_TRACE];

	size_t selected_depth = 0;
	struct am_trace* trace = NULL;

	if(am_dfg_buffer_read_last(ptrace->buffer, (void*)&trace))
		return 1;

	if(cb->widget && cb->widget->isInit() == false) {
		if(trace){
			std::cout << "Updating combobox depth options" << std::endl;
			
			// TODO use one of the macros for this traversal
			struct am_event_collection_array* ecs = &trace->event_collections;
			for(unsigned i = 0; i < ecs->num_elements; i++)
			{
				struct am_array_collection* ac = &ecs->elements[i].event_arrays;
			
				for(unsigned j = 0; j < ac->num_elements; j++)
				{
					struct am_array_collection_entry* ace = &ac->elements[j];

					if(strcmp("am::core::stack_frame", ace->type) == 0)
					{
						struct am_typed_array_generic* array = ace->array;
			
						struct am_stack_frame* frames =
							(struct am_stack_frame*) array->elements;
						
						size_t max_depth = 0;
						for(unsigned k = 0; k < array->num_elements; k++)
						{
							struct am_stack_frame* frame = &frames[k];
							if(frame->depth > max_depth)
								max_depth = frame->depth;
						}
						cb->widget->modifyDepthOptions(max_depth);

					}
				}
			}

		} else {
			cb->widget->modifyDepthOptions(0);
		}

		cb->widget->setInit(true);
	}
	
	std::cout << "Processing the combobox" << std::endl;

	if(am_dfg_port_activated(pfiltered_trace) && cb->widget) {
		std::cout << "Output port is activated." << std::endl;

		selected_depth = cb->widget->getSelectedStackFrameDepth();

		// apply the correct filter to each frame
		std::cout << "Applying filters for depth " << selected_depth << std::endl;

		// TODO use one of the macros for this traversal
		struct am_event_collection_array* ecs = &trace->event_collections;
		for(unsigned i = 0; i < ecs->num_elements; i++)
		{
			struct am_array_collection* ac = &ecs->elements[i].event_arrays;
		
			for(unsigned j = 0; j < ac->num_elements; j++)
			{
				struct am_array_collection_entry* ace = &ac->elements[j];

				if(strcmp("am::core::stack_frame", ace->type) == 0)
				{
					//struct am_stack_frame_array* array =
					//	(struct am_stack_frame_array*) ace->array;
					struct am_typed_array_generic* array = ace->array;
		
					struct am_stack_frame* frames =
						(struct am_stack_frame*) array->elements;
					
					for(unsigned k = 0; k < array->num_elements; k++)
					{
						struct am_stack_frame* frame = &frames[k];
						if(frame->depth != selected_depth && selected_depth != 0){
							frame->post_filter_include = 0;
						} else {
							frame->post_filter_include = 1;
						}
					}
				}
			}
		}

	} else {
		std::cout << "Output port is not active." << std::endl;
	}

	if(am_dfg_buffer_write(pfiltered_trace->buffer, 1, &trace))
		return 1;

	return 0;

}

int am_dfg_amgui_stack_frame_depth_filter_combobox_from_object_notation(
	struct am_dfg_node* n,
	struct am_object_notation_node_group* g)
{
	struct am_dfg_amgui_stack_frame_depth_filter_combobox_node* cb = (typeof(cb))n;
	const char* widget_id;

	if(am_object_notation_eval_retrieve_string(&g->node, "widget_id",
						   &widget_id))
	{
		return 1;
	}

	if(!(cb->widget_id = strdup(widget_id)))
		return 1;

	return 0;
}

int am_dfg_amgui_stack_frame_depth_filter_combobox_to_object_notation(
	struct am_dfg_node* n,
	struct am_object_notation_node_group* g)
{
	struct am_dfg_amgui_stack_frame_depth_filter_combobox_node* cb = (typeof(cb))n;

	return am_object_notation_node_group_build_add_members(
		g,
		AM_OBJECT_NOTATION_BUILD_MEMBER, "widget_id",
		  AM_OBJECT_NOTATION_BUILD_STRING, cb->widget_id);
}
