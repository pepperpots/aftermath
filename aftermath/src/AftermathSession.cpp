/**
 * Author: Andi Drebes <andi@drebesium.org>
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

#include "AftermathSession.h"
#include "dfg/nodes/builtin_nodes.h"
#include "dfg/nodes/gui/histogram.h"
#include "dfg/nodes/gui/hierarchy_combobox.h"
#include "dfg/nodes/gui/label.h"
#include "dfg/nodes/gui/telamon_candidate_tree.h"
#include "dfg/nodes/gui/timeline.h"
#include "dfg/nodes/gui/toolbar_button.h"
#include "dfg/nodes/gui/toolbar_togglebutton.h"
#include "dfg/types/builtin_types.h"
#include "gui/widgets/DFGWidget.h"
#include "gui/widgets/LabelWithDFGNode.h"
#include "gui/widgets/HierarchyComboBox.h"
#include "gui/widgets/HistogramWidget.h"
#include "gui/widgets/TelamonCandidateTreeWidget.h"
#include "gui/widgets/TimelineWidget.h"
#include "gui/widgets/ToolbarButton.h"

#include <map>
#include <vector>
#include <sstream>
#include <stack>
#include <QLabel>

#include <iostream>

extern "C" {
	#include <aftermath/render/timeline/common_layers.h>
	#include <aftermath/render/dfg/nodes/builtin_nodes.h>
	#include <aftermath/render/dfg/types/builtin_types.h>
	#include <aftermath/core/dfg_builtin_types.h>
	#include <aftermath/core/dfg_builtin_node_types.h>
	#include <aftermath/core/dfg/nodes/trace.h>
	#include <aftermath/core/frame_type_registry.h>
	#include <aftermath/core/io_context.h>
	#include <aftermath/core/io_error.h>
	#include <aftermath/core/on_disk.h>
	#include <aftermath/core/parse_status.h>
  #include <aftermath/core/openmp_task_period_array.h>
  #include <aftermath/core/openmp_task_type_array.h>
  #include <aftermath/core/openmp_task_instance_array.h>
  #include <aftermath/core/openmp_for_loop_type_array.h>
  #include <aftermath/core/openmp_for_loop_instance_array.h>
  #include <aftermath/core/openmp_iteration_set_array.h>
  #include <aftermath/core/openmp_iteration_period_array.h>
	#include <aftermath/core/function_symbol_array.h>
	#include <aftermath/core/stack_frame_array.h>
	#include <aftermath/core/stack_frame_period_array.h>
}

/* TODO: Temporary chaining of stack frames together by their execution interval */
void buildCallGraph(am_trace* trace, std::map<uint64_t, std::string>& symbols_by_addr)
{

	/* First go over the frames, associate the correct function symbol and build
   * vectors for variable-sized parent/child relations
	 * Then go over the frames again and build the statically sized child arrays
	 */

  am_event_collection_array ecs = trace->event_collections;

	am_function_symbol_array* function_symbols =
    (am_function_symbol_array*)malloc(sizeof(am_function_symbol_array));
  am_function_symbol_array_init(function_symbols);
	
  std::map<uint64_t, am_function_symbol*> function_symbol_map; // memory address to function_symbol
	std::map<uint64_t, size_t> symbol_frame_counts; // memory address to num instantiations
	std::map<am_stack_frame*, std::vector<am_stack_frame*> > parent_child_relations; // key = parent, value = list of children
	std::map<am_stack_frame*, std::vector<am_stack_frame_period*> > periods_per_frame;

	/* For each core, iterate over the execution and keep 'current_call_stack' updated
	 * Use this to build each frame's parent/child relations, and construct stack_frame_periods when the frame was executing at top of stack 
	 * Also link function symbol (built from binary on trace load) to the addresses of each frame
	 */

  // For each event collection
  for(unsigned i = 0; i < ecs.num_elements; i++)
  {

		//fprintf(stdout, "Processing core %d.\n", i);
    am_array_collection* ac = &ecs.elements[i].event_arrays;
	
		am_stack_frame_period_array* stack_frame_periods =
			(am_stack_frame_period_array*)malloc(sizeof(am_stack_frame_period_array));
		am_stack_frame_period_array_init(stack_frame_periods);

    // For each event type
    for(unsigned j = 0; j < ac->num_elements; j++)
    {
      am_array_collection_entry ace = ac->elements[j];

      if(strcmp("am::core::stack_frame", ace.type) == 0)
      {
        am_typed_array_generic* array = ace.array;

        am_stack_frame* events = (am_stack_frame*) array->elements;

				/* I'm not sorting the original events array, but I am going to
				 * traverse them in sorted order
			   */
				am_stack_frame** sf_pointer_array =
					(am_stack_frame**) malloc(array->num_elements * sizeof(am_stack_frame*));
        for(unsigned k = 0; k < array->num_elements; k++)
					sf_pointer_array[k] = &events[k];
				
				am_stack_frame_qsort_interval_start_ascending(
					sf_pointer_array, array->num_elements);

				std::vector<struct am_stack_frame*> current_call_stack;

				uint64_t previous_period_end = 0;

				// Per each frame
        for(unsigned k = 0; k < array->num_elements; k++)
        {
					//am_stack_frame* frame = &events[k];
					am_stack_frame* frame = sf_pointer_array[k];

					// allocate its function_symbol and update the symbol count
          if(function_symbol_map.count(frame->addr) == 0)
          {
						std::map<uint64_t, std::string>::iterator sym_iter = symbols_by_addr.find(frame->addr);

						std::string sym_name_str = "unknown";
						if(symbols_by_addr.size() > 0){
							if(sym_iter == symbols_by_addr.end()){
								// if we have symbols, and we don't find a symbol for this address, then fail 
								std::stringstream errss;
								errss << "Function with address " << frame->addr << " was not found ";
								errss << "in the binary.";
								throw AftermathException(errss.str());
							} else {
								sym_name_str = sym_iter->second;
							}
						} else {
							if(sym_iter == symbols_by_addr.end()){
								// if we do not have symbols, then set all as unknown
								symbols_by_addr.insert(std::make_pair(frame->addr, sym_name_str));
							} else {
								sym_name_str = sym_iter->second;
							}
						}

						symbol_frame_counts.insert(std::make_pair(frame->addr, 0));
            
						am_function_symbol* new_sym =
              (am_function_symbol*) malloc(sizeof(am_function_symbol));
						
						new_sym->addr = frame->addr;
						new_sym->name = (char*) malloc(sym_name_str.length()+1);
						strncpy(new_sym->name, sym_name_str.c_str(), sym_name_str.length());
						new_sym->name[sym_name_str.length()] = '\0';

						//std::cout << "Setting address " << frame->addr << " to " << sym_name_str << std::endl;

            function_symbol_map.emplace(frame->addr, new_sym);
            am_function_symbol_array_appendp(function_symbols, new_sym);
          }

					symbol_frame_counts.find(frame->addr)->second++;
					frame->function_symbol = function_symbol_map.at(frame->addr);

					// roll back up the stack frame checking if we have left any before this one started
					// we trace periods when they end (either when the frame ends, or when a child starts)
					// therefore we need to keep track of previous_period_end to serve as the
					// correct starting timestamp for the next period
					bool process_exits = true;
					while(process_exits){
						if(current_call_stack.size() > 0){
							am_stack_frame* top_frame = current_call_stack.back();

							// if my parent frame ended, then I need to trace a period between its end and its own parent's end (or my start, if my start is earlier)
							if(top_frame->interval.end < frame->interval.start){
								// the top frame finished before I started, so I am at least at the same depth as the current top frame (and will need to recurse further to check if I'm less deep)

								uint64_t period_start = previous_period_end;
								uint64_t period_end = top_frame->interval.end;

								// add this interval as a period to top_frame
								struct am_stack_frame_period* ending_period = 
									(am_stack_frame_period*) malloc(sizeof(am_stack_frame_period));
								ending_period->stack_frame = top_frame;
								ending_period->interval = {period_start, period_end};

								auto period_iter = periods_per_frame.find(top_frame);
								if(period_iter == periods_per_frame.end()){
									ending_period->rank = 0;
									std::vector<am_stack_frame_period*> periods = {ending_period};
									periods_per_frame.insert(std::make_pair(top_frame, periods));
								} else {
									ending_period->rank = period_iter->second.size();
									period_iter->second.push_back(ending_period);
								}

								//fprintf(stdout,"Tracing end period for frame %p with symbol %s from %lu to %lu.\n", top_frame, top_frame->function_symbol->name, period_start, period_end);
								am_stack_frame_period_array_appendp(stack_frame_periods, ending_period);

								previous_period_end = period_end; // this will then be the starting time for the next period
								current_call_stack.pop_back();

							} else {
								// so it didn't finish before I started, meaning I am a child of the parent
								// I should therefore trace a period from the previous end to my start and add it to the parent
								
								uint64_t period_start = previous_period_end;
								uint64_t period_end = frame->interval.start;

								struct am_stack_frame_period* period_before_start = 
									(am_stack_frame_period*) malloc(sizeof(am_stack_frame_period));
								period_before_start->stack_frame = top_frame;
								period_before_start->interval = {period_start, period_end};
								
								auto period_iter = periods_per_frame.find(top_frame);
								if(period_iter == periods_per_frame.end()){
									period_before_start->rank = 0;
									std::vector<am_stack_frame_period*> periods = {period_before_start};
									periods_per_frame.insert(std::make_pair(top_frame, periods));
								} else {
									period_before_start->rank = period_iter->second.size();
									period_iter->second.push_back(period_before_start);
								}
								
								//fprintf(stdout,"Tracing period (after child start) for frame %p with symbol %s from %lu to %lu.\n", top_frame, top_frame->function_symbol->name, period_start, period_end);

								am_stack_frame_period_array_appendp(stack_frame_periods, period_before_start);

								previous_period_end = period_end;

								process_exits = false;
							}
						} else {
							process_exits = false;
							previous_period_end = frame->interval.start;
						}
					}

					// add the top of the call stack as the parent of this starting frame
					if(current_call_stack.size() > 0){
						am_stack_frame* parent_frame = current_call_stack.back();

						// add the parent frame to the newly started frame
						frame->parent_frame = parent_frame;

						auto it = parent_child_relations.find(parent_frame);
						if(it == parent_child_relations.end()){
							std::vector<am_stack_frame*> children = {frame};
							parent_child_relations.insert(std::make_pair(parent_frame, children));
						} else {
							it->second.push_back(frame);
						}

					}
					
					frame->depth = current_call_stack.size() + 1;

					// push the newly started frame to the call stack
					//fprintf(stdout, "Started new period of frame %p with symbol %s at %lu.\n", frame, frame->function_symbol->name, frame->interval.start);
					current_call_stack.push_back(frame);

				}

				// now roll back up the call stack and trace the final periods
				bool process_exits = true;
				while(process_exits){
					if(current_call_stack.size() > 0){
						am_stack_frame* top_frame = current_call_stack.back();

						// there are no more frame starts, so just trace from previous_period_end to the frame's end
						uint64_t period_start = previous_period_end;
						uint64_t period_end = top_frame->interval.end;

						// add this interval as a period to top_frame
						struct am_stack_frame_period* ending_period = 
							(am_stack_frame_period*) malloc(sizeof(am_stack_frame_period));
						ending_period->stack_frame = top_frame;
						ending_period->interval = {period_start, period_end};

						auto period_iter = periods_per_frame.find(top_frame);
						if(period_iter == periods_per_frame.end()){
							ending_period->rank = 0;
							std::vector<am_stack_frame_period*> periods = {ending_period};
							periods_per_frame.insert(std::make_pair(top_frame, periods));
						} else {
							ending_period->rank = period_iter->second.size();
							period_iter->second.push_back(ending_period);
						}
						
						//fprintf(stdout,"Tracing final ending period for frame %p with symbol %s from %lu to %lu.\n", top_frame, top_frame->function_symbol->name, period_start, period_end);
						
						am_stack_frame_period_array_appendp(stack_frame_periods, ending_period);

						previous_period_end = period_end;
						current_call_stack.pop_back();

					} else {
						process_exits = false;
					}
				}

				delete[] sf_pointer_array;
			}
		}
	
		if(stack_frame_periods->num_elements > 0)
		{
      am_array_collection_add(ac, (am_typed_array_generic*) stack_frame_periods,
        "am::core::stack_frame_period");
		}
	
	}

	std::cout << "Function call breakdown:" << std::endl;
	for(auto map_iter : symbol_frame_counts)
		std::cout << map_iter.first << " (" << symbols_by_addr.find(map_iter.first)->second << "):" << map_iter.second << std::endl;

  am_array_collection* global_ac = &trace->trace_arrays;

	if(function_symbols->num_elements > 0)
  {
    am_array_collection_add(global_ac, (am_typed_array_generic*) function_symbols,
      "am::core::function_symbol");
  }

	// Now build the per-frame child arrays
	for(auto relation : parent_child_relations){

		am_stack_frame* parent_frame = relation.first;
		std::vector<am_stack_frame*> child_frames_vec = relation.second;

		// initialise the parent frame's child array
		parent_frame->child_frames = (am_stack_frame**) malloc(child_frames_vec.size() * sizeof(am_stack_frame*));

		for(decltype(child_frames_vec)::size_type child_idx=0; child_idx<child_frames_vec.size(); child_idx++)
			parent_frame->child_frames[child_idx] = child_frames_vec.at(child_idx);

		parent_frame->num_child_frames = child_frames_vec.size();

	}

	// Now build the per-frame period arrays
	for(auto period_iter : periods_per_frame){

		am_stack_frame* frame = period_iter.first;
		std::vector<am_stack_frame_period*> periods_vec = period_iter.second;

		// initialise the frame's periods array
		frame->periods = (am_stack_frame_period**) malloc(periods_vec.size() * sizeof(am_stack_frame_period*));
	
		for(decltype(periods_vec)::size_type period_idx=0; period_idx<periods_vec.size(); period_idx++)
			frame->periods[period_idx] = periods_vec.at(period_idx);

		frame->num_periods = periods_vec.size();

	}

}

