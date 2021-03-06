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

#include <aftermath/render/timeline/layers/interval.h>
#include <aftermath/render/timeline/renderer.h>
#include <aftermath/core/state_event_array.h>
#include <aftermath/core/event_collection.h>

struct am_timeline_interval_layer {
	struct am_timeline_lane_render_layer super;
	const struct am_color_map* color_map;
	struct am_interval_stats_by_index statistics;
	int statistics_init;
	void* extra_data;
};

struct am_timeline_interval_layer_type {
	struct am_timeline_lane_render_layer_type super;
	char* event_array_type_name;
	size_t element_size;
	off_t interval_offset;
	off_t index_offset;
	unsigned int index_bits;

	void (*stats_subtree)(struct am_timeline_lane_render_layer*,
			      struct am_interval_stats_by_index*,
			      struct am_hierarchy_node*,
			      const struct am_interval*);

	size_t (*calculate_index)(struct am_timeline_interval_layer*, void*);
};

/* Sets the set of colors to be used for rendering. */
void
am_timeline_interval_layer_set_color_map(struct am_timeline_interval_layer* l,
					 const struct am_color_map* color_map)
{
	l->color_map = color_map;
}

/* Sets the maximum interval index. This must be equal or greater than the
 * maximum index that can appear when processing elements.
 *
 * Returns 0 on success, otherwise 1.
 */
int
am_timeline_interval_layer_set_max_index(struct am_timeline_interval_layer* l,
					 size_t max_idx)
{
	if(l->statistics_init)
		am_interval_stats_by_index_destroy(&l->statistics);

	l->statistics_init = 0;

	if(am_interval_stats_by_index_init(&l->statistics, max_idx))
		return 1;

	l->statistics_init = 1;

	return 0;
}

/* Associates private data with the timeline interval layer l. */
void
am_timeline_interval_layer_set_extra_data(struct am_timeline_interval_layer* l,
					  void* extra_data)
{
	l->extra_data = extra_data;
}

/* Returns the private data associated with the timeline interval layer l. */
void*
am_timeline_interval_layer_get_extra_data(struct am_timeline_interval_layer* l)
{
	return l->extra_data;
}

/* Calculates the statistics for an interval i, starting with the hierarchy node
 * hn. If the layer's render mode is
 * AM_TIMELINE_LANE_RENDER_MODE_COMBINE_SUBTREE, the function recurses on the
 * children of hn.
 */
static void am_timeline_interval_layer_default_stats_subtree(
	struct am_timeline_lane_render_layer* rl,
	struct am_interval_stats_by_index* stats,
	struct am_hierarchy_node* hn,
	const struct am_interval* i)
{
	struct am_typed_array_generic* ea;
	struct am_event_mapping* m = &hn->event_mapping;
	struct am_event_collection* ec;
	struct am_hierarchy_node* child;
	struct am_timeline_interval_layer* il = (typeof(il))rl;
	struct am_timeline_render_layer* l = AM_TIMELINE_RENDER_LAYER(il);
	struct am_timeline_interval_layer_type* ilt = (typeof(ilt))l->type;

	am_event_mapping_for_each_collection_overlapping(m, i, ec) {
		ea = am_event_collection_find_event_array(
			ec, ilt->event_array_type_name);

		if(!ea)
			continue;

		if(ilt->calculate_index) {
			am_interval_stats_by_index_fun_collect(
				stats,
				i,
				ea,
				ilt->element_size,
				ilt->interval_offset,
				(size_t (*) (void*, void*))ilt->calculate_index,
				il);
		} else {
			am_interval_stats_by_index_collect(
				stats,
				i,
				ea,
				ilt->element_size,
				ilt->interval_offset,
				ilt->index_offset,
				ilt->index_bits);
		}
	}

	if(il->super.render_mode ==
	   AM_TIMELINE_LANE_RENDER_MODE_COMBINE_SUBTREE)
	{
		am_hierarchy_node_for_each_child(hn, child) {
			am_timeline_interval_layer_default_stats_subtree(
				rl, stats, child, i);
		}
	}
}

