/**
 * Author: Andi Drebes <andi@drebesium.org>
 * Author: Igor Wodiany <igor.wodiany@manchester.ac.uk>
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

#include <aftermath/core/stack_frame_period_array.h>
#include <aftermath/core/dfg/types/pair_timestamp_hierarchy_node.h>
#include <aftermath/render/dfg/nodes/timeline/layers/openmp.h>
#include <aftermath/render/timeline/layer.h>
#include <aftermath/render/timeline/layers/interval.h>
#include <aftermath/render/timeline/renderer.h>

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(stack_frame_period,
	"core::stack_frame_period",
	struct am_timeline_stack_frame_period_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	stack_frame_period,
	"core::stack_frame_period")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	function_symbol,
	"core::function_symbol",
	struct am_timeline_function_symbol_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	function_symbol,
	"core::function_symbol")


int am_render_dfg_timeline_openmp_stack_frame_period_layer_dominant_period_at_pos_node_type_process(
	struct am_dfg_node* n)
{
	struct am_dfg_port* player = &n->ports[0];
	struct am_dfg_port* ppos = &n->ports[1];
	struct am_dfg_port* pdom_nodes = &n->ports[2];

	struct am_dfg_type_pair_timestamp_const_hierarchy_node* mouse_pos;
	struct am_timeline_render_layer** layers;
	struct am_timeline_render_layer* layer;
	struct am_timeline_renderer* renderer;
	struct am_stack_frame_period_array* sfp_array;
	struct am_interval px_interval;
	struct am_stack_frame_period* dom_sfp = NULL;
	uint64_t element_address = 0;
	int dom_index_valid;
	size_t num_layers;
	size_t num_pos;
	size_t old_num_out;
	double px;

	if(!am_dfg_port_activated(player) || player->buffer->num_samples == 0 ||
	   !am_dfg_port_activated(ppos) || ppos->buffer->num_samples == 0 ||
	   !am_dfg_port_activated(pdom_nodes))
	{
		return 0;
	}

	num_layers = player->buffer->num_samples;
	layers = player->buffer->data;

	num_pos = ppos->buffer->num_samples;
	mouse_pos = ppos->buffer->data;

	old_num_out = pdom_nodes->buffer->num_samples;

	for(size_t i = 0; i < num_layers; i++) {
		layer = layers[i];

		for(size_t j = 0; j < num_pos; j++) {
			if(!layer->renderer)
				goto out_err_resize;

			renderer = layer->renderer;

			/* If there is no trace, there cannot be any dominant period */
			if(!layer->renderer->trace)
				continue;

			/* Ignore if the stack frame periods are not being rendered */
			if(!layer->enabled)
				continue;

			/* Ignore invisible positions */
			if(!am_interval_contains_p(&renderer->visible_interval,
						   mouse_pos[j].timestamp))
				continue;

			/* Pixel at the input timestamp */
			px = am_timeline_renderer_timestamp_to_x(
				renderer, mouse_pos[j].timestamp);

			/* Calculate interval that corresponds to the pixel */
			if(am_timeline_renderer_x_to_timestamp(
				   renderer, px, &px_interval.start) ||
			   am_timeline_renderer_x_to_timestamp(
				   renderer, px+1, &px_interval.end))
			{
				goto out_err_resize;
			}

			/* px interval is to be interpreted half-open (end
			 * excluded); Calculate equivalent closed interval. */
			if(px_interval.start != px_interval.end)
				px_interval.end--;

			/* Determine dominant element */
			am_timeline_interval_layer_get_dominant_element(
				   (struct am_timeline_interval_layer*)layer,
				   mouse_pos[j].node,
				   &px_interval,
				   &element_address);

			/* May be NULL if not found, letting next DFG node handle */
			dom_sfp = (struct am_stack_frame_period*) element_address;

			if(am_dfg_buffer_write(
					 pdom_nodes->buffer, 1, &dom_sfp))
				goto out_err_resize;

		}
	}

	return 0;

