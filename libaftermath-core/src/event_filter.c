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

#include <aftermath/core/event_filter.h>

void am_event_filter_array_element_destroy(struct am_event_filter_element* fe)
{
	free(fe->filter_type);
	free(fe->filter_value);
}

void am_event_filter_init(struct am_event_filter* f)
{
	am_event_filter_array_init(&f->filters);
}

void am_event_filter_destroy(struct am_event_filter* f)
{
	am_event_filter_array_destroy(&f->filters);
}

/* Append a filter to the filter list. Returns 0 on success,
 * otherwise 1. */
int am_event_filter_append(struct am_event_filter* f,
					char* type,
			    void* value)
{
	struct am_event_filter_element fe = {
		.filter_type = type,
		.filter_value = value
	};

	return am_event_filter_array_append(&f->filters, fe);
}
