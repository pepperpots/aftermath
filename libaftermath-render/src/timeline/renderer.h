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

#ifndef AFTERMATH_RENDER_TIMELINE_RENDERER_H
#define AFTERMATH_RENDER_TIMELINE_RENDERER_H

#include <aftermath/core/typed_list.h>
#include <aftermath/core/hierarchy.h>
#include <aftermath/core/bitvector.h>
#include <aftermath/core/arithmetic.h>
#include <aftermath/core/timestamp.h>
#include <aftermath/render/cairo_extras.h>
#include <aftermath/render/timeline/layer.h>
#include <aftermath/core/trace.h>

enum am_timeline_renderer_lane_mode {
	/* Always use a separate lane for a node */
	AM_TIMELINE_RENDERER_LANE_MODE_ALWAYS_SEPARATE,

	/* Collapse a node's lane with the lane of its first child */
	AM_TIMELINE_RENDERER_LANE_MODE_COLLAPSE_FIRSTCHILD
};

struct am_timeline_renderer {
	/* Width in pixels of the time line */
	unsigned int width;

	/* Height in pixels of the time line */
	unsigned int height;

	/* Height in pixels of a lane displaying the activity of a hierarchy
	 * node */
	double lane_height;

	/* Lane offset in pixels, i.e., by how many pixels the lanes have been
	 * "scrolled down" */
	double lane_offset;

	/* Width in pixels of the left part of the timeline with the description
	 * for each lane */
	unsigned int ydesc_width;

	/* Height in pixels of the bottom part of the timeline with the x
	 * labels */
	unsigned int xdesc_height;

	struct {
		/* Rectangle describing the area of all lanes (i.e., without
		 * legends and axes) */
		struct am_rect lanes;

		/* Legend for vertical axis on the left of the lanes */
		struct am_rect ylegend;

		/* Legend for horizontal axis below the lanes */
		struct am_rect xlegend;
	} rects;

	struct {
		/* Background color rendered below all layers */
		struct am_rgba bg;
	} colors;

	/* List of all render layers */
	struct list_head layers;

	/* Trace whose events are displayed by the time line */
	struct am_trace* trace;

	/* Hierarchy whose events are displayed by the time line */
	struct am_hierarchy* hierarchy;

	/* Start and end time stamp for the events to be displayed */
	struct am_interval visible_interval;

	/* Bitvector with one entry per hierarchy node of the displayed
	 * hierarchy, indicating whether the node is collapsed (corresponding
	 * bit is 1) or expanded (corresponding bit is 0). The index of the bit
	 * of a node is defined as the index of the parent node plus the number
	 * of preceding siblings and all of its descendants. The root node has
	 * the index 0.*/
	struct am_bitvector collapsed_nodes;

	/* Number of lanes to be rendered, taking into account collapsed
	 * nodes. */
	unsigned int num_visible_lanes;

	/* Number of lanes at the top */
	unsigned int num_invisible_lanes;

	/* Total number of lanes, just based on the height of the timeline and
	 * the lane height */
	unsigned int max_visible_lanes;

	struct {
		/* First node on the first lane */
		struct am_hierarchy_node* node;

		/* Index of the first node on the first lane */
		unsigned int node_index;
	} first_lane;

	/* Defines how lanes for parents and their children are rendered, e.g.,
	 * if the lane of a parent should be collapsed with the first child. */
	enum am_timeline_renderer_lane_mode lane_mode;

	/* List of struct am_timeline_renderer_layer_appearance_change_callback
	 * structures whose callback functions get invoked when a layer informs
	 * the timeline renderer that it's appearance has changed. */
	struct list_head layer_appearance_change_callbacks;
};

/* Entry for the list of callback functions that get invoked when a layer
 * changes it's appearance */
struct am_timeline_renderer_layer_appearance_change_callback {
	struct list_head list;
	void* data;
	void (*callback)(struct am_timeline_renderer*,
			 struct am_timeline_render_layer* layer,
			 void* data);
};