/* TODO: Temporary processing function for loop/task types, instances, set,
   periods */
void processTrace(am_trace* trace, std::map<uint64_t, std::string>& symbols_by_addr)
{
  am_event_collection_array ecs = trace->event_collections;

  // Per trace arrays
  am_openmp_task_type_array* task_types =
    (am_openmp_task_type_array*)malloc(sizeof(am_openmp_task_type_array));
  am_openmp_task_type_array_init(task_types);

  am_openmp_task_instance_array* task_instances =
    (am_openmp_task_instance_array*)malloc(sizeof(am_openmp_task_instance_array));
  am_openmp_task_instance_array_init(task_instances);

  am_openmp_for_loop_type_array* for_loop_types =
    (am_openmp_for_loop_type_array*)malloc(sizeof(am_openmp_for_loop_type_array));
  am_openmp_for_loop_type_array_init(for_loop_types);

  am_openmp_for_loop_instance_array* for_loop_instances =
    (am_openmp_for_loop_instance_array*)malloc(sizeof(am_openmp_for_loop_instance_array));
  am_openmp_for_loop_instance_array_init(for_loop_instances);

  am_openmp_iteration_set_array* iteration_sets =
    (am_openmp_iteration_set_array*)malloc(sizeof(am_openmp_iteration_set_array));
  am_openmp_iteration_set_array_init(iteration_sets);
  
  std::map<uint64_t, am_openmp_task_type*> task_type_map;
  std::map<uint64_t, am_openmp_task_instance*> task_instance_map;

  std::map<uint64_t, am_openmp_for_loop_type*> for_loop_type_map;
  std::map<uint64_t, am_openmp_for_loop_instance*> for_loop_instance_map;
  std::map<std::pair<uint64_t, std::pair<int64_t, int64_t>>, am_openmp_iteration_set*> iter_set_map;
				
  // We first lift per trace types
  // For each event collection
  for(unsigned i = 0; i < ecs.num_elements; i++)
  {
    am_array_collection ac = ecs.elements[i].event_arrays;

    // For each event type
    for(unsigned j = 0; j < ac.num_elements; j++)
    {
      am_array_collection_entry ace = ac.elements[j];

      // Tasks
      if(strcmp("am::openmp::task_create", ace.type) == 0)
      {
        am_typed_array_generic* array = ace.array;

        am_openmp_task_create* events =
          (am_openmp_task_create*) array->elements;

        // Per each event
        for(unsigned k = 0; k < array->num_elements; k++)
        {
          uint64_t codeptr_ra = events[k].codeptr_ra;

          // Unknown type so add it
          if(task_type_map.count(codeptr_ra) == 0)
          {
            am_openmp_task_type* new_type =
              (am_openmp_task_type*) malloc(sizeof(am_openmp_task_type));

            char* type_str = (char*) malloc(32);
            sprintf(type_str, "%lu", codeptr_ra);

            new_type->name = type_str;
            new_type->source = {NULL, 0, 0};

            task_type_map.emplace(codeptr_ra, new_type);

            am_openmp_task_type_array_appendp(task_types, new_type);
          }

          // And now create a new instance
          uint64_t task_id = events[k].new_task_id;

          am_openmp_task_instance* new_instance =
              (am_openmp_task_instance*)malloc(sizeof(am_openmp_task_instance));

          am_openmp_task_type* tt = task_type_map.at(codeptr_ra);

          new_instance->task_type = tt;

          task_instance_map.emplace(task_id, new_instance);

          am_openmp_task_instance_array_appendp(task_instances, new_instance);
        }
      }

      // Loops
      if(strcmp("am::openmp::loop", ace.type) == 0)
      {
        am_typed_array_generic* array = ace.array;

        am_openmp_loop* events =
          (am_openmp_loop*) array->elements;

        // Per each event
        for(unsigned k = 0; k < array->num_elements; k++)
        {
          uint64_t codeptr_ra = events[k].codeptr_ra;

          // Unknown type so add it
          if(for_loop_type_map.count(codeptr_ra) == 0)
          {
            am_openmp_for_loop_type* new_type =
              (am_openmp_for_loop_type*) malloc(sizeof(am_openmp_for_loop_type));

            char* type_str = (char*) malloc(32);
            sprintf(type_str, "%lu", codeptr_ra);

            new_type->source = {type_str, 0, 0};

            for_loop_type_map.emplace(codeptr_ra, new_type);

            am_openmp_for_loop_type_array_appendp(for_loop_types, new_type);
          }

          // And now create a new instance
          uint64_t loop_id = events[k].instance_id;

          am_openmp_for_loop_instance* new_instance =
              (am_openmp_for_loop_instance*)malloc(sizeof(am_openmp_for_loop_instance));

          am_openmp_for_loop_type* lt = for_loop_type_map.at(codeptr_ra);

          new_instance->loop_type = lt;
          new_instance->lower_bound = events[k].lower_bound;
          new_instance->upper_bound = events[k].upper_bound;
          new_instance->num_workers = events[k].num_workers;

          for_loop_instance_map.emplace(loop_id, new_instance);

          am_openmp_for_loop_instance_array_appendp(for_loop_instances, new_instance);
        }
      }
    }
  }

	std::cout << "There were " << task_types->num_elements << " task types." << std::endl;

  am_array_collection* global_ac = &trace->trace_arrays;

  if(task_types->num_elements > 0)
  {
    am_array_collection_add(global_ac, (am_typed_array_generic*) task_types,
      "am::opemp::task_type");

    am_array_collection_add(global_ac, (am_typed_array_generic*) task_instances,
      "am::opemp::task_instance");
  }

  if(for_loop_types->num_elements > 0)
  {
    am_array_collection_add(global_ac, (am_typed_array_generic*) for_loop_types,
      "am::opemp::for_loop_type");

    am_array_collection_add(global_ac, (am_typed_array_generic*) for_loop_instances,
      "am::opemp::for_loop_instance");
  }
  
  // Then we lift per event collection types
  // For each event collection
  for(unsigned i = 0; i < ecs.num_elements; i++)
  {
    am_array_collection ac = ecs.elements[i].event_arrays;

    // Per event collection arrays
    am_openmp_task_period_array* task_periods =
      (am_openmp_task_period_array*)malloc(sizeof(am_openmp_task_period_array));
    am_openmp_task_period_array_init(task_periods);

    am_openmp_iteration_period_array* iteration_periods =
      (am_openmp_iteration_period_array*)malloc(sizeof(am_openmp_iteration_period_array));
    am_openmp_iteration_period_array_init(iteration_periods);
  
    // For each event type
    for(unsigned j = 0; j < ac.num_elements; j++)
    {
      am_array_collection_entry ace = ac.elements[j];

      // Tasks
      if(strcmp("am::openmp::task_schedule", ace.type) == 0)
      {
        am_typed_array_generic* array = ace.array;

        am_openmp_task_schedule* events =
          (am_openmp_task_schedule*) array->elements;

        am_timestamp_t prior_timestamp = events[0].timestamp;

        // For each event
        for(unsigned k = 1; k < array->num_elements; k++)
        {
          am_timestamp_t new_timestamp = events[k].timestamp;

          uint64_t task_id = events[k].prior_task_id;

          // Only add period if there was a task running
          if(task_id != 0)
          {
            // TODO: Temporary hack, we need to handle case when the
            // task is tied to loop (see BOTS)
            am_openmp_task_instance* ti = NULL;
            if(task_instance_map.count(task_id) != 0)
              ti = task_instance_map.at(task_id);

            am_openmp_task_period* new_period =
              (am_openmp_task_period*) malloc(sizeof(am_openmp_task_period));

            new_period->task_instance = ti;
            new_period->interval = {prior_timestamp, new_timestamp};

            am_openmp_task_period_array_appendp(task_periods, new_period);
          }

          prior_timestamp = new_timestamp;
        }
      }

      // Loops
      if(strcmp("am::openmp::loop_chunk", ace.type) == 0)
      {
        am_typed_array_generic* array = ace.array;

        am_openmp_loop_chunk* events =
          (am_openmp_loop_chunk*) array->elements;

        uint64_t inst_id = 0;
        int64_t lb = 0;
        int64_t ub = 0;
        bool last = 0;
        am_timestamp_t timestamp = 0;

        std::stack<std::pair<uint64_t, std::pair<int64_t, int64_t>>> bounds;

        // For each event
        for(unsigned k = 0; k < array->num_elements; k++)
        {
          // Try to close current period
          if(timestamp != 0 && bounds.size() > 0)
          {
            // If last we pop nest level from stack
            if(events[k].is_last)
            {
              auto bound = bounds.top();
              bounds.pop();

              // If previous was also last we need to recover bounds
              // of the current level
              if(last)
              {
                inst_id = bound.first;
                lb = bound.second.first;
                ub = bound.second.second;
              }
            }
            // If the previous node was last, but current is no last, we
            // have consecutive iterations on the same nest level,
            // use iteration from level above for this period. 
            else if(last)
            {
              // We don't leave nest so don't pop
              auto bound = bounds.top();
              inst_id = bound.first;
              lb = bound.second.first;
              ub = bound.second.second;
            }

            // Ignore cases when lb > ub, as loop did no useful for
            // (just was initialized).
            if(lb <= ub)
            {
              am_openmp_iteration_set* is = iter_set_map.at({inst_id, {lb, ub}});

              am_openmp_iteration_period* new_period =
                (am_openmp_iteration_period*) malloc(sizeof(am_openmp_iteration_period));

              new_period->iteration_set = is;
              new_period->interval = {timestamp, events[k].timestamp};

              am_openmp_iteration_period_array_appendp(iteration_periods, new_period);
            }
          }

          // New instance so nest level increases
          if(!events[k].is_last && inst_id != events[k].instance_id)
          {
            bounds.push({events[k].instance_id, {events[k].lower_bound, events[k].upper_bound}});
          }

          inst_id = events[k].instance_id;
          lb = events[k].lower_bound;
          ub = events[k].upper_bound;
          last = events[k].is_last;
          timestamp = events[k].timestamp;

          // Iteration set not created yet and it is not the marker
          // event.
          if(!last && lb <= ub && iter_set_map.count({inst_id, {lb, ub}}) == 0)
          {
             am_openmp_iteration_set* new_set =
              (am_openmp_iteration_set*) malloc(sizeof(am_openmp_iteration_set));

            new_set->loop_instance = for_loop_instance_map.at(inst_id);
            new_set->lower_bound = lb;
            new_set->upper_bound = ub;

            iter_set_map.emplace(std::make_pair(inst_id, std::make_pair(lb, ub)), new_set);

            am_openmp_iteration_set_array_appendp(iteration_sets, new_set);
          }
        }
      }

    }

    if(task_periods->num_elements > 0)
    {
      am_array_collection_add(&ac, (am_typed_array_generic*) task_periods,
        "am::openmp::task_period");
    }

    if(iteration_periods->num_elements > 0)
    {
      am_array_collection_add(&ac, (am_typed_array_generic*) iteration_periods,
        "am::openmp::iteration_period");
    }

  }
	
  if(iteration_sets->num_elements > 0)
  {
    am_array_collection_add(global_ac, (am_typed_array_generic*) iteration_sets,
      "am::openmp::iteration_set");
  }

  // Some debug printing
/*  for(unsigned i = 0; i < ecs.num_elements; i++)
  {
    am_array_collection ac = ecs.elements[i].event_arrays;

    for(unsigned j = 0; j < ac.num_elements; j++)
    {
      am_array_collection_entry ace = ac.elements[j];

      if(strcmp(ace.type, "am::openmp::iteration_period") == 0)
      {
        am_typed_array_generic* array = ace.array;

        am_openmp_iteration_period* ip = (am_openmp_iteration_period*) array->elements;

        for(unsigned k = 0; k < array->num_elements; k++)
        {
          printf("%lu\n", ip[k].iteration_set->lower_bound);
        }
      }
    }
  }*/

	buildCallGraph(trace, symbols_by_addr);

}

