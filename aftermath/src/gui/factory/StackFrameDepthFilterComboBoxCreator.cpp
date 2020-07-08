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

#include "StackFrameDepthFilterComboBoxCreator.h"
#include "../widgets/ManagedWidget.h"
#include "../../dfg/nodes/gui/stack_frame_depth_filter_combobox.h"

/* Helper class for traversal of Aftermath GUI */
AM_ALIAS_WIDGET(ManagedStackFrameDepthFilterComboBox,
		StackFrameDepthFilterComboBox,
		"amgui_stack_frame_depth_filter_combobox")

StackFrameDepthFilterComboBoxCreator::StackFrameDepthFilterComboBoxCreator() :
	NonContainerWidgetCreator("amgui_stack_frame_depth_filter_combobox")
{
}

QWidget* StackFrameDepthFilterComboBoxCreator::instantiateDefault()
{
	return new ManagedStackFrameDepthFilterComboBox();
}

QWidget* StackFrameDepthFilterComboBoxCreator::instantiate(
	const struct am_object_notation_node_group* n)
{
	return this->instantiateDefault();
}

const std::string StackFrameDepthFilterComboBoxCreator::getDFGNodeTypeName()
{
	return "am::gui::stack_frame_depth_filter_combobox";
}

void StackFrameDepthFilterComboBoxCreator::associateDFGNode(QWidget* w,
						struct am_dfg_node* n)
{
	struct am_dfg_amgui_stack_frame_depth_filter_combobox_node* cbn;
	StackFrameDepthFilterComboBox* c;

	c = static_cast<StackFrameDepthFilterComboBox*>(w);
	cbn = reinterpret_cast<struct am_dfg_amgui_stack_frame_depth_filter_combobox_node*>(n);

	c->setDFGNode(n);
	cbn->widget = c;
}
