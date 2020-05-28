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

#ifndef AM_RENDER_DFG_NODE_TIMELINE_LAYERS_OPENMP_H
#define AM_RENDER_DFG_NODE_TIMELINE_LAYERS_OPENMP_H

#include <aftermath/core/dfg_node.h>
#include <aftermath/render/dfg/timeline_layer_common.h>

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	stack_frame_period,
	"core::stack_frame_period",
	"Timeline OpenMP Stack Frame Period Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	stack_frame_period,
	"core::stack_frame_period",
	"Timeline OpenMP Stack Frame Layer Configuration")

int am_render_dfg_timeline_stack_frame_period_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	function_symbol,
	"core::function_symbol",
	"Timeline OpenMP Function Symbol Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	function_symbol,
	"core::function_symbol",
	"Timeline OpenMP Function Symbol Layer Configuration")

int am_render_dfg_timeline_function_symbol_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_for_loop_type,
	"openmp::for_loop_type",
	"Timeline OpenMP Loop Type Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_for_loop_type,
	"openmp::for_loop_type",
	"Timeline OpenMP Loop Type Layer Configuration")

int am_render_dfg_timeline_openmp_for_loop_type_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_for_loop_instance,
	"openmp::for_loop_instance",
	"Timeline OpenMP Loop Instance Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_for_loop_instance,
	"openmp::for_loop_instance",
	"Timeline OpenMP Loop Instance Layer Configuration")

int am_render_dfg_timeline_openmp_for_loop_instance_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_iteration_set,
	"openmp::iteration_set",
	"Timeline OpenMP Iteration Set Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_iteration_set,
	"openmp::iteration_set",
	"Timeline OpenMP Iteration Set Layer Configuration")

int am_render_dfg_timeline_openmp_iteration_set_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_iteration_period,
	"openmp::iteration_period",
	"Timeline OpenMP Iteration Period Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_iteration_period,
	"openmp::iteration_period",
	"Timeline OpenMP Iteration Period Layer Configuration")

int am_render_dfg_timeline_openmp_iteration_period_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_type,
	"openmp::task_type",
	"Timeline OpenMP Task Type Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_type,
	"openmp::task_type",
	"Timeline OpenMP Task Type Layer Configuration")

int am_render_dfg_timeline_openmp_task_type_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_instance,
	"openmp::task_instance",
	"Timeline OpenMP Task Instance Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_instance,
	"openmp::task_instance",
	"Timeline OpenMP Task Instance Layer Configuration")

int am_render_dfg_timeline_openmp_task_instance_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_period,
	"openmp::task_period",
	"Timeline OpenMP Task Period Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_period,
	"openmp::task_period",
	"Timeline OpenMP Task Period Layer Configuration")

int am_render_dfg_timeline_openmp_task_period_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_thread,
	"openmp::thread",
	"Timeline OpenMP Thread Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_thread,
	"openmp::thread",
	"Timeline OpenMP Thread Layer Configuration")

int am_render_dfg_timeline_openmp_thread_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_parallel,
	"openmp::parallel",
	"Timeline OpenMP Parallel Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_parallel,
	"openmp::parallel",
	"Timeline OpenMP Parallel Layer Configuration")

int am_render_dfg_timeline_openmp_parallel_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_create,
	"openmp::task_create",
	"Timeline OpenMP Task Create Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_create,
	"openmp::task_create",
	"Timeline OpenMP Task Create Layer Configuration")

int am_render_dfg_timeline_openmp_task_create_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_schedule,
	"openmp::task_schedule",
	"Timeline OpenMP Task Schedule Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_schedule,
	"openmp::task_schedule",
	"Timeline OpenMP Task Schedule Layer Configuration")

int am_render_dfg_timeline_openmp_task_schedule_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_implicit_task,
	"openmp::implicit_task",
	"Timeline OpenMP Implicit Task Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_implicit_task,
	"openmp::implicit_task",
	"Timeline OpenMP Implicit Task Layer Configuration")

int am_render_dfg_timeline_openmp_implicit_task_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_sync_region_wait,
	"openmp::sync_region_wait",
	"Timeline OpenMP Sync Region Wait Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_sync_region_wait,
	"openmp::sync_region_wait",
	"Timeline OpenMP Sync Region Wait Layer Configuration")

int am_render_dfg_timeline_openmp_sync_region_wait_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_mutex_released,
	"openmp::mutex_released",
	"Timeline OpenMP Mutex Released Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_mutex_released,
	"openmp::mutex_released",
	"Timeline OpenMP Mutex Released Layer Configuration")

int am_render_dfg_timeline_openmp_mutex_released_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_dependences,
	"openmp::dependences",
	"Timeline OpenMP Dependences Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_dependences,
	"openmp::dependences",
	"Timeline OpenMP Dependences Layer Configuration")

int am_render_dfg_timeline_openmp_dependences_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_task_dependence,
	"openmp::task_dependence",
	"Timeline OpenMP Task Dependence Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_task_dependence,
	"openmp::task_dependence",
	"Timeline OpenMP Task Dependence Layer Configuration")

int am_render_dfg_timeline_openmp_task_dependence_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_work,
	"openmp::work",
	"Timeline OpenMP Worksharing Region Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_work,
	"openmp::work",
	"Timeline OpenMP Worksharing Region Layer Configuration")

int am_render_dfg_timeline_openmp_work_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_master,
	"openmp::master",
	"Timeline OpenMP Master Region Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_master,
	"openmp::master",
	"Timeline OpenMP Master Region Layer Configuration")