AftermathSession::AftermathSession()
//	trace(NULL)
{
	this->dfg.graph = NULL;
	this->dfg.coordinate_mapping = NULL;

	this->trace[0] = NULL;
	this->trace[1] = NULL;

	am_dfg_type_registry_init(&this->dfg.type_registry,
				  AM_DFG_TYPE_REGISTRY_DESTROY_TYPES);
	am_dfg_node_type_registry_init(&this->dfg.node_type_registry,
				       AM_DFG_NODE_TYPE_REGISTRY_DESTROY_TYPES);

	am_timeline_render_layer_type_registry_init(&this->rltr);
	am_register_common_timeline_layer_types(&this->rltr);

	if(am_dfg_builtin_types_register(&this->dfg.type_registry)) {
		this->cleanup();
		throw RegisterDFGTypesException();
	}

	if(am_render_dfg_builtin_types_register(&this->dfg.type_registry)) {
		this->cleanup();
		throw RegisterDFGTypesException();
	}

	if(amgui_dfg_builtin_types_register(&this->dfg.type_registry)) {
		this->cleanup();
		throw RegisterDFGTypesException();
	}

	if(am_dfg_builtin_node_types_register(&this->dfg.node_type_registry,
					      &this->dfg.type_registry))
	{
		this->cleanup();
		throw RegisterDFGNodeTypesException();
	}

	if(am_render_dfg_builtin_node_types_register(
		   &this->dfg.node_type_registry,
		   &this->dfg.type_registry))
	{
		this->cleanup();
		throw RegisterDFGNodeTypesException();
	}

	if(amgui_dfg_builtin_node_types_register(&this->dfg.node_type_registry,
						 &this->dfg.type_registry))
	{
		this->cleanup();
		throw RegisterDFGNodeTypesException();
	}
}

