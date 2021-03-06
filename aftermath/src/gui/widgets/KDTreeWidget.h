/**
 * Author: Andi Drebes <andi@drebesium.org>
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

#ifndef AM_KDTREEWIDGET_H
#define AM_KDTREEWIDGET_H

#include "CairoWidget.h"
#include "../../Exception.h"

extern "C" {
	#include <aftermath/render/kdtree/renderer.h>
}

/**
 * Widget encapsulating a renderer for k-d-trees
 */
class KDTreeWidget : public CairoWidget {
	Q_OBJECT

	public:
		typedef CairoWidget super;

		KDTreeWidget(QWidget* parent = NULL);
		virtual ~KDTreeWidget();
		virtual void resizeEvent(QResizeEvent *event);

		void setKDTree(struct am_kdtree* t);

		virtual void mouseMoveEvent(QMouseEvent* event);
		virtual void mousePressEvent(QMouseEvent* event);
		virtual void mouseReleaseEvent(QMouseEvent* event);
		virtual void wheelEvent(QWheelEvent* event);

	protected:
		/* Current mode */
		enum {
			MOUSE_MODE_NONE = 0,
			MOUSE_MODE_NAVIGATE
		} mouseMode;

		/* Position of the mouse in pixel coordinates at the beginning
		 * of a dragging operation */
		struct am_point mouseStartPos;

		/* Position of the mouse in graph coordinates at the beginning
		 * of a dragging operation */
		struct {
			double x;
			double y;
		} graphStartPos;

		struct am_point currMouseGraphPos;

		struct am_kdtree_renderer renderer;

		virtual void cairoPaintEvent(cairo_t* cr);

		/* Pure virtual function that carries out the actual painting of
		 * a node; Must be implemented by the subclass. */
		virtual void paintNode(cairo_t* cr,
				       struct am_point screen_pos,
				       double zoom,
				       const struct am_kdtree_node* n) = 0;

		static void paintNodeCallback(cairo_t* cr,
					      struct am_point screen_pos,
					      double zoom,
					      const struct am_kdtree_node* n,
					      void* data);

		void mouseMoveNavigate(const struct am_point* screen_pos);
};

#endif
