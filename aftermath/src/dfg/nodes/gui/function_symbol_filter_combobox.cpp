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
#include "function_symbol_filter_combobox.h"
#include "../../../gui/widgets/FunctionSymbolFilterComboBox.h"

extern "C" {
	#include <aftermath/core/trace.h>
	#include <aftermath/core/function_symbol_array.h>
}

int am_dfg_amgui_function_symbol_filter_combobox_init(struct am_dfg_node* n)
{
	struct am_dfg_amgui_function_symbol_filter_combobox_node* cb = (typeof(cb))n;

	cb->widget = NULL;
	cb->widget_id = NULL;

	return 0;
}

void am_dfg_amgui_function_symbol_filter_combobox_destroy(struct am_dfg_node* n)
{
	struct am_dfg_amgui_function_symbol_filter_combobox_node* cb = (typeof(cb))n;

	free(cb->widget_id);
}

void am_dfg_amgui_function_symbol_filter_combobox_disconnect(struct am_dfg_node* n,
						struct am_dfg_port* pi)
{
	struct am_dfg_amgui_function_symbol_filter_combobox_node* cb = (typeof(cb))n;

	if(pi == &n->ports[0])
		if(cb->widget){
			cb->widget->clearSymbolOptions();
		}
}

int am_dfg_amgui_function_symbol_filter_combobox_process(struct am_dfg_node* n)
{

	struct am_dfg_amgui_function_symbol_filter_combobox_node* cb = (typeof(cb))n;
	struct am_dfg_port* ptrace = &n->ports[AM_DFG_AMGUI_FUNCTION_SYMBOL_FILTER_COMBOBOX_NODE_TRACE];
	struct am_dfg_port* pfiltered_trace =
		&n->ports[AM_DFG_AMGUI_FUNCTION_SYMBOL_FILTER_COMBOBOX_NODE_FILTERED_TRACE];

	std::string selected_symbol = "";
	struct am_trace* trace = NULL;
	
	std::cout << "Processing function symbol combobox" << std::endl;

	if(am_dfg_buffer_read_last(ptrace->buffer, (void*)&trace)){
		std::cout << "Couldnt read trace" << std::endl;
		return 1;
	}

	if(cb->widget && cb->widget->isInit() == false) {
		std::cout << "Was not init" << std::endl;
		if(trace){
			std::cout << "Updating combobox symbol options" << std::endl;

			struct am_function_symbol_array* fsa = 
				(am_function_symbol_array*) am_trace_find_trace_array(
					trace,
					"am::core::function_symbol"
				);


			if(fsa == NULL){
				if(am_dfg_buffer_write(pfiltered_trace->buffer, 1, &trace))
					return 1;
				else
					return 0;
			}

			struct am_function_symbol* symbols =
				(struct am_function_symbol*) fsa->elements;

			for(unsigned k = 0; k < fsa->num_elements; k++)
			{
				struct am_function_symbol* symbol = &symbols[k];
				std::string str(symbol->name);
				cb->widget->updateSymbolOptions(str);
			}

		}

		cb->widget->setInit(true);
	}
	
	std::cout << "Processing the combobox" << std::endl;

	if(am_dfg_port_activated(pfiltered_trace) && cb->widget) {
		std::cout << "Output port is activated." << std::endl;

		selected_symbol = cb->widget->getSelectedFunctionSymbol();

		struct am_function_symbol_array* fsa = 
			(am_function_symbol_array*) am_trace_find_trace_array(
				trace,
				"am::core::function_symbol"
			);

		if(fsa == NULL){
			if(am_dfg_buffer_write(pfiltered_trace->buffer, 1, &trace))
				return 1;
			else
				return 0;
		}

		struct am_function_symbol* symbols =
			(struct am_function_symbol*) fsa->elements;

		for(unsigned k = 0; k < fsa->num_elements; k++)
		{
			struct am_function_symbol* symbol = &symbols[k];

			if(strcmp(symbol->name, selected_symbol.c_str()) == 0)
				symbol->post_filter_include = 1;
			else
				symbol->post_filter_include = 0;

		}

	} else {
		std::cout << "Output port is not active." << std::endl;
	}

	if(am_dfg_buffer_write(pfiltered_trace->buffer, 1, &trace))
		return 1;

	return 0;

}

int am_dfg_amgui_function_symbol_filter_combobox_from_object_notation(
	struct am_dfg_node* n,
	struct am_object_notation_node_group* g)
{
	struct am_dfg_amgui_function_symbol_filter_combobox_node* cb = (typeof(cb))n;
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

int am_dfg_amgui_function_symbol_filter_combobox_to_object_notation(
	struct am_dfg_node* n,
	struct am_object_notation_node_group* g)
{
	struct am_dfg_amgui_function_symbol_filter_combobox_node* cb = (typeof(cb))n;

	return am_object_notation_node_group_build_add_members(
		g,
		AM_OBJECT_NOTATION_BUILD_MEMBER, "widget_id",
		  AM_OBJECT_NOTATION_BUILD_STRING, cb->widget_id);
}