void AftermathSession::cleanup()
{
	am_timeline_render_layer_type_registry_destroy(&this->rltr);

	if(this->trace[0]) {
		am_trace_destroy(this->trace[0]);
		free(this->trace[0]);
	}

	if(this->trace[1]) {
		am_trace_destroy(this->trace[1]);
		free(this->trace[1]);
	}

	if(this->dfg.graph) {
		am_dfg_graph_destroy(this->dfg.graph);
		free(this->dfg.graph);
	}

	if(this->dfg.coordinate_mapping) {
		am_dfg_coordinate_mapping_destroy(this->dfg.coordinate_mapping);
		free(this->dfg.coordinate_mapping);
	}

	am_dfg_node_type_registry_set_instantiate_callback_fun(
		&this->dfg.node_type_registry, NULL, NULL);

	am_dfg_node_type_registry_destroy(&this->dfg.node_type_registry);
	am_dfg_type_registry_destroy(&this->dfg.type_registry);
}

AftermathSession::~AftermathSession()
{
	this->cleanup();
}

struct am_dfg_type_registry* AftermathSession::getDFGTypeRegistry() noexcept
{
	return &this->dfg.type_registry;
}

struct am_dfg_node_type_registry* AftermathSession::getDFGNodeTypeRegistry()
	noexcept
{
	return &this->dfg.node_type_registry;
}