/* Initialize a layer appearance change callback entry */
static inline void am_timeline_renderer_layer_appearance_change_callback_init(
	struct am_timeline_renderer_layer_appearance_change_callback* cbfe,
	void (*callback)(struct am_timeline_renderer*,
			 struct am_timeline_render_layer* layer,
			 void* data),
	void* data)
{
	INIT_LIST_HEAD(&cbfe->list);
	cbfe->data = data;
	cbfe->callback = callback;
}

#define am_timeline_renderer_for_each_layer(r, layer) \
	am_typed_list_for_each(r, layers, layer, list)

#define am_timeline_renderer_for_each_layer_prev(r, layer) \
	am_typed_list_for_each_prev(r, layers, layer, list)

#define am_timeline_renderer_for_each_layer_safe(r, i, n) \
	am_typed_list_for_each_safe(r, layers, i, n, list)

#define am_timeline_renderer_for_each_layer_prev_safe(r, i, n) \
	am_typed_list_for_each_prev_safe(r, layers, i, n, list)

#define AM_DECL_TIMELINE_RENDERER_SETCOLOR_FUN2(color_name, member)		\
	static inline void							\
	am_timeline_renderer_set_##color_name##_color(struct am_timeline_renderer* r, \
						      double col_r,		\
						      double col_g,		\
						      double col_b,		\
						      double col_a)		\
	{									\
		r->colors.member.r = col_r;					\
		r->colors.member.g = col_g;					\
		r->colors.member.b = col_b;					\
		r->colors.member.a = col_a;					\
	}

#define AM_DECL_TIMELINE_RENDERER_SETCOLOR_FUN(member) \
	AM_DECL_TIMELINE_RENDERER_SETCOLOR_FUN2(member, member)

#define AM_DECL_TIMELINE_RENDERER_SETTER_FUN2(name, member)			\
	static inline void							\
	am_timeline_renderer_set_##name(struct am_timeline_renderer* r,	\
					typeof(r->member) name)		\
	{									\
		r->member = name;						\
	}

#define AM_DECL_TIMELINE_RENDERER_SETTER_FUN(member) \
	AM_DECL_TIMELINE_RENDERER_SETTER_FUN2(member, member)

#define AM_DECL_TIMELINE_RENDERER_GETTER_FUN2(name, member)			\
	static inline typeof(((struct am_timeline_renderer*)0)->member)	\
	am_timeline_renderer_get_##name(struct am_timeline_renderer* r)	\
	{									\
		return r->member;						\
	}

#define AM_DECL_TIMELINE_RENDERER_GETTER_FUN(member) \
	AM_DECL_TIMELINE_RENDERER_GETTER_FUN2(member, member)

#define AM_DECL_TIMELINE_RENDERER_SETTER_FUN_STRUCT2(name, member)		\
	static inline void							\
	am_timeline_renderer_set_##name(struct am_timeline_renderer* r,	\
					const typeof(r->member)* name)		\
	{									\
		r->member = *name;						\
	}

#define AM_DECL_TIMELINE_RENDERER_SETTER_FUN_STRUCT(member) \
	AM_DECL_TIMELINE_RENDERER_SETTER_FUN_STRUCT2(member, member)

#define AM_DECL_TIMELINE_RENDERER_GETTER_FUN_STRUCT2(name, member)	\
	static inline void						\
	am_timeline_renderer_get_##name(struct am_timeline_renderer* r, \
					typeof(r->member)* name)	\
	{								\
		*name = r->member;					\
	}

#define AM_DECL_TIMELINE_RENDERER_GETTER_FUN_STRUCT(member) \
	AM_DECL_TIMELINE_RENDERER_GETTER_FUN_STRUCT2(member, member)

