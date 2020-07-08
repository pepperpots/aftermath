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

#ifndef AM_STACK_FRAME_DEPTH_FILTER_COMBOBOX_H
#define AM_STACK_FRAME_DEPTH_FILTER_COMBOBOX_H

#include <QComboBox>
#include "WidgetWithDFGNode.h"

extern "C" {
	#include <aftermath/core/stack_frame_array.h>
}

/* A non-editable combo box widget that lets the user select a depth
 * from a list of possible stack frame depths.
 */
class StackFrameDepthFilterComboBox : public QComboBox {
	AM_WIDGETWITHDFGNODE_DECLS
	Q_OBJECT

	public:
		StackFrameDepthFilterComboBox(QWidget* parent = NULL);
		virtual ~StackFrameDepthFilterComboBox();

		bool isInit();
		void setInit(bool init);

		void clearDepthOptions();

		/* This will update the options to ensure inclusion of depths up to
		 * max_depth
		 */
		void modifyDepthOptions(size_t max_depth);

		size_t getSelectedStackFrameDepth();

	protected:
		size_t max_depth;
		int init;
};

#endif
