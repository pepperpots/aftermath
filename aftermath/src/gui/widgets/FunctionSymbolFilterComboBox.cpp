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
#include "FunctionSymbolFilterComboBox.h"
#include "../../dfg/nodes/gui/function_symbol_filter_combobox.h"

FunctionSymbolFilterComboBox::FunctionSymbolFilterComboBox(QWidget* parent) :
	QComboBox(parent)
{
	this->setEditable(false);
	this->init = false;

	std::string str("All");
	this->updateSymbolOptions(str);

	QObject::connect(this, static_cast<void(QComboBox::*)(int)>
			 (&QComboBox::currentIndexChanged),
			 this, [=](int idx){
				 if(this->dfgNode) {
					 this->dfgNode->required_mask.push_new =
						 (1 << AM_DFG_AMGUI_FUNCTION_SYMBOL_FILTER_COMBOBOX_NODE_FILTERED_TRACE);
					 this->processDFGNode();
				 }
			 });

}

FunctionSymbolFilterComboBox::~FunctionSymbolFilterComboBox()
{
}

size_t FunctionSymbolFilterComboBox::getSelectedFunctionSymbol()
{
	int idx = this->currentIndex();

	if((size_t) idx >= this->symbols.size()){
		// This shouldn't be possible
		return 0;
	}

	return idx;
}

bool FunctionSymbolFilterComboBox::isInit()
{
	return this->init;
}

void FunctionSymbolFilterComboBox::setInit(bool init)
{
	this->init = init;
}

void FunctionSymbolFilterComboBox::clearSymbolOptions(){
	this->clear();
	this->symbols.clear();
	this->init = false;
}

void FunctionSymbolFilterComboBox::updateSymbolOptions(std::string symbol_name)
{
	if(std::find(this->symbols.begin(),
			this->symbols.end(),
			symbol_name) != this->symbols.end())
		return;

	this->symbols.push_back(symbol_name);
	this->addItem(symbol_name.c_str());

}