int am_timeline_renderer_init(struct am_timeline_renderer* r);
void am_timeline_renderer_destroy(struct am_timeline_renderer* r);
int am_timeline_renderer_add_layer(struct am_timeline_renderer* r,
				   struct am_timeline_render_layer* l);
int am_timeline_renderer_remove_layer(struct am_timeline_renderer* r,
				      struct am_timeline_render_layer* l);
void am_timeline_renderer_render(struct am_timeline_renderer* r,
				 cairo_t* cr);

int am_timeline_renderer_set_trace(struct am_timeline_renderer* r,
				   struct am_trace* t);

int am_timeline_renderer_set_hierarchy(struct am_timeline_renderer* r,
				       struct am_hierarchy* h);

void am_timeline_renderer_set_width(struct am_timeline_renderer* r,
				    unsigned int w);
void am_timeline_renderer_set_height(struct am_timeline_renderer* r,
				     unsigned int h);
void am_timeline_renderer_set_lane_height(struct am_timeline_renderer* r,
					  double h);

void am_timeline_renderer_set_lane_offset(struct am_timeline_renderer* r,
					  double offs);

int am_timeline_renderer_set_horizontal_axis_y(struct am_timeline_renderer* r,
						double y);

int am_timeline_renderer_set_vertical_axis_x(struct am_timeline_renderer* r,
					     double x);

int am_timeline_renderer_collapse_node_idx(struct am_timeline_renderer* r,
					   unsigned int idx);

int am_timeline_renderer_expand_node_idx(struct am_timeline_renderer* r,
					 unsigned int idx);

int am_timeline_renderer_toggle_node_idx(struct am_timeline_renderer* r,
					 unsigned int idx);

int am_timeline_renderer_identify_entities(struct am_timeline_renderer* r,
					   struct list_head* lst,
					   double x, double y);

void am_timeline_renderer_destroy_entities(struct am_timeline_renderer* r,
					   struct list_head* lst);

void am_timeline_renderer_register_layer_appearance_change_callback(
	struct am_timeline_renderer* r,
	struct am_timeline_renderer_layer_appearance_change_callback* cbfe);

void am_timeline_renderer_unregister_layer_appearance_change_callback(
	struct am_timeline_renderer* r,
	struct am_timeline_renderer_layer_appearance_change_callback* cbfe);

void am_timeline_renderer_indicate_layer_appearance_change(
	struct am_timeline_renderer* r,
	struct am_timeline_render_layer* layer);

/* Calculates the X coordinate in pixels for a timestamp t relative to the left
 * of the rectangle for the lanes. */
static inline double
am_timeline_renderer_timestamp_to_relx(struct am_timeline_renderer* r,
				       am_timestamp_t t)
{
	struct am_time_offset d;
	am_timestamp_t dist;
	double ret;

	am_interval_duration(&r->visible_interval, &d);

	if(d.abs == 0 || r->rects.lanes.width == 0)
		return 0;

	if(t >= r->visible_interval.start) {
		dist = t - r->visible_interval.start;
		am_timestamp_muldiv_sat(&dist, r->rects.lanes.width, d.abs);
		ret = (double)dist;
	} else {
		dist = r->visible_interval.start - t;
		am_timestamp_muldiv_sat(&dist, r->rects.lanes.width, d.abs);
		ret = -((double)dist);
	}

	return ret;
}

/* Calculates the X coordinate in pixels for a timestamp t. */
static inline double
am_timeline_renderer_timestamp_to_x(struct am_timeline_renderer* r,
				    am_timestamp_t t)
{
	return r->rects.lanes.x + am_timeline_renderer_timestamp_to_relx(r, t);
}

/* Calculates the duration corresponding to a width in pixels. */
static inline enum am_arithmetic_status
am_timeline_renderer_width_to_duration(struct am_timeline_renderer* r,
				       double w,
				       struct am_time_offset* out)
{
	struct am_time_offset d;

	if(r->rects.lanes.width == 0)
		return AM_ARITHMETIC_STATUS_OVERFLOW;