/* Render function of the layer */
static void render(struct am_timeline_interval_layer* il,
		   struct am_hierarchy_node* hn,
		   struct am_interval* i,
		   double lane_width,
		   double lane_height,
		   cairo_t* cr)
{
	struct am_timeline_interval_layer_type* ilt;
	struct am_interval i_px;
	struct am_timeline_renderer* r;
	const struct am_rgba* color;
	unsigned int last_start_px = 0;
	size_t idx;
	size_t last_idx = 0;
	int last_valid = 0;
	int valid;

	ilt = (struct am_timeline_interval_layer_type*)il->super.super.type;

	if(!il->color_map || !il->statistics_init)
		return;

	r = AM_TIMELINE_RENDER_LAYER(il)->renderer;

	/* Process horizontal pixels of the lane */
	for(unsigned int px = 0; px < ceil(lane_width); px++) {
		am_timeline_renderer_relx_to_timestamp(r, px, &i_px.start);
		am_timeline_renderer_relx_to_timestamp(r, px+1, &i_px.end);

		/* Intervals are always inclusive; Exclude the last timestamp
		 * from the current interval, since it will already be included
		 * in the interval for the next pixel. */
		if(i_px.end > i_px.start+1)
			i_px.end--;

		am_interval_stats_by_index_reset(&il->statistics);

		ilt->stats_subtree(&il->super, &il->statistics, hn, &i_px);

		valid = am_interval_stats_by_index_max(&il->statistics, &idx);

		/* Draw the previous rectangle if the current color is different
		 * or if the current pixel is transparent. */
		if((!valid && last_valid) ||
		   (valid && last_valid && last_idx != idx))
		{
			color = am_color_map_get_color(il->color_map, last_idx);

			if(color) {
				cairo_set_source_rgba(cr, AM_PRGBA_ARGS(color));
				cairo_rectangle(cr,
						last_start_px + 0.5,
						0,
						px - last_start_px,
						lane_height);
				cairo_fill(cr);
			}
		}

		if(valid) {
			if(!last_valid || last_idx != idx)
				last_start_px = px;

			last_valid = 1;
			last_idx = idx;
		} else {
			last_valid = 0;
		}
	}

	/* Finish last rectangle if remaining */
	if(last_valid) {
		color = am_color_map_get_color(il->color_map, last_idx);

		if(color) {
			cairo_set_source_rgba(cr, AM_PRGBA_ARGS(color));
			cairo_rectangle(cr,
					last_start_px + 0.5,
					0,
					ceil(lane_width) - last_start_px,
					lane_height);
			cairo_fill(cr);
		}
	}
}

static void destroy(struct am_timeline_interval_layer* l)
{
	if(l->statistics_init)
		am_interval_stats_by_index_destroy(&l->statistics);
}

static struct am_timeline_interval_layer*
instantiate(struct am_timeline_interval_layer_type* t)
{
	struct am_timeline_interval_layer* l;

	if(!(l = malloc(sizeof(*l))))
		return NULL;

	l->color_map = NULL;
	l->statistics_init = 0;
	l->extra_data = NULL;

	am_timeline_lane_render_layer_init(&l->super, &t->super);

	return l;
}

/* Common type instantiation function used by
 * am_timeline_interval_layer_instantiate_type_default_stats_common and
 * am_timeline_interval_layer_instantiate_type_stats_fun */
struct am_timeline_interval_layer_type*
am_timeline_interval_layer_instantiate_type_common(
	const char* name)
{
	struct am_timeline_interval_layer_type* t;

	if(!(t = calloc(1, sizeof(*t))))
		goto out_err;

	if(am_timeline_lane_render_layer_type_init(&t->super, name))
		goto out_err_free;

	t->super.destroy = AM_TIMELINE_LANE_RENDER_LAYER_DESTROY_FUN(destroy);
	t->super.render = AM_TIMELINE_LANE_RENDER_LAYER_RENDER_FUN(render);
	t->super.instantiate =
		AM_TIMELINE_LANE_RENDER_LAYER_INSTANTIATE_FUN(instantiate);

	return t;

out_err_free:
	free(t);
out_err:
	return NULL;
}

/* Common type instantiation function used by
 * am_timeline_interval_layer_instantiate_type_index_member and
 * am_timeline_interval_layer_instantiate_type_index_fun */
