# Author: Andi Drebes <andi@drebesium.org>
# Author: Igor Wodiany <igor.wodiany@manchester.ac.uk>
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

from aftermath.types import TypeList, Field, FieldList
from aftermath import relations
from aftermath import tags
from aftermath.types.on_disk import Frame, EventFrame
import aftermath.types.on_disk

am_dsk_openmp_for_loop_type = Frame(
    name = "am_dsk_openmp_for_loop_type",
    entity = "on-disk OpenMP for loop",
    comment = "An OpenMP for loop type",
    fields = FieldList([
        Field(
            name = "type_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of this loop type"),
        Field(
            name = "source",
            field_type = aftermath.types.on_disk.am_dsk_source_location,
            comment = "Location of the source code for this for loop type"),
        Field(
            name = "addr",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Address of the first instruction of the outlined loop body"),
        Field(
            name = "flags",
            field_type = aftermath.types.builtin.uint32_t,
            comment = "Flags for this loop type")]))

tags.dsk.tomem.add_per_trace_array_tags(
    am_dsk_openmp_for_loop_type,
    aftermath.types.openmp.in_memory.am_openmp_for_loop_type)

################################################################################

am_dsk_openmp_for_loop_instance = Frame(
    name = "am_dsk_openmp_for_loop_instance",
    entity = "on-disk OpenMP for loop instance",
    comment = "An instance of an OpenMP for loop",

    fields = FieldList([
        Field(
            name = "type_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of the for loop type for this instance"),
        Field(
            name = "instance_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of this for_loop instance"),
        Field(
            name = "lower_bound",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "First value (included) for this loop instance"),
        Field(
            name = "upper_bound",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Last value (included) for this loop instance"),
        Field(
            name = "num_workers",
            field_type = aftermath.types.builtin.uint32_t,
            comment = "Number of workers that participated in the execution of this loop instance")]))

tags.dsk.tomem.add_per_trace_array_tags(
    am_dsk_openmp_for_loop_instance,
    aftermath.types.openmp.in_memory.am_openmp_for_loop_instance)

relations.join.make_join(
    dsk_src_field = am_dsk_openmp_for_loop_instance.getFields().getFieldByName("type_id"),
    dsk_target_field = am_dsk_openmp_for_loop_type.getFields().getFieldByName("type_id"),
    mem_ptr_field = aftermath.types.openmp.in_memory.am_openmp_for_loop_instance.getFields().getFieldByName("loop_type"),
    mem_target_type = aftermath.types.openmp.in_memory.am_openmp_for_loop_type,
    null_allowed = False)

################################################################################

am_dsk_openmp_iteration_set = Frame(
    name = "am_dsk_openmp_iteration_set",
    entity = "on-disk OpenMP for loop iteration set",
    comment = "A set of iterations executed by a worker",

    fields = FieldList([
        Field(
            name = "instance_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of the for loop instance this iteration set belongs to"),
        Field(
            name = "iteration_set_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of this for loop iteration set"),
        Field(
            name = "lower_bound",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "First value (included) for this iteration set"),
        Field(
            name = "upper_bound",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Last value (included) for this iteration set")]))

tags.dsk.tomem.add_per_trace_array_tags(
    am_dsk_openmp_iteration_set,
    aftermath.types.openmp.in_memory.am_openmp_iteration_set)

relations.join.make_join(
    dsk_src_field = am_dsk_openmp_iteration_set.getFields().getFieldByName("instance_id"),
    dsk_target_field = am_dsk_openmp_for_loop_instance.getFields().getFieldByName("instance_id"),
    mem_ptr_field = aftermath.types.openmp.in_memory.am_openmp_iteration_set.getFields().getFieldByName("loop_instance"),
    mem_target_type = aftermath.types.openmp.in_memory.am_openmp_for_loop_instance,
    null_allowed = False)

################################################################################

am_dsk_openmp_iteration_period = EventFrame(
    name = "am_dsk_openmp_iteration_period",
    entity = "on-disk OpenMP iteration execution period",
    comment = "An OpenMP iteration period (contiguous interval of " + \
              "execution of an iteration set)",
    fields = FieldList([
        Field(
            name = "iteration_set_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of the OpenMP iteration set this " + \
                      "period belongs to"),
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_iteration_period,
    aftermath.types.openmp.in_memory.am_openmp_iteration_period,
    "collection_id")

relations.join.make_join(
    dsk_src_field = am_dsk_openmp_iteration_period.getFields().getFieldByName("iteration_set_id"),
    dsk_target_field = am_dsk_openmp_iteration_set.getFields().getFieldByName("iteration_set_id"),
    mem_ptr_field = aftermath.types.openmp.in_memory.am_openmp_iteration_period.getFields().getFieldByName("iteration_set"),
    mem_target_type = aftermath.types.openmp.in_memory.am_openmp_iteration_set,
    null_allowed = False
)

################################################################################

am_dsk_openmp_task_type = Frame(
    name = "am_dsk_openmp_task_type",
    entity = "on-disk OpenMP task type",
    comment = "An OpenMP task type",
    fields = FieldList([
        Field(
            name = "type_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of this task type"),
        Field(
            name = "name",
            field_type = aftermath.types.on_disk.am_dsk_string,
            comment = "Name of this task (e.g., symbol in the executable)"),
        Field(
            name = "source",
            field_type = aftermath.types.on_disk.am_dsk_source_location,
            comment = "Location of the source code for this task type")]))

tags.dsk.tomem.add_per_trace_array_tags(
    am_dsk_openmp_task_type,
    aftermath.types.openmp.in_memory.am_openmp_task_type)

################################################################################

am_dsk_openmp_task_instance = Frame(
    name = "am_dsk_openmp_task_instance",
    entity = "on-disk OpenMP task instance",
    comment = "An instance of an OpenMP task",

    fields = FieldList([
        Field(
            name = "type_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of the task type for this instance"),
        Field(
            name = "instance_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of this task instance")]))

tags.dsk.tomem.add_per_trace_array_tags(
    am_dsk_openmp_task_instance,
    aftermath.types.openmp.in_memory.am_openmp_task_instance)

relations.join.make_join(
    dsk_src_field = am_dsk_openmp_task_instance.getFields().getFieldByName("type_id"),
    dsk_target_field = am_dsk_openmp_task_type.getFields().getFieldByName("type_id"),
    mem_ptr_field = aftermath.types.openmp.in_memory.am_openmp_task_instance.getFields().getFieldByName("task_type"),
    mem_target_type = aftermath.types.openmp.in_memory.am_openmp_task_type,
    null_allowed = False)

################################################################################

am_dsk_openmp_task_period = EventFrame(
    name = "am_dsk_openmp_task_period",
    entity = "on-disk OpenMP task execution period",
    comment = "An OpenMP task execution period (contiguous interval of " + \
              "execution of a task)",
    fields = FieldList([
        Field(
            name = "instance_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Numerical ID of the OpenMP task instance this " + \
                      "period accounts for"),
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_task_period,
    aftermath.types.openmp.in_memory.am_openmp_task_period,
    "collection_id")

relations.join.make_join(
    dsk_src_field = am_dsk_openmp_task_period.getFields().getFieldByName("instance_id"),
    dsk_target_field = am_dsk_openmp_task_instance.getFields().getFieldByName("instance_id"),
    mem_ptr_field = aftermath.types.openmp.in_memory.am_openmp_task_period.getFields().getFieldByName("task_instance"),
    mem_target_type = aftermath.types.openmp.in_memory.am_openmp_task_instance,
    null_allowed = False
)

################################################################################

am_dsk_openmp_thread = EventFrame(
    name = "am_dsk_openmp_thread",
    entity = "on-disk OpenMP thread execution interval",
    comment = "An OpenMP thread execution interval",
    fields = FieldList([
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval"),
        Field(
            name = "type",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Thread type")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_thread,
    aftermath.types.openmp.in_memory.am_openmp_thread,
    "collection_id")

################################################################################

am_dsk_openmp_parallel = EventFrame(
    name = "am_dsk_openmp_parallel",
    entity = "on-disk OpenMP parallel section execution interval",
    comment = "An OpenMP parallel section execution interval",
    fields = FieldList([
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval"),
        Field(
            name = "requested_parallelism",
            field_type = aftermath.types.builtin.uint32_t,
            comment = "Number of threads or teams in the region"),
        Field(
              name = "flags",
              field_type = aftermath.types.builtin.uint16_t,
              comment = "Type of the parallel region")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_parallel,
    aftermath.types.openmp.in_memory.am_openmp_parallel,
    "collection_id")

################################################################################

am_dsk_openmp_task_create = EventFrame(
    name = "am_dsk_openmp_task_create",
    entity = "on-disk OpenMP task create event",
    comment = "An event generated when OpenMP task regions are generated",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "task_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Unique task id"),
        Field(
            name = "new_task_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Unique new task id"),
        Field(
            name = "flags",
            field_type = aftermath.types.builtin.uint16_t,
            comment = "Kind of the task"),
        Field(
            name = "has_dependences",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "True if the task has dependences"),
        Field(
            name = "codeptr_ra",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "OpenMP region code pointer")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_task_create,
    aftermath.types.openmp.in_memory.am_openmp_task_create,
    "collection_id")

################################################################################

am_dsk_openmp_task_schedule = EventFrame(
    name = "am_dsk_openmp_task_schedule",
    entity = "on-disk OpenMP task schedule event",
    comment = "An event generated when an OpenMP task is scheduled",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "prior_task_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Unique id of the encountering task"),
        Field(
            name = "next_task_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Unique id of the next task"),
        Field(
            name = "prior_task_status",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Status of the task at the scheduling point")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_task_schedule,
    aftermath.types.openmp.in_memory.am_openmp_task_schedule,
    "collection_id")

################################################################################

am_dsk_openmp_implicit_task = EventFrame(
    name = "am_dsk_openmp_implicit_task",
    entity = "on-disk OpenMP initial or implicit task execution interval",
    comment = "An OpenMP initial or implicit task execution interval",
    fields = FieldList([
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval"),
        Field(
            name = "actual_parallelism",
            field_type = aftermath.types.builtin.uint32_t,
            comment = "Number of threads or teams in the region"),
        Field(
            name = "index",
            field_type = aftermath.types.builtin.uint32_t,
            comment = "Thread or team number of the calling thread"),
        Field(
            name = "flags",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Indication if the task is initial or implicit")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_implicit_task,
    aftermath.types.openmp.in_memory.am_openmp_implicit_task,
    "collection_id")

################################################################################

am_dsk_openmp_sync_region_wait = EventFrame(
    name = "am_dsk_openmp_sync_region_wait",
    entity = "on-disk OpenMP sync region wait execution interval",
    comment = "An OpenMP sync region wait execution interval",
    fields = FieldList([
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval"),
        Field(
            name = "kind",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Argument indicates a type of the synchronization "
                      + "construct")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_sync_region_wait,
    aftermath.types.openmp.in_memory.am_openmp_sync_region_wait,
    "collection_id")

################################################################################

am_dsk_openmp_mutex_released = EventFrame(
    name = "am_dsk_openmp_mutex_released",
    entity = "on-disk OpenMP mutex released event",
    comment = "An event generated when an OpenMP mutex is released",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "wait_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "ID of the object that is being released"),
        Field(
            name = "kind",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Type of the mutual exclusion event")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_mutex_released,
    aftermath.types.openmp.in_memory.am_openmp_mutex_released,
    "collection_id")

###############################################################################

am_dsk_openmp_dependences = EventFrame(
    name = "am_dsk_openmp_dependences",
    entity = "on-disk OpenMP dependences event",
    comment = "An event generated when new tasks and ordered constructs with "
              + "dependences are dispatched",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "ndeps",
            field_type = aftermath.types.builtin.uint32_t,
            comment = "Number of dependences")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_dependences,
    aftermath.types.openmp.in_memory.am_openmp_dependences,
    "collection_id")

################################################################################

am_dsk_openmp_task_dependence = EventFrame(
    name = "am_dsk_openmp_task_dependence",
    entity = "on-disk OpenMP task dependence event",
    comment = "An event generated when unfulfilled task dependences are "
              + "encountered",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "src_task_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Unique id of the source task"),
        Field(
            name = "sink_task_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Unique id of the sink task")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_task_dependence,
    aftermath.types.openmp.in_memory.am_openmp_task_dependence,
    "collection_id")

################################################################################

am_dsk_openmp_work = EventFrame(
    name = "am_dsk_openmp_work",
    entity = "on-disk OpenMP work execution interval",
    comment = "An OpenMP work execution interval",
    fields = FieldList([
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval"),
        Field(
            name = "type",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Type of the work"),
        Field(
            name = "count",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Measure of quantity involved in work")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_work,
    aftermath.types.openmp.in_memory.am_openmp_work,
    "collection_id")

################################################################################

am_dsk_openmp_master = EventFrame(
    name = "am_dsk_openmp_master",
    entity = "on-disk OpenMP master region execution interval",
    comment = "An OpenMP master region execution interval",
    fields = FieldList([
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_master,
    aftermath.types.openmp.in_memory.am_openmp_master,
    "collection_id")

################################################################################

am_dsk_openmp_sync_region = EventFrame(
    name = "am_dsk_openmp_sync_region",
    entity = "on-disk OpenMP sync region execution interval",
    comment = "An OpenMP sync region execution interval",
    fields = FieldList([
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval"),
        Field(
            name = "kind",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Type of the synchronization construct")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_sync_region,
    aftermath.types.openmp.in_memory.am_openmp_sync_region,
    "collection_id")

################################################################################

am_dsk_openmp_lock_init = EventFrame(
    name = "am_dsk_openmp_lock_init",
    entity = "on-disk OpenMP lock init event",
    comment = "An event generated when an OpenMP lock is initialized",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "wait_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "ID of the lock that is being initialized"),
        Field(
            name = "kind",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Type of the lock")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_lock_init,
    aftermath.types.openmp.in_memory.am_openmp_lock_init,
    "collection_id")

################################################################################

am_dsk_openmp_lock_destroy = EventFrame(
    name = "am_dsk_openmp_lock_destroy",
    entity = "on-disk OpenMP lock destroyed event",
    comment = "An event generated when an OpenMP lock is destroyed",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "wait_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "ID of the lock that is being destroyed"),
        Field(
            name = "kind",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Type of the lock")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_lock_destroy,
    aftermath.types.openmp.in_memory.am_openmp_lock_destroy,
    "collection_id")

################################################################################

am_dsk_openmp_mutex_acquire = EventFrame(
    name = "am_dsk_openmp_mutex_acquire",
    entity = "on-disk OpenMP mutex acquire event",
    comment = "An event generated when an OpenMP mutex is being acquired",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "wait_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "ID of the object that is being locked"),
        Field(
            name = "kind",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Type of the mutual exclusion"),
        Field(
            name = "hint",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Hint provided at mutex initialization"),
        Field(
            name = "implementation",
            field_type = aftermath.types.builtin.uint32_t,
            comment = "Mechanism chosen by runtime to implement the mutex")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_mutex_acquire,
    aftermath.types.openmp.in_memory.am_openmp_mutex_acquire,
    "collection_id")

################################################################################

am_dsk_openmp_mutex_acquired = EventFrame(
    name = "am_dsk_openmp_mutex_acquired",
    entity = "on-disk OpenMP mutex acquired event",
    comment = "An event when an OpenMP mutex is acquired",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "wait_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "ID of the object that is being locked"),
        Field(
            name = "kind",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Type of the mutual exclusion")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_mutex_acquired,
    aftermath.types.openmp.in_memory.am_openmp_mutex_acquired,
    "collection_id")

################################################################################

am_dsk_openmp_nest_lock = EventFrame(
    name = "am_dsk_openmp_nest_lock",
    entity = "on-disk OpenMP nest lock execution interval",
    comment = "An OpenMP nested lock interval when the nested lock was held",
    fields = FieldList([
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval"),
        Field(
            name = "wait_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "ID of the object that is locked")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_nest_lock,
    aftermath.types.openmp.in_memory.am_openmp_nest_lock,
    "collection_id")

################################################################################

am_dsk_openmp_flush = EventFrame(
    name = "am_dsk_openmp_flush",
    entity = "on-disk OpenMP flush event",
    comment = "A memory flush event generated by the OpenMP flush construct",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_flush,
    aftermath.types.openmp.in_memory.am_openmp_flush,
    "collection_id")

################################################################################

am_dsk_openmp_cancel = EventFrame(
    name = "am_dsk_openmp_cancel",
    entity = "on-disk OpenMP cancel event",
    comment = "An event generated by OpenMP cancel construct",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "flags",
            field_type = aftermath.types.builtin.uint8_t,
            comment = "Cancellation source and cancelled construct information")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_cancel,
    aftermath.types.openmp.in_memory.am_openmp_cancel,
    "collection_id")

################################################################################

am_dsk_openmp_loop = EventFrame(
    name = "am_dsk_openmp_loop",
    entity = "on-disk OpenMP loop execution interval",
    comment = "An OpenMP loop execution interval",
    fields = FieldList([
        Field(
            name = "interval",
            field_type = aftermath.types.on_disk.am_dsk_interval,
            comment = "Start and end of the execution interval"),
        Field(
            name = "instance_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Loop instance id"),
        Field(
            name = "flags",
            field_type = aftermath.types.builtin.int64_t,
            comment = "Flags associated with the loop"),
        Field(
            name = "lower_bound",
            field_type = aftermath.types.builtin.int64_t,
            comment = "Lower bound of the loop"),
        Field(
            name = "upper_bound",
            field_type = aftermath.types.builtin.int64_t,
            comment = "Lower bound of the loop"),
        Field(
            name = "increment",
            field_type = aftermath.types.builtin.int64_t,
            comment = "Loop increment"),
        Field(
            name = "num_workers",
            field_type = aftermath.types.builtin.int32_t,
            comment = "Number of workers"),
        Field(
            name = "codeptr_ra",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Loop body address")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_loop,
    aftermath.types.openmp.in_memory.am_openmp_loop,
    "collection_id")

################################################################################

am_dsk_openmp_loop_chunk = EventFrame(
    name = "am_dsk_openmp_loop_chunk",
    entity = "on-disk OpenMP loop chunk dispatch event",
    comment = "An event generated by OpenMP loop chunk dispatch",
    fields = FieldList([
        Field(
            name = "timestamp",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Time of the event"),
        Field(
            name = "instance_id",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Associated loop instance id"),
        Field(
            name = "lower_bound",
            field_type = aftermath.types.builtin.int64_t,
            comment = "Lower bound of the chunk"),
        Field(
            name = "upper_bound",
            field_type = aftermath.types.builtin.int64_t,
            comment = "Lower bound of the chunk"),
        Field(
            name = "is_last",
            field_type = aftermath.types.builtin.int32_t,
            comment = "Is it end of the last period")]))

tags.dsk.tomem.add_per_event_collection_tags(
    am_dsk_openmp_loop_chunk,
    aftermath.types.openmp.in_memory.am_openmp_loop_chunk,
    "collection_id")

################################################################################

all_types = TypeList([
    am_dsk_openmp_for_loop_type,
    am_dsk_openmp_for_loop_instance,
    am_dsk_openmp_iteration_set,
    am_dsk_openmp_iteration_period,
    am_dsk_openmp_task_type,
    am_dsk_openmp_task_instance,
    am_dsk_openmp_task_period,
    am_dsk_openmp_thread,
    am_dsk_openmp_parallel,
    am_dsk_openmp_task_create,
    am_dsk_openmp_task_schedule,
    am_dsk_openmp_implicit_task,
    am_dsk_openmp_sync_region_wait,
    am_dsk_openmp_mutex_released,
    am_dsk_openmp_dependences,
    am_dsk_openmp_task_dependence,
    am_dsk_openmp_work,
    am_dsk_openmp_master,
    am_dsk_openmp_sync_region,
    am_dsk_openmp_lock_init,
    am_dsk_openmp_lock_destroy,
    am_dsk_openmp_mutex_acquire,
    am_dsk_openmp_mutex_acquired,
    am_dsk_openmp_nest_lock,
    am_dsk_openmp_flush,
    am_dsk_openmp_cancel,
    am_dsk_openmp_loop,
    am_dsk_openmp_loop_chunk
])

aftermath.config.addDskTypes(*all_types)