struct am_trace* AftermathSession::getTrace(unsigned id) noexcept
{
	return this->trace[id];
}

struct am_timeline_render_layer_type_registry*
AftermathSession::getRenderLayerTypeRegistry() noexcept
{
	return &this->rltr;
}

void AftermathSession::setTrace(struct am_trace* t, unsigned id) noexcept
{
	this->trace[id] = t;
}

struct am_dfg_graph* AftermathSession::getDFG() noexcept
{
	return this->dfg.graph;
}

void AftermathSession::setDFG(struct am_dfg_graph* g) noexcept
{
	if(this->dfg.graph) {
		am_dfg_graph_destroy(this->dfg.graph);
		free(this->dfg.graph);
	}

	this->dfg.graph = g;
	this->dfgProcessor.setDFG(g);
}

struct am_dfg_coordinate_mapping* AftermathSession::getDFGCoordinateMapping()
	noexcept
{
	return this->dfg.coordinate_mapping;
}

void AftermathSession::setDFGCoordinateMapping(
	struct am_dfg_coordinate_mapping* m) noexcept
{
	this->dfg.coordinate_mapping = m;
}

AftermathGUI& AftermathSession::getGUI()
{
	return this->gui;
}

DFGQTProcessor& AftermathSession::getDFGProcessor()
{
	return this->dfgProcessor;
}