struct am_timeline_interval_layer_type*
am_timeline_interval_layer_instantiate_type_default_stats_common(
	const char* name,
	const char* event_array_type_name,
	size_t element_size,
	off_t interval_offset)
{
	struct am_timeline_interval_layer_type* t;

	if(!(t = am_timeline_interval_layer_instantiate_type_common(name)))
		goto out_err;

	/* FIXME: Never freed */
	if(!(t->event_array_type_name = strdup(event_array_type_name)))
		goto out_err_destroy;

	t->element_size = element_size;
	t->interval_offset = interval_offset;
	t->stats_subtree = am_timeline_interval_layer_default_stats_subtree;

	return t;

out_err_destroy:
	am_timeline_render_layer_type_destroy(AM_TIMELINE_RENDER_LAYER_TYPE(t));
	free(t);
out_err:
	return NULL;
}

/* Instatiate an interval layer type. Name is the name of the instantiated type,
 * event_array_type_name the name of the array type whose events are rendered
 * for each event collection, element_size defines the size in bytes of each
 * array element, interval_offset is the offset in bytes of the interval member
 * to extract from each array element, index_offset the offset of the interval
 * index (e.g., interval type) and index_bits specifies the size of each index
 * field in bits.
 */
struct am_timeline_render_layer_type*
am_timeline_interval_layer_instantiate_type_index_member(
	const char* name,
	const char* event_array_type_name,
	size_t element_size,
	off_t interval_offset,
	off_t index_offset,
	unsigned int index_bits)
{
	struct am_timeline_interval_layer_type* t;

	if(!(t = am_timeline_interval_layer_instantiate_type_default_stats_common(
		     name, event_array_type_name, element_size, interval_offset)))
	{
		return NULL;
	}

	t->index_offset = index_offset;
	t->index_bits = index_bits;

	return AM_TIMELINE_RENDER_LAYER_TYPE(t);
}

/* Instatiate an interval layer type. Name is the name of the instantiated type,
 * event_array_type_name the name of the array type whose events are rendered
 * for each event collection, element_size defines the size in bytes of each
 * array element and interval_offset is the offset in bytes of the interval
 * member to extract from each array element.
 *
 * Calculate_index is invoked for each event instance considered by the renderer
 * in order to obtain an index.
 */
struct am_timeline_render_layer_type*
am_timeline_interval_layer_instantiate_type_index_fun(
	const char* name,
	const char* event_array_type_name,
	size_t element_size,
	off_t interval_offset,
	size_t (*calculate_index)(struct am_timeline_interval_layer*, void*))
{
	struct am_timeline_interval_layer_type* t;

	if(!(t = am_timeline_interval_layer_instantiate_type_default_stats_common(
		     name, event_array_type_name, element_size, interval_offset)))
	{
		return NULL;
	}

	t->calculate_index = calculate_index;

	return AM_TIMELINE_RENDER_LAYER_TYPE(t);
}

/* Instatiate an interval layer type with a custom statistics function. Name is
 * the name of the instantiated type. Stats_subtree is invoked for each visible
 * lane.
 */
struct am_timeline_render_layer_type*
am_timeline_interval_layer_instantiate_type_stats_fun(
	const char* name,
	void (*stats_subtree)(struct am_timeline_lane_render_layer*,
			      struct am_interval_stats_by_index*,
			      struct am_hierarchy_node*,
			      const struct am_interval*))
{
	struct am_timeline_interval_layer_type* t;

	if(!(t = am_timeline_interval_layer_instantiate_type_common(name)))
		return NULL;

	t->stats_subtree = stats_subtree;
	t->calculate_index = NULL;

	return AM_TIMELINE_RENDER_LAYER_TYPE(t);
}

/* Calculates the index whose intervals account for most of the time for the
 * interval i for the hierarchy node hn. The dominant index is returned in
 * *index. However, if no intervals overlap with i, *index_valid is 0, otherwise
 * 1.
 *
 * Returns 0 on success, otherwise 1.
 */
int am_timeline_interval_layer_get_dominant_index(
	struct am_timeline_interval_layer* il,
	struct am_hierarchy_node* hn,
	const struct am_interval* i,
	size_t* index,
	int* index_valid)
{
	struct am_timeline_interval_layer_type* ilt;
	struct am_interval_stats_by_index stats;

	ilt = (struct am_timeline_interval_layer_type*)il->super.super.type;

	if(am_interval_stats_by_index_init(&stats, il->statistics.max_index))
		return 1;

	am_interval_stats_by_index_reset(&stats);

	ilt->stats_subtree(&il->super, &stats, hn, i);
	*index_valid = am_interval_stats_by_index_max(&stats, index);

	am_interval_stats_by_index_destroy(&stats);

	return 0;
}
