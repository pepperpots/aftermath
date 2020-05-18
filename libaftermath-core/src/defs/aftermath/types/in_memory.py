# Author: Andi Drebes <andi@drebesium.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as published
# by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301,
# USA.

from aftermath.types import TypeList, \
    CompoundType, \
    NonCompoundType, \
    Field, \
    FieldList
import aftermath.types.base
from aftermath import tags

#################################################################################

class InMemoryCompoundType(CompoundType):
    """A CompoundType for in-memory data structures. Checks for in-memory-specific
    tags upon invocation of finalize()"""

    def __init__(self, *args, **kwargs):
        CompoundType.__init__(self, *args, **kwargs)

        # By default check if an update of the minimum and maximum timestamp for
        # the trace is needed
        self.addTag(tags.process.CheckGenerateTraceMinMaxTimestampCompoundScan())

    def finalize(self):
        CompoundType.finalize(self)

        # Check update of minimum and maximum timestamp for the trace is desired
        # and needed
        if self.hasTag(tags.process.CheckGenerateTraceMinMaxTimestampCompoundScan):
            if self.referencesType(aftermath.types.base.am_timestamp_t):
                scan_tag = tags.process.GenerateTraceMinMaxTimestampCompoundScan()
                self.addTags(scan_tag)
                process_tag = self.getOrAddTagInheriting(tags.process.GenerateProcessFunction)
                process_tag.addStep(tags.FunctionTagStep(scan_tag))

#################################################################################

am_counter_event = InMemoryCompoundType(
    name = "am_counter_event",
    entity = "counter event",
    comment = "A counter event",
    ident = "am::core::counter_event",
    tags = [ tags.mem.dfg.DeclareConstPointerType() ],

    fields = FieldList([
        Field(
            name = "time",
            field_type = aftermath.types.base.am_timestamp_t,
            comment = "Timestamp of the counter interval"),
        Field(
            name = "value",
            field_type = aftermath.types.base.am_counter_value_t,
            comment = "Value of the counter event")]))

#################################################################################

am_interval = InMemoryCompoundType(
    name = "am_interval",
    entity = "interval",
    comment = "An interval with a start and end timestamp.",
    ident = "am::core::interval",

    fields = FieldList([
        Field(
            name = "start",
            field_type = aftermath.types.base.am_timestamp_t,
            comment = "Start of the interval"),
        Field(
            name = "end",
            field_type = aftermath.types.base.am_timestamp_t,
            comment = "End of the interval")]))

#################################################################################

am_measurement_interval = InMemoryCompoundType(
    name = "am_measurement_interval",
    entity = "measurement interval",
    comment = "A measurement interval",
    ident = "am::core::measurement_interval",
    tags = [ tags.mem.dfg.DeclareConstPointerType() ],

    fields = FieldList([
        Field(
            name = "interval",
            field_type = am_interval,
            comment = "Start and end of the measurement interval")]))

#################################################################################

am_time_offset = InMemoryCompoundType(
    name = "am_time_offset",
    entity = "timestamp difference",
    comment = "Difference between two timestamps",
    ident = "am::core::time_offset",

    fields = FieldList([
        Field(
            name = "abs",
            field_type = aftermath.types.base.am_timestamp_t,
            comment = "Absolute difference"),
        Field(
            name = "sign",
            field_type = aftermath.types.base.am_bool_t,
            comment = "If sign != 0 the difference is negative")]))

#################################################################################

am_counter_description = InMemoryCompoundType(
    name = "am_counter_description",
    entity = "counter description",
    comment = "A description associating a name with a numerical counter ID",
    ident = "am::core::counter_description",
    tags = [ tags.mem.dfg.DeclareConstPointerType() ],

    fields = FieldList([
        Field(
            name = "name",
            field_type = aftermath.types.base.am_string,
            comment = "Name of the counter")]))

#################################################################################

am_state_description = InMemoryCompoundType(
    name = "am_state_description",
    entity = "state description",
    comment = "A description associating a name with a numerical state ID",
    ident = "am::core::state_description",
    tags = [ tags.mem.dfg.DeclareConstPointerType() ],

    fields = FieldList([
        Field(
            name = "name",
            field_type = aftermath.types.base.am_string,
            comment = "Name of the state")]))