DFGQTProcessor* AftermathSession::getDFGProcessorp()
{
	return &this->dfgProcessor;
}

/* Schedules the entire DFG graph of the session. Throws an exception if
 * scheduling fails. */
void AftermathSession::scheduleDFG()
{
	int ret;

	if(!this->dfg.graph)
		throw NoDFGException();

	this->dfgProcessor.disable();

	ret = am_dfg_schedule_graph(this->dfg.graph);

	this->dfgProcessor.enable();

	if(ret)
		throw DFGSchedulingException();
}

/* For each error message in the error stack s, a line with the message is
 * appended to msg */
static void errorStackToString(struct am_io_error_stack* s, std::string& msg)
{
	for(size_t i = 0; i < s->pos; i++) {
		msg += s->errors[i].msgbuf;
		msg += "\n";
	}
}

// TODO these should perhaps be in a separate utility header
std::vector<std::string> split(const std::string& s, char delim, std::vector<std::string>& elems){

	std::stringstream ss(s);
	std::string item;

	while (std::getline(ss, item, delim))
		elems.push_back(item);

	return elems;
}

std::vector<std::string> split_string_to_vector(const std::string& s, char delim){
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::map<uint64_t, std::string> AftermathSession::parseBinarySymbols(const char* binary_filename)
{

	std::map<uint64_t, std::string> symbols_by_addr;

	std::stringstream ss;
	ss << R"(sh -c ")";
	ss << R"(nm -l -td )" << binary_filename;
	ss << R"( | grep '[0-9A-Fa-f]* [tT]')";
	ss << R"( | sed -e 's/:/ /g' -e 's/[\t]/ /g' -e 's/[ ]\{1,\}/ /g')";
	ss << R"(" 2> /dev/null)";

	std::string command = ss.str();
	
	char buffer[256];
	std::string result = "";

	FILE* fp = popen(command.c_str(), "r");
	if(fp == nullptr)
		throw AftermathException("Could not parse binary symbols via: " + command);

	while(!feof(fp)){
		if(fgets(buffer, 256, fp) != nullptr)
			result += buffer;
	}

	// Parse the result into lines then parse each line
	std::vector<std::string> lines = split_string_to_vector(result, '\n');
	for(auto line : lines){
		std::vector<std::string> words = split_string_to_vector(line, ' ');
		if(words.size() != 3 || !(words.at(1) == "T" || words.at(1) == "t") || words.at(0) == "")
			continue; // just ignore any symbols we can't parse
			//throw AftermathException("Failed to parse binary symbol results via: " + command);
		
		// first word is address
		// second word is the symbol type
		// third word is the symbol name
		size_t char_idx = 0;
		uint64_t addr = std::stoull(words.at(0), &char_idx, 10);
	
		symbols_by_addr.insert(std::make_pair(addr, words.at(2)));
		
	}

	pclose(fp);

	return symbols_by_addr;

}

