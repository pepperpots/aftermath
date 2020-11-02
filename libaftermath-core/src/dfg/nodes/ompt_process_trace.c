/**
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

#include <aftermath/core/trace.h>
#include <aftermath/core/openmp_task_period_array.h>
#include <aftermath/core/openmp_task_type_array.h>
#include <aftermath/core/openmp_task_instance_array.h>

#include "ompt_process_trace.h"

#define ADDR2LINE_BUFFER_SIZE 256

int am_dfg_ompt_process_trace_node_init(struct am_dfg_node* n)
{
	struct am_dfg_node_ompt_process_trace* t = (typeof(t))n;

	t->trace = NULL;

	return 0;
}

/* For the given executable and the line number lookup function name, file name and the line number.
   Results are stored in first 3 arguments of the function.

  addr2line.sh source code:

  '''
  HEX=$(printf "%x" ${2})
  OUTPUT=$(addr2line -f -e ${1} ${HEX})
  echo $OUTPUT | sed -r 's/:/\n/g' | sed -r 's/\s/\n/g'
  '''
*/
static int am_dfg_ompt_process_trace_node_addr2line(char** func_name, char** file_name,
  unsigned long* line, char* executable_name, unsigned long codeptr_ra)
{
  *func_name = (char*) malloc(ADDR2LINE_BUFFER_SIZE);
  if(*func_name == NULL)
    goto out_err_terminate;

  *file_name = (char*) malloc(ADDR2LINE_BUFFER_SIZE);
  if(*file_name == NULL)
    goto out_err_destroy_func_name;

  char command[ADDR2LINE_BUFFER_SIZE];

  int length = snprintf(command, ADDR2LINE_BUFFER_SIZE, "./addr2line.sh %s %ld",
    executable_name, codeptr_ra);

  if(length > ADDR2LINE_BUFFER_SIZE)
    goto out_err_destroy_file_name;

  FILE* fp = popen(command, "r");
  if (fp == NULL)
    goto out_err_destroy_file_name;

  if(fgets(*func_name, ADDR2LINE_BUFFER_SIZE, fp) == NULL)
    goto out_err_destroy_file;

  // Remove the trailing '\n' from the string
  char* func_name_res = strtok(*func_name, "\n");
  if(func_name_res == NULL)
    goto out_err_destroy_file;

  if(fgets(*file_name, ADDR2LINE_BUFFER_SIZE, fp) == NULL)
    goto out_err_destroy_file;

  // Remove the trailing '\n' from the string
  char* file_name_res = strtok(*file_name, "\n");
  if(file_name_res == NULL)
    goto out_err_destroy_file;

  char tmp_buffer[8];

  if(fgets(tmp_buffer, sizeof(tmp_buffer), fp) == NULL)
    goto out_err_destroy_file;
  *line = atoi(tmp_buffer);

  pclose(fp);

  return 0;

out_err_destroy_file:
  pclose(fp);
out_err_destroy_file_name:
  free(*file_name);
out_err_destroy_func_name:
  free(*func_name);
out_err_terminate:
  return 1;
}