#################################################################################

am_state_event = InMemoryCompoundType(
    name = "am_state_event",
    entity = "state",
    comment = 'A state (e.g., a run-time state)',
    ident = "am::core::state_event",
    tags = [
        tags.mem.dfg.DeclareConstPointerType(),
        tags.mem.dfg.DeclareEventMappingOverlappingIntervalExtractionNode(
            stripname_plural = "state_events",
            port_name = "state events",
            include_file = "<aftermath/core/state_event_array.h>",
            title_hrplural_cap = "State Events")
    ],

    fields = FieldList([
        Field(
            name = "interval",
            field_type = am_interval,
            comment = "Interval during which the state was active"),
        Field(
            name = "state",
            field_type = am_state_description,
            comment = "state description",
            is_pointer = True),
        Field(
            name = "state_idx",
            field_type = aftermath.types.base.am_state_t,
            comment = "Index of the state"),
    ]))

#################################################################################

am_source_location = InMemoryCompoundType(
    name = "am_source_location",
    entity = "source location",
    comment = "A source code location (file, line, character)",
    ident = "am::core::source_location",
    tags = [ tags.mem.dfg.DeclareConstPointerType() ],

    fields = FieldList([
        Field(
            name = "file",
            field_type = aftermath.types.base.am_string,
            comment = "File containing the source"),
        Field(
            name = "line",
            field_type = aftermath.types.base.am_source_line_t,
            comment = "Zero-indexed number of the line within the file"),
        Field(
            name = "character",
            field_type = aftermath.types.base.am_source_character_t,
            comment = "Zero-indexed number of the character within the line")
    ]))

#################################################################################

am_function_symbol = InMemoryCompoundType(
    name = "am_function_symbol",
    entity = "function symbol",
    comment = "A function symbol, as declared in the program's symbol table",
    ident = "am::function_symbol",

    fields = FieldList([
        Field(
            name = "addr",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Memory address of the symbol"),
        Field(
            name = "name",
            field_type = aftermath.types.base.am_string,
            comment = "Name of the symbol")]))

################################################################################

am_stack_frame = InMemoryCompoundType(
    name = "am_stack_frame",
    entity = "Stack frame",
    comment = "A stack frame within the function call graph",
    ident = "am::stack_frame",

    fields = FieldList([
        Field(
            name = "function_symbol",
            field_type = am_function_symbol,
            is_pointer = True,
						is_owened = False,
            comment = "What function from the symbol table did this stack " + \
						"frame execute"),
        Field(
            name = "interval",
            field_type = am_interval,
            comment = "Interval during which the state was active")
				]))

am_stack_frame.getFields().prependFields([
		Field(
				name = "parent_frame",
				field_type = am_stack_frame,
				is_pointer = True,
				is_owned = False,
				comment = "The stack frame immediately below this on the stack"),
		Field(
				name = "num_child_frames",
				field_type = aftermath.types.builtin.size_t,
				comment = "Number of entries in the child_frames array (how " + \
				"many instrumented function calls did this frame make)"),
		Field(
				name = "child_frames",
				field_type = am_stack_frame,
				is_pointer = True,
				# Not owned, as we want to access these stack frames independently
				is_owned = False, 
				is_array = True,
				array_num_elements_field_name = "num_child_frames",
				comment = "Frames pushed immediately above this on the stack")])

am_stack_frame.addTag(
    aftermath.tags.GenerateDefaultConstructor(field_values = [
        ("parent_frame", "NULL"),
        ("num_child_frames", "0"),
        ("child_frames", "NULL")
    ]))

################################################################################

toplevel_types = TypeList([
    am_counter_event,
    am_measurement_interval,
    am_counter_description,
    am_state_description,
    am_state_event,
    am_source_location,
		am_function_symbol,
		am_stack_frame
])

aux_types = TypeList([
    am_interval,
    am_time_offset
])

all_types = toplevel_types + aux_types

aftermath.config.addMemTypes(*all_types)