/* Reads the trace file whose filename including its path is given from disk and
 * sets it as the trace for this Aftermath session.
 *
 * Throws an exception on error.
 */
void AftermathSession::loadTrace(
	const char* filename,
	unsigned id,
	const char* binary_filename)
{
	struct am_trace* trace;
	struct am_io_context ioctx;
	struct am_frame_type_registry frame_types;

	if(am_frame_type_registry_init(&frame_types, AM_MAX_FRAME_TYPES)) {
		throw AftermathException("Could not initialize frame type "
					 "registry");
	}

	if(am_dsk_register_frame_types(&frame_types)) {
		am_frame_type_registry_destroy(&frame_types);
		throw AftermathException("Could not register builtin frame types");
	}

	if(am_io_context_init(&ioctx, &frame_types)) {
		am_frame_type_registry_destroy(&frame_types);
		throw AftermathException("Could not initialize I/O context");
	}

	try {
		if(am_io_context_open(&ioctx, filename, AM_IO_READ)) {
			throw AftermathException("Could not open trace file "
						 "for reading");
		}

		if(am_dsk_load_trace(&ioctx, &trace)) {
			std::string msg;

			errorStackToString(&ioctx.error_stack, msg);
			throw AftermathException(msg);
		}
	} catch(...) {
		am_io_context_destroy(&ioctx);
		am_frame_type_registry_destroy(&frame_types);
		throw;
	}

	std::map<uint64_t, std::string> symbol_table;
	if(strcmp(binary_filename, "") != 0) {
		try {

			symbol_table = parseBinarySymbols(binary_filename);

		} catch(...) {
			// We couldn't get symbols from the provided binary
			// TODO perhaps just continue?
			throw;
		}
	}

	am_io_context_destroy(&ioctx);
	am_frame_type_registry_destroy(&frame_types);

  /* NO-body expects the Spanish Inquisition! */
  /* TODO: Temporary hack to create Aftermath types from
     raw OMPT data */
  processTrace(trace, symbol_table);

	this->setTrace(trace, id);
}

