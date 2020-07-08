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

#include "FunctionSymbolFilterComboBoxCreator.h"
#include "../widgets/ManagedWidget.h"
#include "../../dfg/nodes/gui/function_symbol_filter_combobox.h"

/* Helper class for traversal of Aftermath GUI */
AM_ALIAS_WIDGET(ManagedFunctionSymbolFilterComboBox,
		FunctionSymbolFilterComboBox,
		"amgui_function_symbol_filter_combobox")

FunctionSymbolFilterComboBoxCreator::FunctionSymbolFilterComboBoxCreator() :
	NonContainerWidgetCreator("amgui_function_symbol_filter_combobox")
{
}

QWidget* FunctionSymbolFilterComboBoxCreator::instantiateDefault()
{
	return new ManagedFunctionSymbolFilterComboBox();
}

QWidget* FunctionSymbolFilterComboBoxCreator::instantiate(
	const struct am_object_notation_node_group* n)
{
	return this->instantiateDefault();
}

const std::string FunctionSymbolFilterComboBoxCreator::getDFGNodeTypeName()
{
	return "am::gui::function_symbol_filter_combobox";
}

void FunctionSymbolFilterComboBoxCreator::associateDFGNode(QWidget* w,
						struct am_dfg_node* n)
{
	struct am_dfg_amgui_function_symbol_filter_combobox_node* cbn;
	FunctionSymbolFilterComboBox* c;

	c = static_cast<FunctionSymbolFilterComboBox*>(w);
	cbn = reinterpret_cast<struct am_dfg_amgui_function_symbol_filter_combobox_node*>(n);

	c->setDFGNode(n);
	cbn->widget = c;
}