int am_render_dfg_timeline_openmp_master_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_sync_region,
	"openmp::sync_region",
	"Timeline OpenMP Sync Region Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_sync_region,
	"openmp::sync_region",
	"Timeline OpenMP Sync Region Layer Configuration")

int am_render_dfg_timeline_openmp_sync_region_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_lock_init,
	"openmp::lock_init",
	"Timeline OpenMP Mutex Lock Init Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_lock_init,
	"openmp::lock_init",
	"Timeline OpenMP Mutex Lock Init Configuration")

int am_render_dfg_timeline_openmp_lock_init_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_lock_destroy,
	"openmp::lock_destroy",
	"Timeline OpenMP Mutex Lock Destroy Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_lock_destroy,
	"openmp::lock_destroy",
	"Timeline OpenMP Mutex Lock Destroy Configuration")

int am_render_dfg_timeline_openmp_lock_destroy_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_mutex_acquire,
	"openmp::mutex_acquire",
	"Timeline OpenMP Mutex Acquire Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_mutex_acquire,
	"openmp::mutex_acquire",
	"Timeline OpenMP Mutex Acquire Layer Configuration")

int am_render_dfg_timeline_openmp_mutex_acquire_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_mutex_acquired,
	"openmp::mutex_acquired",
	"Timeline OpenMP Mutex Acquired Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_mutex_acquired,
	"openmp::mutex_acquired",
	"Timeline OpenMP Mutex Acquired Layer Configuration")

int am_render_dfg_timeline_openmp_mutex_acquired_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_nest_lock,
	"openmp::nest_lock",
	"Timeline OpenMP Nest Lock Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_nest_lock,
	"openmp::nest_lock",
	"Timeline OpenMP Nest Lock Layer Configuration")

int am_render_dfg_timeline_openmp_nest_lock_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_flush,
	"openmp::flush",
	"Timeline OpenMP Flush Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_flush,
	"openmp::flush",
	"Timeline OpenMP Flush Layer Configuration")

int am_render_dfg_timeline_openmp_flush_layer_configuration_node_process(
	struct am_dfg_node* n);

AM_RENDER_DFG_DECL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	openmp_cancel,
	"openmp::cancel",
	"Timeline OpenMP Cancel Layer Filter")

AM_RENDER_DFG_DECL_TIMELINE_LAYER_ENABLE_CONFIGURATION_NODE_TYPE(
	openmp_cancel,
	"openmp::cancel",
	"Timeline OpenMP Cancel Layer Configuration")

int am_render_dfg_timeline_openmp_cancel_layer_configuration_node_process(
	struct am_dfg_node* n);

int am_render_dfg_timeline_openmp_stack_frame_period_layer_dominant_period_at_pos_node_type_process(
	struct am_dfg_node* n);

/* Node that extracts the dominant Stack Frame Period at the pixel intervals */
AM_DFG_DECL_BUILTIN_NODE_TYPE(
	am_render_dfg_timeline_openmp_stack_frame_period_layer_dominant_period_at_pos_node_type,
	"am::render::timeline::layer::openmp::stack_frame_period::dominant_period_at_pos",
	"Dominant Stack Frame Frame Period at Position",
	AM_DFG_NODE_DEFAULT_SIZE,
	AM_DFG_DEFAULT_PORT_DEPS_PURE_FUNCTIONAL,
	AM_DFG_NODE_FUNCTIONS({
		.process = am_render_dfg_timeline_openmp_stack_frame_period_layer_dominant_period_at_pos_node_type_process
	}),
	AM_DFG_NODE_PORTS(
		{ "layer",
		  "const am::render::timeline::layer::core::stack_frame_period*",
		  AM_DFG_PORT_IN },
		{ "mouse position",
		  "am::core::pair<am::core::timestamp,const am::core::hierarchy_node*>",
		  AM_DFG_PORT_IN },
		{ "dominant stack_frame_period",
		  "const am::core::stack_frame_period*",
		  AM_DFG_PORT_OUT }
	),
	AM_DFG_PORT_DEPS(),
	AM_DFG_NODE_PROPERTIES())

AM_DFG_ADD_BUILTIN_NODE_TYPES(
	&am_render_dfg_timeline_openmp_for_loop_type_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_for_loop_type_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_for_loop_instance_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_for_loop_instance_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_iteration_set_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_iteration_set_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_iteration_period_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_iteration_period_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_task_type_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_task_type_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_task_instance_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_task_instance_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_task_period_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_task_period_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_thread_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_thread_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_parallel_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_parallel_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_task_create_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_task_create_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_task_schedule_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_task_schedule_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_implicit_task_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_implicit_task_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_sync_region_wait_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_sync_region_wait_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_mutex_released_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_mutex_released_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_dependences_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_dependences_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_task_dependence_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_task_dependence_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_work_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_work_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_master_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_master_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_sync_region_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_sync_region_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_lock_init_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_lock_init_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_lock_destroy_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_lock_destroy_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_mutex_acquire_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_mutex_acquire_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_mutex_acquired_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_mutex_acquired_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_nest_lock_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_nest_lock_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_flush_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_flush_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_cancel_layer_filter_node_type,
	&am_render_dfg_timeline_openmp_cancel_layer_configuration_node_type,
	&am_render_dfg_timeline_stack_frame_period_layer_filter_node_type,
	&am_render_dfg_timeline_stack_frame_period_layer_configuration_node_type,
	&am_render_dfg_timeline_function_symbol_layer_filter_node_type,
	&am_render_dfg_timeline_function_symbol_layer_configuration_node_type,
	&am_render_dfg_timeline_openmp_stack_frame_period_layer_dominant_period_at_pos_node_type)

#endif