/* Loads a DFG graph from the specified location. */
void AftermathSession::loadDFG(const char* filename)
{
	struct am_dfg_graph* g;
	struct am_dfg_coordinate_mapping* m;
	struct am_dfg_type_registry* tr;
	struct am_dfg_node_type_registry* ntr;
	struct am_object_notation_node* n_graph;
	struct am_parse_status ps;

	ntr = this->getDFGNodeTypeRegistry();
	tr = this->getDFGTypeRegistry();

	am_dfg_node_type_registry_set_instantiate_callback_fun(
		ntr, AftermathSession::DFGNodeInstantiationCallback, this);

	if(!(n_graph = am_object_notation_load_with_status(filename, &ps))) {
		std::stringstream errss;

		if(ps.result == AM_PARSE_RESULT_ERROR) {
			/* Actual parsing error */
			errss << "Could not load object notation for DFG:"
			      << std::endl
			      << ps.source_name << ":"
			      << ps.line << ":" << ps.character
			      << ": error: " << ps.errmsg << std::endl;
		} else {
			/* Something else went wrong, e.g., memory allocation,
			 * I/O, etc. */
			errss << "Could not load object notation for DFG";
		}

		throw AftermathException(errss.str());
	}

	if(!(g = ((typeof(g))malloc(sizeof(*g))))) {
		am_object_notation_node_destroy(n_graph);
		free(n_graph);

		throw AftermathException("Could not allocate memory for DFG");
	}

	am_dfg_graph_init(g, AM_DFG_GRAPH_DESTROY_ALL);

	if(!(m = ((typeof(m))malloc(sizeof(*m))))) {
		throw AftermathException("Could not allocate memory for DFG "
					 "coordinate mapping");
	}

	am_dfg_coordinate_mapping_init(m);

	try {
		if(am_dfg_graph_from_object_notation(g, n_graph, tr, ntr))
			throw AftermathException("Could not load DFG from "
						 "object notation");

		struct am_object_notation_node_list* lst =
			(typeof(lst))am_object_notation_eval(n_graph, "positions");

		if(am_dfg_coordinate_mapping_from_object_notation(m, lst))
			throw AftermathException("Could not load DFG coordinate "
						 "mapping from object notation");
	} catch(...) {
		am_object_notation_node_destroy(n_graph);
		free(n_graph);
		am_dfg_graph_destroy(g);
		free(g);
		am_dfg_node_type_registry_set_instantiate_callback_fun(
			ntr, NULL, NULL);

		throw;
	}

	this->setDFG(g);
	this->setDFGCoordinateMapping(m);

	am_object_notation_node_destroy(n_graph);
	free(n_graph);

	am_dfg_node_type_registry_set_instantiate_callback_fun(
		ntr, NULL, NULL);
}

/* Gets called on every instantiation of a DFG node. Performs lookup of the
 * widget for DFG nodes that are associated with a widget.
 */
int AftermathSession::DFGNodeInstantiationCallback(
	struct am_dfg_node_type_registry* reg,
	struct am_dfg_node* n,
	void* data)
{
	AftermathSession* session = (AftermathSession*)data;
	QWidget* w;

	if(strcmp(n->type->name, "am::gui::label") == 0) {
		struct am_dfg_amgui_label_node* l = (typeof(l))n;

		try {
			w = session->getGUI().getWidget(l->label_id);

			if(!(l->label = dynamic_cast<LabelWithDFGNode*>(w)))
				return 1;

			l->label->setDFGNode(n);
		} catch(...) {
			return 1;
		}
	} else if(strcmp(n->type->name, "am::gui::hierarchy_combobox") == 0) {
		struct am_dfg_amgui_hierarchy_combobox_node* hcb = (typeof(hcb))n;

		try {
			w = session->getGUI().getWidget(hcb->widget_id);

			if(!(hcb->widget = dynamic_cast<HierarchyComboBox*>(w)))
				return 1;

			hcb->widget->setDFGNode(n);
		} catch(...) {
			return 1;
		}
	} else if(strcmp(n->type->name, "am::gui::telamon::candidate_tree") == 0) {
		struct am_dfg_amgui_telamon_candidate_tree_node* t = (typeof(t))n;

		try {
			w = session->getGUI().getWidget(t->tree_id);

			if(!(t->tree = dynamic_cast<TelamonCandidateTreeWidget*>(w)))
				return 1;

			t->tree->setDFGNode(n);
		} catch(...) {
			return 1;
		}
	} else if(strcmp(n->type->name, "am::gui::timeline") == 0) {
		struct am_dfg_amgui_timeline_node* t = (typeof(t))n;

		try {
			w = session->getGUI().getWidget(t->timeline_id);

			if(!(t->timeline = dynamic_cast<TimelineWidget*>(w)))
				return 1;

			t->timeline->setDFGNode(n);
		} catch(...) {
			return 1;
		}
	} else if(strcmp(n->type->name, "am::gui::toolbar_togglebutton") == 0) {
		struct am_dfg_amgui_toolbar_togglebutton_node* t = (typeof(t))n;

		try {
			w = session->getGUI().getWidget(t->widget_id);

			if(!(t->togglebutton = dynamic_cast<ToolbarToggleButton*>(w)))
				return 1;

			t->togglebutton->setDFGNode(n);
		} catch(...) {
			return 1;
		}
	} else if(strcmp(n->type->name, "am::gui::toolbar_button") == 0) {
		struct am_dfg_amgui_toolbar_button_node* t = (typeof(t))n;

		try {
			w = session->getGUI().getWidget(t->widget_id);

			if(!(t->button = dynamic_cast<ToolbarButton*>(w)))
				return 1;

			t->button->setDFGNode(n);
		} catch(...) {
			return 1;
		}
	} else if(strcmp(n->type->name, "am::gui::histogram") == 0) {
		struct am_dfg_amgui_histogram_node* h = (typeof(h))n;

		try {
			w = session->getGUI().getWidget(h->histogram_id);

			if(!(h->histogram_widget = dynamic_cast<HistogramWidget*>(w)))
				return 1;

			h->histogram_widget->setDFGNode(n);
		} catch(...) {
			return 1;
		}
	} else if(strcmp(n->type->name, "am::core::trace") == 0) {
		struct am_dfg_node_trace* t = (typeof(t))n;

		t->trace = session->getTrace(0);
    t->aux_trace = session->getTrace(1);
	}

	return 0;
}
