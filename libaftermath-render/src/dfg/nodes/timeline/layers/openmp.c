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

#include <aftermath/render/dfg/nodes/timeline/layers/openmp.h>
#include <aftermath/render/timeline/layer.h>
#include <aftermath/render/timeline/layers/interval.h>
#include <aftermath/render/timeline/renderer.h>

AM_RENDER_DFG_IMPL_TIMELINE_LAYER_FILTER_NODE_TYPE(
	stack_frame_period,
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

