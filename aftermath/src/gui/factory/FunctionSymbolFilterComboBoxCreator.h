/**
 * Author: Richard Neill <richard.neill@manchester.ac.uk>
 *
 / This program is free software; you can redistribute it and/or modify it under
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

#ifndef AM_FUNCTION_SYMBOL_FILTER_COMBOBOXCREATOR_H
#define AM_FUNCTION_SYMBOL_FILTER_COMBOBOXCREATOR_H

#include "GUIFactory.h"
#include "../widgets/FunctionSymbolFilterComboBox.h"

/* Widget creator creating function symbol filter combobox widgets.
 * The expected node format is:
 *
 *   amgui_function_symbol_filter_combobox { }
 *
 */
class FunctionSymbolFilterComboBoxCreator : public NonContainerWidgetCreator {
	public:
		FunctionSymbolFilterComboBoxCreator();

		QWidget* instantiateDefault();

		QWidget*
		instantiate(const struct am_object_notation_node_group* n);

		const std::string getDFGNodeTypeName();
		void associateDFGNode(QWidget* w, struct am_dfg_node* n);
};

#endif
