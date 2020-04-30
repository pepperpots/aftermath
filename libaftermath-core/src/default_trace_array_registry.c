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

#include <aftermath/core/default_trace_array_registry.h>
#include <aftermath/core/default_array_registry.h>
#include <aftermath/core/event_collection_array.h>
#include <aftermath/core/state_description_array.h>
#include <aftermath/core/counter_description_array.h>
#include <aftermath/core/counter_event_array.h>
#include <aftermath/core/counter_event_array_collection.h>
#include <aftermath/core/state_event_array.h>
#include <aftermath/core/measurement_interval_array.h>
#include <aftermath/core/openstream_task_type_array.h>
#include <aftermath/core/openstream_task_instance_array.h>
#include <aftermath/core/openstream_task_period_array.h>

#include <aftermath/core/openmp_task_instance_array.h>
#include <aftermath/core/openmp_task_type_array.h>
#include <aftermath/core/openmp_task_period_array.h>

#include <aftermath/core/openmp_for_loop_type_array.h>
#include <aftermath/core/openmp_for_loop_instance_array.h>
#include <aftermath/core/openmp_iteration_set_array.h>
#include <aftermath/core/openmp_iteration_period_array.h>

#include <aftermath/core/openmp_thread_array.h>
#include <aftermath/core/openmp_parallel_array.h>
#include <aftermath/core/openmp_task_create_array.h>
#include <aftermath/core/openmp_task_schedule_array.h>
#include <aftermath/core/openmp_implicit_task_array.h>
#include <aftermath/core/openmp_sync_region_wait_array.h>
#include <aftermath/core/openmp_mutex_released_array.h>
#include <aftermath/core/openmp_dependences_array.h>
#include <aftermath/core/openmp_task_dependence_array.h>
#include <aftermath/core/openmp_work_array.h>
#include <aftermath/core/openmp_master_array.h>
#include <aftermath/core/openmp_sync_region_array.h>
#include <aftermath/core/openmp_lock_init_array.h>
#include <aftermath/core/openmp_lock_destroy_array.h>
#include <aftermath/core/openmp_mutex_acquire_array.h>
#include <aftermath/core/openmp_mutex_acquired_array.h>
#include <aftermath/core/openmp_nest_lock_array.h>
#include <aftermath/core/openmp_flush_array.h>
#include <aftermath/core/openmp_cancel_array.h>
#include <aftermath/core/openmp_loop_array.h>
#include <aftermath/core/openmp_loop_chunk_array.h>

#include <aftermath/core/tensorflow_node_array.h>
#include <aftermath/core/tensorflow_node_execution_array.h>

#include <aftermath/core/telamon_candidate_array.h>
#include <aftermath/core/telamon_candidate_evaluate_action_array.h>
#include <aftermath/core/telamon_candidate_expand_action_array.h>
#include <aftermath/core/telamon_candidate_kill_action_array.h>
#include <aftermath/core/telamon_candidate_mark_implementation_action_array.h>
#include <aftermath/core/telamon_candidate_select_action_array.h>
#include <aftermath/core/telamon_candidate_select_child_action_array.h>
#include <aftermath/core/telamon_thread_trace_array.h>

#include <aftermath/core/on_disk_meta.h>

#include <stdlib.h>

AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_state_description_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_counter_description_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_measurement_interval_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openstream_task_type_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openstream_task_instance_array)

AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_state_event_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_counter_event_array_collection)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openstream_task_period_array)

AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_task_instance_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_task_period_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_task_type_array)

AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_for_loop_instance_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_for_loop_type_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_iteration_period_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_iteration_set_array)

AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_thread_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_parallel_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_task_create_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_task_schedule_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_implicit_task_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_sync_region_wait_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_mutex_released_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_dependences_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_task_dependence_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_work_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_master_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_sync_region_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_lock_init_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_lock_destroy_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_mutex_acquire_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_mutex_acquired_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_nest_lock_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_flush_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_cancel_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_loop_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_openmp_loop_chunk_array)

AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_tensorflow_node_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_tensorflow_node_execution_array)

AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_telamon_candidate_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_telamon_candidatep_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_telamon_candidate_evaluate_action_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_telamon_candidate_expand_action_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_telamon_candidate_kill_action_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_telamon_candidate_mark_implementation_action_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_telamon_candidate_select_action_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_telamon_candidate_select_child_action_array)
AM_DECL_DEFAULT_ARRAY_REGISTRY_FUNCTIONS(am_telamon_thread_trace_array)

int am_build_default_trace_array_registry(struct am_array_registry* r)
{
	if(AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_state_description_array,
					      "am::core::state_description") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_counter_description_array,
					      "am::core::counter_description") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_measurement_interval_array,
					      "am::core::measurement_interval") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openstream_task_type_array,
					      "am::openstream::task_type") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openstream_task_instance_array,
					      "am::openstream::task_instance") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openstream_task_period_array,
					      "am::openstream::task_period") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_state_event_array,
					      "am::core::state_event") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_counter_event_array_collection,
					      "am::core::counter_event") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_task_instance_array,
					      "am::openmp::task_instance") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_task_period_array,
					      "am::openmp::task_period") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_task_type_array,
					      "am::openmp::task_type") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_for_loop_instance_array,
					      "am::openmp::for_loop_instance")||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_for_loop_type_array,
					      "am::openmp::for_loop_type") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_iteration_period_array,
					      "am::openmp::iteration_period") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_iteration_set_array,
					      "am::openmp::iteration_set") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_thread_array,
					      "am::openmp::thread") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_parallel_array,
					      "am::openmp::parallel") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_task_create_array,
					      "am::openmp::task_create") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_task_schedule_array,
					      "am::openmp::task_schedule") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_implicit_task_array,
					      "am::openmp::implicit_task") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_sync_region_wait_array,
					      "am::openmp::sync_region_wait") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_mutex_released_array,
					      "am::openmp::mutex_released") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_dependences_array,
					      "am::openmp::dependences") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_task_dependence_array,
					      "am::openmp::task_dependence") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_work_array,
					      "am::openmp::work") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_master_array,
					      "am::openmp::master") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_sync_region_array,
					      "am::openmp::sync_region") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_lock_init_array,
					      "am::openmp::lock_init") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_lock_destroy_array,
					      "am::openmp::lock_destroy") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_mutex_acquire_array,
					      "am::openmp::mutex_acquire") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_mutex_acquired_array,
					      "am::openmp::mutex_acquired") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_nest_lock_array,
					      "am::openmp::nest_lock") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_flush_array,
					      "am::openmp::flush") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_loop_array,
					      "am::openmp::loop") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_loop_chunk_array,
					      "am::openmp::loop_chunk") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_openmp_cancel_array,
					      "am::openmp::cancel") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_tensorflow_node_array,
					      "am::tensorflow::node") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_tensorflow_node_execution_array,
					      "am::tensorflow::node_execution") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_telamon_candidate_array,
					      "am::telamon::candidate") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_telamon_candidate_evaluate_action_array,
					      "am::telamon::action::evaluate_candidate") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_telamon_candidate_expand_action_array,
					      "am::telamon::action::expand_candidate") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_telamon_candidate_kill_action_array,
					      "am::telamon::action::kill_candidate") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_telamon_candidate_mark_implementation_action_array,
					      "am::telamon::action::mark_implementation") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_telamon_candidate_select_action_array,
					      "am::telamon::action::select_candidate") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_telamon_candidate_select_child_action_array,
					      "am::telamon::action::select_child") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_telamon_candidatep_array,
					      "am::telamon::candidate_root") ||
	   AM_DEFAULT_ARRAY_REGISTRY_REGISTER(r, am_telamon_thread_trace_array,
					      "am::telamon::thread_trace"))
	{
		return 1;
	}

	if(am_build_default_meta_array_registry(r))
		return 1;

	return 0;
}
