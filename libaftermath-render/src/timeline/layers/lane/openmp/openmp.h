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

#ifndef AM_TIMELINE_LANE_RENDERER_OPENMP_H
#define AM_TIMELINE_LANE_RENDERER_OPENMP_H

struct am_timeline_render_layer_type*
am_timeline_stack_frame_period_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_function_symbol_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_for_loop_type_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_for_loop_instance_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_iteration_set_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_iteration_period_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_task_type_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_task_instance_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_task_period_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_thread_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_parallel_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_task_create_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_task_schedule_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_implicit_task_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_sync_region_wait_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_mutex_released_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_dependences_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_task_dependence_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_work_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_master_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_sync_region_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_lock_init_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_lock_destroy_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_mutex_acquire_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_mutex_acquired_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_nest_lock_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_flush_layer_instantiate_type(void);

struct am_timeline_render_layer_type*
am_timeline_openmp_cancel_layer_instantiate_type(void);

#endif