static int am_dfg_ompt_process_trace_node_process_trace(struct am_trace* trace)
{
  // Stage I: Prepare required data structures
  struct am_openmp_task_type_array* task_type_array;
  task_type_array = (struct am_openmp_task_type_array*)
    malloc(sizeof(struct am_openmp_task_type_array));
  if(task_type_array == NULL)
    goto out_err_terminate;
  am_openmp_task_type_array_init(task_type_array);

  struct am_openmp_task_instance_array* task_instance_array;
  task_instance_array = (struct am_openmp_task_instance_array*)
    malloc(sizeof(struct am_openmp_task_instance_array));
  if(task_instance_array == NULL)
    goto out_err_destroy_type_array;
  am_openmp_task_instance_array_init(task_instance_array);

  // Stage II: Extract information about tasks
  struct am_event_collection_array_iter iter;
  am_trace_iter_each_event_collection_array(trace, "am::ompt::task_create", iter) {
    struct am_ompt_task_create* events = iter.arr->elements;
    for(int i = 0; i < iter.arr->num_elements; i++) {
      uint64_t codeptr_ra = events[i].codeptr_ra;
      //uint64_t task_id = events[i].new_task_id;

      // Stage II.I: Create new task type
      struct am_openmp_task_type* task_type;
      task_type = (struct am_openmp_task_type*)
        malloc(sizeof(struct am_openmp_task_type));
      // If failed to allocate just skip the current task types - no memory leak
      if(task_type == NULL)
        continue;

      // TODO: Make file name an input parameter to the node
      // Subtracting 1 from the codeptr_ra gives a correct line number for the final task and
      // changes nothing for other tasks - no idea why but it works.
      if(am_dfg_ompt_process_trace_node_addr2line(&task_type->name, &task_type->source.file,
        &task_type->source.line, "example", codeptr_ra - 1)) {
        free(task_type);
        // Addr2line failed so skip adding this task
        continue;
      }

      task_type->source.character = 0;

      short add_new_type = true;

      // TODO: For performance we should run a check based on codeptr_ra, before addr2line is
      // called, however writing this in C is an effort (easy when C++ map is used).
      struct am_openmp_task_type* tt = NULL;
      for(int j = 0; j < task_type_array->num_elements; j++) {
        tt = &task_type_array->elements[j];
        if(strcmp(tt->name, task_type->name) == 0) {
          free(task_type->name);
          free(task_type->source.file);
          free(task_type);
          add_new_type = false;
          break;
        }
      }

      if(add_new_type)
        am_openmp_task_type_array_appendp(task_type_array, task_type);

      // Stage II.II: Create new task instance
      struct am_openmp_task_instance* task_instance;
      task_instance = (struct am_openmp_task_instance*)
        malloc(sizeof(struct am_openmp_task_instance));
      // If failed to allocate just skip the current task instance - no memory leak
      if(task_instance == NULL)
        continue;

      if(add_new_type)
        task_instance->task_type = task_type;
      else
        task_instance->task_type = tt;

      am_openmp_task_instance_array_appendp(task_instance_array, task_instance);
    }
  }

  struct am_array_collection* trace_ac = &trace->trace_arrays;

  // If there are no task types, then also there are no instances, nor periods.
  if(task_type_array->num_elements > 0) {
    if(am_array_collection_add(trace_ac, (struct am_typed_array_generic*) task_type_array,
      "am::openmp::task_type")) {
      // TODO: Improve error handling!
      printf("Cannot add task types to the trace!\n");
    }

    if(am_array_collection_add(trace_ac, (struct am_typed_array_generic*) task_instance_array,
      "am::openmp::task_instance")) {
      // TODO: Improve error handling!
      printf("Cannot add task instances to the trace!\n");
    }
  }
  else {
    free(task_type_array);
    free(task_instance_array);
    // It's valid to have not tasks, so finish without an error
    return 0;
  }

  // Stage III: Extract information about tasks' intervals (periods)
  am_trace_iter_each_event_collection_array(trace, "am::ompt::task_schedule", iter) {

  }

  // TODO: For development only - remove later!
  printf("Print task instances:\n");
  for(int j = 0; j < task_instance_array->num_elements; j++) {
    struct am_openmp_task_instance ti = task_instance_array->elements[j];
    printf("%s %s %lu\n", ti.task_type->name, ti.task_type->source.file, ti.task_type->source.line);
  }

  return 0;

out_err_destroy_type_array:
  free(task_type_array);
out_err_terminate:
  return 1;
}

int am_dfg_ompt_process_trace_node_process(struct am_dfg_node* n)
{
	struct am_dfg_node_ompt_process_trace* t = (typeof(t))n;
	struct am_dfg_port* ptrace_in = &n->ports[0];
	struct am_dfg_port* ptrace_out = &n->ports[1];

  if(!am_dfg_port_is_connected(ptrace_in) || !am_dfg_port_is_connected(ptrace_out))
    return 0;

	if(am_dfg_port_activated_and_has_data(ptrace_in)) {
		if(am_dfg_buffer_read_last(ptrace_in->buffer, &t->trace))
			return 1;

    am_dfg_ompt_process_trace_node_process_trace(t->trace);

	  return am_dfg_buffer_write(ptrace_out->buffer, 1, &t->trace);
  }

  return 0;
}
