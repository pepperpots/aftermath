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

#include <aftermath/core/telamon.h>

/* Internal callback function for am_dfs_norec_telamon_candidate_depth */
void am_telamon_depth_dfs_callback(const struct am_telamon_candidate* node,
				   size_t depth,
				   void* data)
{
	size_t* pmax_depth = (size_t*)data;

	if(depth > *pmax_depth)
		*pmax_depth = depth;
}

/* Internal callback function for am_dfs_norec_telamon_candidate_count_nodes */
void am_telamon_count_nodes_dfs_callback(const struct am_telamon_candidate* node,
				   size_t depth,
				   void* data)
{
	size_t* count = (size_t*)data;
	(*count)++;
}
