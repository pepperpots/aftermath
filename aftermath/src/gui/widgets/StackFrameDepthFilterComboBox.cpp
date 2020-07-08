/**
 * Author: Richard Neill <richard.neill@manchester.ac.uk>
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

#include <sstream>
#include "StackFrameDepthFilterComboBox.h"
#include "../../dfg/nodes/gui/stack_frame_depth_filter_combobox.h"

StackFrameDepthFilterComboBox::StackFrameDepthFilterComboBox(QWidget* parent) :
	QComboBox(parent)
{
	this->setEditable(false);
	this->init = false;
	this->max_depth = 0;
	this->modifyDepthOptions(this->max_depth);

	QObject::connect(this, static_cast<void(QComboBox::*)(int)>
			 (&QComboBox::currentIndexChanged),
			 this, [=](int idx){
				 if(this->dfgNode) {
					 this->dfgNode->required_mask.push_new =
						 (1 << AM_DFG_AMGUI_STACK_FRAME_DEPTH_FILTER_COMBOBOX_NODE_FILTERED_TRACE);
					 this->processDFGNode();
				 }
			 });

}

StackFrameDepthFilterComboBox::~StackFrameDepthFilterComboBox()
{
}

size_t StackFrameDepthFilterComboBox::getSelectedStackFrameDepth()
{
	int idx = this->currentIndex();

	if((size_t) idx > max_depth){
		// This shouldn't be possible
		return 0;
	}

	return idx;
}

bool StackFrameDepthFilterComboBox::isInit()
{
	return this->init;
}

void StackFrameDepthFilterComboBox::setInit(bool init)
{
	this->init = init;
}

void StackFrameDepthFilterComboBox::clearDepthOptions(){
	this->clear();
	this->init = false;
}

void StackFrameDepthFilterComboBox::modifyDepthOptions(size_t updated_max_depth)
{
	if(updated_max_depth < this->max_depth)
		return;

	this->max_depth = updated_max_depth;	
	this->clearDepthOptions();

	std::string all("All");
	this->addItem(all.c_str());

	for(size_t i = 1; i <= updated_max_depth; i++){
		std::stringstream ss;
		ss << i;
		this->addItem(ss.str().c_str());
	}
}