out_err_resize:
	am_dfg_buffer_resize(pdom_nodes->buffer, old_num_out);
	return 1;
}


/********************/

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_for_loop_type,
	"openmp::for_loop_type",
	struct am_timeline_openmp_for_loop_type_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_for_loop_type,
	"openmp::for_loop_type")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_for_loop_instance,
	"openmp::for_loop_instance",
	struct am_timeline_openmp_for_loop_instance_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_for_loop_instance,
	"openmp::for_loop_instance")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_iteration_set,
	"openmp::iteration_set",
	struct am_timeline_openmp_iteration_set_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_iteration_set,
	"openmp::iteration_set")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_iteration_period,
	"openmp::iteration_period",
	struct am_timeline_openmp_iteration_period_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_iteration_period,
	"openmp::iteration_period")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_type,
	"openmp::task_type",
	struct am_timeline_openmp_task_type_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_type,
	"openmp::task_type")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_instance,
	"openmp::task_instance",
	struct am_timeline_openmp_task_instance_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_instance,
	"openmp::task_instance")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_period,
	"openmp::task_period",
	struct am_timeline_openmp_task_period_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_period,
	"openmp::task_period")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_thread,
	"openmp::thread",
	struct am_timeline_openmp_thread_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_thread,
	"openmp::thread")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_parallel,
	"openmp::parallel",
	struct am_timeline_openmp_parallel_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_parallel,
	"openmp::parallel")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_create,
	"openmp::task_create",
	struct am_timeline_openmp_task_create_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_create,
	"openmp::task_create")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_schedule,
	"openmp::task_schedule",
	struct am_timeline_openmp_task_schedule_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_schedule,
	"openmp::task_schedule")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_implicit_task,
	"openmp::implicit_task",
	struct am_timeline_openmp_implicit_task_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_implicit_task,
	"openmp::implicit_task")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_sync_region_wait,
	"openmp::sync_region_wait",
	struct am_timeline_openmp_sync_region_wait_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_sync_region_wait,
	"openmp::sync_region_wait")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_mutex_released,
	"openmp::mutex_released",
	struct am_timeline_openmp_mutex_released_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_mutex_released,
	"openmp::mutex_released")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_dependences,
	"openmp::dependences",
	struct am_timeline_openmp_dependences_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_dependences,
	"openmp::dependences")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_dependence,
	"openmp::task_dependence",
	struct am_timeline_openmp_task_dependence_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_dependence,
	"openmp::task_dependence")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_work,
	"openmp::work",
	struct am_timeline_openmp_work_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_work,
	"openmp::work")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_master,
	"openmp::master",
	struct am_timeline_openmp_master_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_master,
	"openmp::master")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_sync_region,
	"openmp::sync_region",
	struct am_timeline_openmp_sync_region_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_sync_region,
	"openmp::sync_region")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_lock_init,
	"openmp::lock_init",
	struct am_timeline_openmp_lock_init_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_lock_init,
	"openmp::lock_init")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_lock_destroy,
	"openmp::lock_destroy",
	struct am_timeline_openmp_lock_destroy_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_lock_destroy,
	"openmp::lock_destroy")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_mutex_acquire,
	"openmp::mutex_acquire",
	struct am_timeline_openmp_mutex_acquire_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_mutex_acquire,
	"openmp::mutex_acquire")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_mutex_acquired,
	"openmp::mutex_acquired",
	struct am_timeline_openmp_mutex_acquired_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_mutex_acquired,
	"openmp::mutex_acquired")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_nest_lock,
	"openmp::nest_lock",
	struct am_timeline_openmp_nest_lock_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_nest_lock,
	"openmp::nest_lock")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_flush,
	"openmp::flush",
	struct am_timeline_openmp_flush_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_flush,
	"openmp::flush")

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_cancel,
	"openmp::cancel",
	struct am_timeline_openmp_cancel_layer)

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_cancel,
	"openmp::cancel")