	am_interval_duration(&r->visible_interval, &d);

	if(w < 0) {
		out->sign = 1;
		w = -w;
	} else {
		out->sign = 0;
	}

	return am_muldiv_sat_u64(w, d.abs, r->rects.lanes.width, &out->abs);
}

/* Calculates the timestamp given the X coordinate of a pixel relative to the
 * beginning of the rectangle containing the lanes. The return value indicates
 * whether the result has been saturated. */
static inline enum am_arithmetic_status
am_timeline_renderer_relx_to_timestamp(struct am_timeline_renderer* r,
				       double x,
				       am_timestamp_t* out)
{
	struct am_time_offset d;

	if(r->rects.lanes.width == 0) {
		*out = r->visible_interval.start;
		return AM_ARITHMETIC_STATUS_EXACT;
	}

	*out = r->visible_interval.start;

	am_timeline_renderer_width_to_duration(r, x, &d);

	if(d.sign)
		return am_timestamp_sub_sat(out, d.abs);
	else
		return am_timestamp_add_sat(out, d.abs);
}

/* Calculates the timestamp given the X coordinate of a pixel. The return value
 * indicates whether the result has been saturated. */
static inline enum am_arithmetic_status
am_timeline_renderer_x_to_timestamp(struct am_timeline_renderer* r,
				    double x,
				    am_timestamp_t* out)
{
	double rx = x - r->rects.lanes.x;
	return am_timeline_renderer_relx_to_timestamp(r, rx, out);
}

AM_DECL_TIMELINE_RENDERER_SETCOLOR_FUN2(background, bg)
AM_DECL_TIMELINE_RENDERER_GETTER_FUN(lane_offset)
AM_DECL_TIMELINE_RENDERER_GETTER_FUN(lane_height)
AM_DECL_TIMELINE_RENDERER_GETTER_FUN(lane_mode)
AM_DECL_TIMELINE_RENDERER_SETTER_FUN_STRUCT(visible_interval)
AM_DECL_TIMELINE_RENDERER_GETTER_FUN_STRUCT(visible_interval)

/* Return value for the callback function passed to
 * am_timeline_renderer_foreach_visible_lane */
enum am_timeline_renderer_lane_callback_status {
	/* Indicates that the callback should be invoked for subsequent visible
	 * lanes */
	AM_TIMELINE_RENDERER_LANE_CALLBACK_STATUS_CONTINUE = 0,

	/* Indicates that the callback should *NOT* be invoked for subsequent
	 * visible lanes */
	AM_TIMELINE_RENDERER_LANE_CALLBACK_STATUS_STOP = 1
};

typedef enum am_timeline_renderer_lane_callback_status			\
(*am_timeline_renderer_lane_fun_t)(struct am_timeline_renderer* r,
					       struct am_hierarchy_node* n,
					       unsigned int node_idx,
					       unsigned int lane,
					       void* data);

int am_timeline_renderer_foreach_visible_lane(struct am_timeline_renderer* r,
					      am_timeline_renderer_lane_fun_t cb,
					      void* data);

int am_timeline_renderer_lane_extents(struct am_timeline_renderer* r,
				      struct am_rect* e,
				      unsigned int lane);

int am_timeline_renderer_is_leaf_lane(struct am_timeline_renderer* r,
				      struct am_hierarchy_node* n,
				      unsigned int node_idx);

int am_timeline_renderer_parent_on_same_lane(struct am_timeline_renderer* r,
					     struct am_hierarchy_node* n);

void am_timeline_renderer_set_lane_mode(struct am_timeline_renderer* r,
					enum am_timeline_renderer_lane_mode m);

int am_timeline_renderer_lane_at_y(struct am_timeline_renderer* r,
				   double y,
				   unsigned int* lane);

struct am_hierarchy_node*
am_timeline_renderer_hierarchy_node_at_y(struct am_timeline_renderer* r,
					 double y);

#endif
