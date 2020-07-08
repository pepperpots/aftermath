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

#ifndef AM_FUNCTION_SYMBOL_FILTER_COMBOBOX_H
#define AM_FUNCTION_SYMBOL_FILTER_COMBOBOX_H

#include <QComboBox>
#include "WidgetWithDFGNode.h"

extern "C" {
	#include <aftermath/core/function_symbol_array.h>
}

/* A non-editable combo box widget that lets the user select a particular
 * function symbol from a list of possible symbols.
 */
class FunctionSymbolFilterComboBox : public QComboBox {
	AM_WIDGETWITHDFGNODE_DECLS
	Q_OBJECT

	public:
		FunctionSymbolFilterComboBox(QWidget* parent = NULL);
		virtual ~FunctionSymbolFilterComboBox();

		bool isInit();
		void setInit(bool init);

		void clearSymbolOptions();
		void updateSymbolOptions(std::string symbol_name);

		size_t getSelectedFunctionSymbol();

	protected:
		std::vector<std::string> symbols;
		int init;
};

#endif
