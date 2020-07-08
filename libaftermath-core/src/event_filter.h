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

#ifndef AM_EVENT_FILTER_H
#define AM_EVENT_FILTER_H

/* An event filter associates event filter elements to an entity (such as a
 * hierarchy node). Each element consists of a filter type and a pointer to
 * the value of the filter, with a value-type that depends on the filter
 * type
 */

#include <aftermath/core/base_types.h>
#include <aftermath/core/typed_array.h>

struct am_event_filter_element {
	/* Temporarily a filter type is just a string
	 * These filter types should be auto-generated for each filterable in-memory
   * type via some tag that can be applied
	 */
	char* filter_type;

	/* filter_value is a pointer to a type specific to the filter_type */
	void* filter_value;
};

void am_event_filter_array_element_destroy(struct am_event_filter_element* fe);

AM_DECL_TYPED_ARRAY_WITH_ELEMENT_DESTRUCTOR(
	am_event_filter_array,
	struct am_event_filter_element,
	am_event_filter_array_element_destroy)

struct am_event_filter {
	struct am_event_filter_array filters;
};

void am_event_filter_init(struct am_event_filter* f);
void am_event_filter_destroy(struct am_event_filter* f);
int am_event_filter_append(struct am_event_filter* m,
					char* type,
			    void* value);

#endif
