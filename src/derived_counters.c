/**
 * Copyright (C) 2013 Andi Drebes <andi.drebes@lip6.fr>
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
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "derived_counters.h"

int get_min_index(struct multi_event_set* mes, int* curr_idx, uint64_t* curr_start)
{
	int i;
	int min_idx = -1;
	uint64_t min = UINT64_MAX;
	uint64_t time;

	for(i = 0; i < mes->num_sets; i++) {
		if(curr_idx[i] != -1) {
			time = curr_start[i];

			if(time < min) {
				min = time;
				min_idx = i;
			}
		}
	}

	return min_idx;
}

int derive_aggregate_counter(struct multi_event_set* mes, struct counter_description** cd_out, const char* counter_name, unsigned int counter_idx, int num_samples, int cpu)
{
	struct counter_description* cd;
	struct event_set* cpu_es;
	struct counter_event ce;

	int set_indexes[mes->num_sets];
	int sets_having[mes->num_sets];
	int num_sets_having;
	int cpu_idx;

	uint64_t id;
	uint64_t min_time = multi_event_set_first_event_start(mes);
	uint64_t max_time = multi_event_set_last_event_end(mes);
	uint64_t interval_length = (max_time - min_time) / (uint64_t)num_samples;

	cpu_idx = multi_event_set_find_cpu_idx(mes, cpu);
	cpu_es = &mes->sets[cpu_idx];

	id = multi_event_set_get_free_counter_id(mes);

	if(!(cd = multi_event_set_counter_description_alloc_ptr(mes, id, strlen(counter_name))))
		return 1;

	cd->counter_id = id;
	strcpy(cd->name, counter_name);

	ce.active_task = multi_event_set_find_task_by_addr(mes, 0x0);
	ce.active_frame = multi_event_set_find_frame_by_addr(mes, 0x0);
	ce.counter_id = id;
	ce.counter_index = cd->index;

	num_sets_having = 0;
	for(int i = 0; i < mes->num_sets; i++) {
		int j = event_set_counter_event_set_index(&mes->sets[i], counter_idx);

		if(j != -1) {
			sets_having[num_sets_having] = i;
			set_indexes[num_sets_having] = j;
			num_sets_having++;
		}
	}

	for(int sample = 0; sample < num_samples; sample++) {
		ce.time = min_time + (uint64_t)sample * interval_length;
		ce.value = 0;

		for(int i = 0; i < num_sets_having; i++)
			ce.value += counter_event_set_get_value(&mes->sets[sets_having[i]].counter_event_sets[set_indexes[i]], ce.time);

		if(event_set_add_counter_event(cpu_es, &ce) != 0)
			return 1;

		multi_event_set_check_update_counter_bounds(mes, &ce);
	}

	*cd_out = cd;

	return 0;
}

int derive_parallelism_counter(struct multi_event_set* mes, struct counter_description** cd_out, const char* counter_name, enum worker_state state, int num_samples, int cpu)
{
	struct counter_description* cd;
	int cpu_idx;
	struct event_set* cpu_es;
	uint64_t id;
	int curr_idx[mes->num_sets];
	uint64_t curr_start[mes->num_sets];

	struct state_event* se;
	int min_idx;
	int parallelism = 0;
	uint64_t min_time = multi_event_set_first_event_start(mes);
	uint64_t max_time = multi_event_set_last_event_end(mes);
	uint64_t interval_length = (max_time - min_time) / (uint64_t)num_samples;
	uint64_t interval_start;
	uint64_t interval_end;
	struct counter_event ce;

	cpu_idx = multi_event_set_find_cpu_idx(mes, cpu);
	cpu_es = &mes->sets[cpu_idx];

	id = multi_event_set_get_free_counter_id(mes);

	if(!(cd = multi_event_set_counter_description_alloc_ptr(mes, id, strlen(counter_name))))
		return 1;

	cd->counter_id = id;
	strcpy(cd->name, counter_name);

	for(int i = 0; i < mes->num_sets; i++) {
		if(mes->sets[i].num_state_events > 0)
			curr_idx[i] = event_set_get_next_state_event(&mes->sets[i], -1, state);
		else
			curr_idx[i] = -1;

		curr_start[i] = (curr_idx[i] != -1) ? mes->sets[i].state_events[curr_idx[i]].start : 0;
	}

	for(int sample = 0; sample < num_samples; sample++) {
		interval_start = min_time + (uint64_t)sample * interval_length;
		interval_end = interval_start + interval_length;
		parallelism = 0;

		while((min_idx = get_min_index(mes, curr_idx, curr_start)) != -1 &&
			curr_start[min_idx] < interval_end)
		{
			se = &mes->sets[min_idx].state_events[curr_idx[min_idx]];

			if(se->end < interval_end) {
				curr_idx[min_idx] = event_set_get_next_state_event(&mes->sets[min_idx], curr_idx[min_idx], state);
				parallelism += se->end - curr_start[min_idx];

				if(curr_idx[min_idx] != -1)
					curr_start[min_idx] = mes->sets[min_idx].state_events[curr_idx[min_idx]].start;
			} else {
				parallelism += interval_end - curr_start[min_idx];
				curr_start[min_idx] = interval_end;
			}
		}

		ce.time = interval_start + interval_length / 2;
		ce.active_task = multi_event_set_find_task_by_addr(mes, 0x0);
		ce.active_frame = multi_event_set_find_frame_by_addr(mes, 0x0);
		ce.counter_id = id;
		ce.counter_index = cd->index;
		ce.value = parallelism / interval_length;

		if(event_set_add_counter_event(cpu_es, &ce) != 0)
			return 1;

		multi_event_set_check_update_counter_bounds(mes, &ce);
	}

	*cd_out = cd;

	return 0;
}

int derive_numa_contention_counter_spikes(struct multi_event_set* mes, struct counter_description** cd_out, const char* counter_name, unsigned int numa_node, enum access_type contention_type, int num_samples, int cpu)
{
	struct counter_description* cd;
	int cpu_idx;
	struct event_set* cpu_es;
	uint64_t id;
	int curr_idx[mes->num_sets];
	uint64_t curr_time[mes->num_sets];

	struct comm_event* ce;
	int min_idx;
	uint64_t min_time = multi_event_set_first_event_start(mes);
	uint64_t max_time = multi_event_set_last_event_end(mes);
	uint64_t interval_length = (max_time - min_time) / (uint64_t)num_samples;
	uint64_t interval_start;
	uint64_t interval_end;
	struct counter_event cre;
	enum comm_event_type comm_types[] = { COMM_TYPE_DATA_WRITE, COMM_TYPE_DATA_READ };
	int num_comm_types = 2;

	cpu_idx = multi_event_set_find_cpu_idx(mes, cpu);
	cpu_es = &mes->sets[cpu_idx];

	id = multi_event_set_get_free_counter_id(mes);

	if(!(cd = multi_event_set_counter_description_alloc_ptr(mes, id, strlen(counter_name))))
		return 1;

	cd->counter_id = id;
	strcpy(cd->name, counter_name);

	for(int i = 0; i < mes->num_sets; i++) {
		if(mes->sets[i].num_comm_events > 0)
			curr_idx[i] = event_set_get_next_comm_event_arr(&mes->sets[i], -1, num_comm_types, comm_types);
		else
			curr_idx[i] = -1;

		curr_time[i] = (curr_idx[i] != -1) ? mes->sets[i].comm_events[curr_idx[i]].time : 0;
	}

	cre.value = 0;

	for(int sample = 0; sample < num_samples; sample++) {
		interval_start = min_time + (uint64_t)sample * interval_length;
		interval_end = interval_start + interval_length;

		while((min_idx = get_min_index(mes, curr_idx, curr_time)) != -1 &&
			curr_time[min_idx] < interval_end)
		{
			ce = &mes->sets[min_idx].comm_events[curr_idx[min_idx]];

			if((contention_type == ACCESS_TYPE_READS_AND_WRITES &&
			    (ce->type == COMM_TYPE_DATA_READ || ce->type == COMM_TYPE_DATA_WRITE)) ||
			   (contention_type == ACCESS_TYPE_READS_ONLY && ce->type == COMM_TYPE_DATA_READ) ||
			   (contention_type == ACCESS_TYPE_WRITES_ONLY && ce->type == COMM_TYPE_DATA_WRITE))
			{
				if(ce->what->numa_node == numa_node)
					cre.value += ce->size;
			}

			curr_idx[min_idx] = event_set_get_next_comm_event_arr(&mes->sets[min_idx], curr_idx[min_idx], num_comm_types, comm_types);

			if(curr_idx[min_idx] != -1)
				curr_time[min_idx] = mes->sets[min_idx].comm_events[curr_idx[min_idx]].time;
		}

		cre.time = interval_start + interval_length / 2;
		cre.active_task = multi_event_set_find_task_by_addr(mes, 0x0);
		cre.active_frame = multi_event_set_find_frame_by_addr(mes, 0x0);
		cre.counter_id = id;
		cre.counter_index = cd->index;

		if(event_set_add_counter_event(cpu_es, &cre) != 0)
			return 1;

		multi_event_set_check_update_counter_bounds(mes, &cre);
	}

	*cd_out = cd;

	return 0;
}

static uint64_t get_comm_bytes_in_interval(struct event_set* es, uint64_t start, uint64_t end, unsigned int numa_node, enum access_type contention_type)
{
	struct comm_event* ce;
	uint64_t bytes = 0;

	for_each_comm_event_in_interval(es, start, end, ce) {
		if((contention_type == ACCESS_TYPE_READS_AND_WRITES &&
		    (ce->type == COMM_TYPE_DATA_READ || ce->type == COMM_TYPE_DATA_WRITE)) ||
		   (contention_type == ACCESS_TYPE_READS_ONLY && ce->type == COMM_TYPE_DATA_READ) ||
		   (contention_type == ACCESS_TYPE_WRITES_ONLY && ce->type == COMM_TYPE_DATA_WRITE))
		{
			if(ce->what->numa_node == numa_node)
				bytes += ce->size;
		}
	}

	return bytes;
}

int derive_numa_contention_counter_linear(struct multi_event_set* mes, struct counter_description** cd_out, const char* counter_name, unsigned int numa_node, enum access_type contention_type, int num_samples, int cpu)
{
	struct counter_description* cd;
	int cpu_idx;
	struct event_set* cpu_es;
	uint64_t id;
	int curr_idx[mes->num_sets];
	uint64_t curr_start[mes->num_sets];
	uint64_t curr_end[mes->num_sets];
	uint64_t curr_length[mes->num_sets];
	uint64_t curr_bytes[mes->num_sets];

	int min_idx;
	uint64_t min_time = multi_event_set_first_event_start(mes);
	uint64_t max_time = multi_event_set_last_event_end(mes);
	uint64_t interval_length = (max_time - min_time) / (uint64_t)num_samples;
	uint64_t interval_start;
	uint64_t interval_end;
	struct counter_event ce;

	cpu_idx = multi_event_set_find_cpu_idx(mes, cpu);
	cpu_es = &mes->sets[cpu_idx];

	id = multi_event_set_get_free_counter_id(mes);

	if(!(cd = multi_event_set_counter_description_alloc_ptr(mes, id, strlen(counter_name))))
		return 1;

	cd->counter_id = id;
	strcpy(cd->name, counter_name);

	for(int i = 0; i < mes->num_sets; i++) {
		curr_idx[i] = event_set_get_next_single_event(&mes->sets[i], -1, SINGLE_TYPE_TEXEC_START);
		curr_start[i] = (curr_idx[i] != -1) ? mes->sets[i].single_events[curr_idx[i]].time : 0;
		curr_end[i] = (curr_idx[i] != -1) ? mes->sets[i].single_events[curr_idx[i]].next_texec_end->time : 0;
		curr_length[i] = (curr_idx[i] != -1) ? curr_end[i] - curr_start[i] : 0;
		curr_bytes[i] = (curr_idx[i] != -1) ? get_comm_bytes_in_interval(&mes->sets[i], curr_start[i], curr_end[i], numa_node, contention_type) : 0;
	}

	ce.value = 0;
	ce.active_task = multi_event_set_find_task_by_addr(mes, 0x0);
	ce.active_frame = multi_event_set_find_frame_by_addr(mes, 0x0);
	ce.counter_id = id;

	for(int sample = 0; sample < num_samples; sample++) {
		interval_start = min_time + (uint64_t)sample * interval_length;
		interval_end = interval_start + interval_length;

		while((min_idx = get_min_index(mes, curr_idx, curr_start)) != -1 &&
			curr_start[min_idx] < interval_end)
		{
			if(curr_end[min_idx] < interval_end) {
				ce.value += ((curr_end[min_idx] - curr_start[min_idx]) * curr_bytes[min_idx]) / curr_length[min_idx];

				curr_idx[min_idx] = event_set_get_next_single_event(&mes->sets[min_idx], curr_idx[min_idx], SINGLE_TYPE_TEXEC_START);
				if(curr_idx[min_idx] != -1) {
					curr_start[min_idx] = mes->sets[min_idx].single_events[curr_idx[min_idx]].time;
					curr_end[min_idx] = mes->sets[min_idx].single_events[curr_idx[min_idx]].next_texec_end->time;
					curr_length[min_idx] = curr_end[min_idx] - curr_start[min_idx];
					curr_bytes[min_idx] = get_comm_bytes_in_interval(&mes->sets[min_idx], curr_start[min_idx], curr_end[min_idx], numa_node, contention_type);
				}
			} else {
				ce.value += ((interval_end - curr_start[min_idx]) * curr_bytes[min_idx]) / curr_length[min_idx];
				curr_start[min_idx] = interval_end;
			}
		}

		ce.time = interval_start + interval_length / 2;
		ce.counter_index = cd->index;

		if(event_set_add_counter_event(cpu_es, &ce) != 0)
			return 1;

		multi_event_set_check_update_counter_bounds(mes, &ce);
	}

	*cd_out = cd;

	return 0;
}

int derive_numa_contention_counter(struct multi_event_set* mes, struct counter_description** cd_out, const char* counter_name, unsigned int numa_node, enum access_type contention_type, enum access_model model, int num_samples, int cpu)
{
	switch(model) {
		case ACCESS_MODEL_SPIKES:
			return derive_numa_contention_counter_spikes(mes, cd_out, counter_name, numa_node, contention_type, num_samples, cpu);
		case ACCESS_MODEL_LINEAR:
			return derive_numa_contention_counter_linear(mes, cd_out, counter_name, numa_node, contention_type, num_samples, cpu);
	}

	return 1;
}
