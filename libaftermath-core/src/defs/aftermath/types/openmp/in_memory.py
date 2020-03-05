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

from aftermath.types import TypeList, Field, FieldList, EnumType, EnumVariant
from aftermath.types.in_memory import InMemoryCompoundType
from aftermath import tags
import aftermath.types.base

am_openmp_sync_region_t = EnumType(
    name = "enum am_openmp_sync_region_t",
    entity = "An OpenMP valid synchronization region kind",
    comment = "Type defines the valid kind of the synchronization region",
    variants = [
        EnumVariant(
            "AM_OPENMP_SYNC_REGION_BARRIER",
            1,
            "Barrier"),

        EnumVariant(
            "AM_OPENMP_SYNC_REGION_BARRIER_IMPLICIT",
            2,
            "Implicit Barrier"),

        EnumVariant(
            "AM_OPENMP_SYNC_REGION_BARRIER_EXPLICIT",
            3,
            "Explicit barrier"),

        EnumVariant(
            "AM_OPENMP_SYNC_REGION_BARRIER_IMPLEMENTATION",
            4,
            "Barrier implementation"),

        EnumVariant(
            "AM_OPENMP_SYNC_REGION_TASKWAIT",
            5,
            "Taskwait"),

        EnumVariant(
            "AM_OPENMP_SYNC_REGION_TASKGROUP",
            6,
            "Taskgroup"),

        EnumVariant(
            "AM_OPENMP_SYNC_REGION_REDUCTION",
            7,
            "Reduction")
    ])

################################################################################

am_openmp_work_t = EnumType(
    name = "enum am_openmp_work_t",
    entity = "OpenMP work types",
    comment = "Type defines the valid OpenMP work types",
    variants = [
        EnumVariant(
            "AM_OPENMP_WORK_LOOP",
            1,
            "Loop"),

        EnumVariant(
            "AM_OPENMP_WORK_SECTIONS",
            2,
            "Sections"),

        EnumVariant(
            "AM_OPENMP_WORK_SINGLE_EXECUTOR",
            3,
            "Single executor"),

        EnumVariant(
            "AM_OPENMP_WORK_SINGLE_OTHER",
            4,
            "Single other"),

        EnumVariant(
            "AM_OPENMP_WORK_WORKSHARE",
            5,
            "Workshare"),

        EnumVariant(
            "AM_OPENMP_WORK_DISTRIBUTE",
            6,
            "Distribute"),

        EnumVariant(
            "AM_OPENMP_WORK_TASKLOOP",
            7,
            "Taskloop")
    ])

################################################################################

am_openmp_thread_t = EnumType(
    name = "enum am_openmp_thread_t",
    entity = "OpenMP thread types",
    comment = "Type defines the valid OpenMP thread types",
    variants = [
        EnumVariant(
            "AM_OPENMP_THREAD_INITIAL",
            1,
            "Initial thread"),

        EnumVariant(
            "AM_OPENMP_THREAD_WORKER",
            2,
            "Worker thread"),

        EnumVariant(
            "AM_OPENMP_THREAD_OTHER",
            3,
            "Other thread"),

        EnumVariant(
            "AM_OPENMP_THREAD_UNKNOWN",
            4,
            "Unknown thread")
    ])

################################################################################

am_openmp_parallel_flag_t = EnumType(
    name = "enum am_openmp_parallel_flag_t",
    entity = "OpenMP parallel region types",
    comment = "Type defines valid OpenMP parallel region types",
    variants = [
        EnumVariant(
            "AM_OPENMP_INVOKER_PROGRAM_LEAGUE",
            0x40000001,
            "Invoker program and league"),

        EnumVariant(
            "AM_OPENMP_INVOKER_PROGRAM_TEAM",
            0x80000001,
            "Invoker program and team"),

        EnumVariant(
            "AM_OPENMP_INVOKER_RUNTIME_LEAGUE",
            0x40000002,
            "Invoker runtime and league"),

        EnumVariant(
            "AM_OPENMP_INVOKER_RUNTIME_TEAM",
            0x80000002,
            "Invoker runtime and league")
    ])

################################################################################

am_openmp_implicit_task_t = EnumType(
    name = "enum am_openmp_implicit_task_t",
    entity = "OpenMP implicit task types",
    comment = "Type defines valid OpenMP implicit task types",
    variants = [
        EnumVariant(
            "AM_OPENMP_IMPLICIT_TASK_INITIAL",
            1,
            "Initial task"),

        EnumVariant(
            "AM_OPENMP_IMPLICIT_TASK_IMPLICIT",
            2,
            "Implicit task"),
    ])

################################################################################

am_openmp_mutex_t = EnumType(
    name = "enum am_openmp_mutex_t",
    entity = "OpenMP mutex types",
    comment = "Type defines valid OpenMP mutex types",
    variants = [
        EnumVariant(
            "AM_OPENMP_MUTEX_LOCK",
            1,
            "Lock"),

        EnumVariant(
            "AM_OPENMP_MUTEX_TEST_LOCK",
            2,
            "Test lock"),

        EnumVariant(
            "AM_OPENMP_MUTEX_NEST_LOCK",
            3,
            "Nest lock"),

        EnumVariant(
            "AM_OPENMP_MUTEX_TEST_NEST_LOCK",
            4,
            "Test nest lock"),

        EnumVariant(
            "AM_OPENMP_MUTEX_CRITICAL",
            5,
            "Critical"),

        EnumVariant(
            "AM_OPENMP_MUTEX_ATOMIC",
            6,
            "Atomic"),

        EnumVariant(
            "AM_OPENMP_MUTEX_ORDERED",
            7,
            "Ordered")
    ])

################################################################################

am_openmp_sync_hint_t = EnumType(
    name = "enum am_openmp_sync_hint_t",
    entity = "OpenMP sync hint types",
    comment = "Type defines valid OpenMP sync hint types",
    variants = [
        EnumVariant(
            "AM_OPENMP_SYNC_HINT_NONE",
            0x0,
            "None"),

        EnumVariant(
            "AM_OPENMP_SYNC_HINT_UNCONTENDED",
            0x1,
            "Uncontended"),

        EnumVariant(
            "AM_OPENMP_SYNC_HINT_CONTENDED",
            0x2,
            "Contended"),

        EnumVariant(
            "AM_OPENMP_SYNC_HINT_NONSPECULATIVE",
            0x4,
            "Nonspeculative"),

        EnumVariant(
            "AM_OPENMP_SYNC_HINT_SPECULATIVE",
            0x8,
            "Speculative"),

    ])

################################################################################

am_openmp_task_status_t = EnumType(
    name = "enum am_openmp_task_status_t",
    entity = "OpenMP task status",
    comment = "Type defines the valid OpenMP task status",
    variants = [
        EnumVariant(
            "AM_OPENMP_TASK_COMPLETE",
            1,
            "Task complete"),

        EnumVariant(
            "AM_OPENMP_TASK_YIELD",
            2,
            "Task yield"),

        EnumVariant(
            "AM_OPENMP_TASK_CANCEL",
            3,
            "Task cancel"),

        EnumVariant(
            "AM_OPENMP_TASK_DETACH",
            4,
            "Task detach"),

        EnumVariant(
            "AM_OPENMP_TASK_EARLY_FULFILL",
            5,
            "Task early fulfill"),

        EnumVariant(
            "AM_OPENMP_TASK_LATE_FULFILL",
            6,
            "Task late fulfill"),

        EnumVariant(
            "AM_OPENMP_TASK_SWITCH",
            7,
            "Task switch")

    ])

################################################################################

am_openmp_task_flag_t = EnumType(
    name = "enum am_openmp_task_flag_t",
    entity = "An OpenMP task flag",
    comment = "Type defines the valid OpenMP task flags",
    variants = [
        EnumVariant(
            "AM_OPENMP_TASK_INITIAL_UNDEFERRED",
            0x08000001,
            "Task initial undeferred"),

        EnumVariant(
            "AM_OPENMP_TASK_INITIAL_UNTIED",
            0x10000001,
            "Task initial untied"),

        EnumVariant(
            "AM_OPENMP_TASK_INITIAL_FINAL",
            0x20000001,
            "Task initial final"),

        EnumVariant(
            "AM_OPENMP_TASK_INITIAL_MERGEABLE",
            0x40000001,
            "Task initial mergeable"),

        EnumVariant(
            "AM_OPENMP_TASK_INITIAL_MERGED",
            0x80000001,
            "Task initial merged"),

        EnumVariant(
            "AM_OPENMP_TASK_IMPLICIT_UNDEFERRED",
            0x08000002,
            "Task implicit undeferred"),

        EnumVariant(
            "AM_OPENMP_TASK_IMPLICIT_UNTIED",
            0x10000002,
            "Task implicit untied"),

        EnumVariant(
            "AM_OPENMP_TASK_IMPLICIT_FINAL",
            0x20000002,
            "Task implicit final"),

        EnumVariant(
            "AM_OPENMP_TASK_IMPLICIT_MERGEABLE",
            0x40000002,
            "Task implicit mergeable"),

        EnumVariant(
            "AM_OPENMP_TASK_IMPLICIT_MERGED",
            0x80000002,
            "Task implicit merged"),

        EnumVariant(
            "AM_OPENMP_TASK_EXPLICIT_UNDEFERRED",
            0x08000004,
            "Task explicit undeferred"),

        EnumVariant(
            "AM_OPENMP_TASK_EXPLICIT_UNTIED",
            0x10000004,
            "Task explicit untied"),

        EnumVariant(
            "AM_OPENMP_TASK_EXPLICIT_FINAL",
            0x20000004,
            "Task explicit final"),

        EnumVariant(
            "AM_OPENMP_TASK_EXPLICIT_MERGEABLE",
            0x40000004,
            "Task explicit mergeable"),

        EnumVariant(
            "AM_OPENMP_TASK_EXPLICIT_MERGED",
            0x80000004,
            "Task explicit merged"),

        EnumVariant(
            "AM_OPENMP_TASK_TARGET_UNDEFERRED",
            0x08000008,
            "Task target undeferred"),

        EnumVariant(
            "AM_OPENMP_TASK_TARGET_UNTIED",
            0x10000008,
            "Task target untied"),

        EnumVariant(
            "AM_OPENMP_TASK_TARGET_FINAL",
            0x20000008,
            "Task target final"),

        EnumVariant(
            "AM_OPENMP_TASK_TARGET_MERGEABLE",
            0x40000008,
            "Task target mergeable"),

        EnumVariant(
            "AM_OPENMP_TASK_TARGET_MERGED",
            0x80000005,
            "Task target merged")

    ])

################################################################################

am_openmp_cancel_flag_t = EnumType(
    name = "enum am_openmp_cancel_flag_t",
    entity = "An OpenMP cancel flag",
    comment = "Type defines valid OpenMP cancel flags",
    variants = [
        EnumVariant(
            "AM_OPENMP_CANCEL_PARALLEL",
            0x01,
            "Cancel parallel"),

        EnumVariant(
            "AM_OPENMP_CANCEL_SECTIONS",
            0x02,
            "Cancel sections"),

        EnumVariant(
            "AM_OPENMP_CANCEL_LOOP",
            0x04,
            "Cancel loop"),

        EnumVariant(
            "AM_OPENMP_CANCEL_TASKGROUP",
            0x08,
            "Cancel taskgroup"),

        EnumVariant(
            "AM_OPENMP_CANCEL_ACTIVATED",
            0x10,
            "Cancel activated"),

        EnumVariant(
            "AM_OPENMP_CANCEL_DETECTED",
            0x20,
            "Cancel detected"),

        EnumVariant(
            "AM_OPENMP_CANCEL_DISCARDED_TASK",
            0x40,
            "Cancel discarded task")

    ])

################################################################################

am_openmp_for_loop_type = InMemoryCompoundType(
    name = "am_openmp_for_loop_type",
    entity = "OpenMP for loop type",
    comment = "An OpenMP for loop type",
    ident = "am::openmp::for_loop_type",

    fields = FieldList([
        Field(
            name = "source",
            field_type = aftermath.types.in_memory.am_source_location,
            comment = "Location of the source code for this for loop type")]))

################################################################################

am_openmp_for_loop_instance = InMemoryCompoundType(
    name = "am_openmp_for_loop_instance",
    entity = "OpenMP for loop instance",
    comment = "An OpenMP for loop instance",
    ident = "am::openmp::for_loop_instance",

    fields = FieldList([
        Field(
            name = "loop_type",
            field_type = am_openmp_for_loop_type,
            is_pointer = True,
            comment = "Type of this for loop instance"),
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
            comment = "Number of workers that participated in the execution of "\
            "this loop instance")]))

################################################################################

am_openmp_iteration_set = InMemoryCompoundType(
    name = "am_openmp_iteration_set",
    entity = "OpenMP for loop iteration_set",
    comment = "A set of iterations executed by a worker",
    ident = "am::openmp::iteration_set",

    fields = FieldList([
        Field(
            name = "loop_instance",
            field_type = am_openmp_for_loop_instance,
            is_pointer = True,
            comment = "Loop instance this iteration set belongs to"),
        Field(
            name = "lower_bound",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "First value (included) for this iteration set"),
        Field(
            name = "upper_bound",
            field_type = aftermath.types.builtin.uint64_t,
            comment = "Last value (included) for this iteration set")]))

################################################################################

am_openmp_iteration_period = InMemoryCompoundType(
    name = "am_openmp_iteration_period",
    entity = "OpenMP iteration execution period",
    comment = "An OpenMP iteration execution period",
    ident = "am::openmp::iteration_period",

        fields = FieldList([
            Field(
                name = "iteration_set",
                field_type = am_openmp_iteration_set,
                is_pointer = True,
                comment = "Iteration set this period belongs to"),
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the execution")]))

################################################################################

am_openmp_task_type = InMemoryCompoundType(
    name = "am_openmp_task_type",
    entity = "OpenMP task type",
    comment = "An OpenMP task type",
    ident = "am::openmp::task_type",

    fields = FieldList([
        Field(
            name = "name",
            field_type = aftermath.types.base.am_string,
            comment = "Name of this task (e.g., symbol in the executable)"),
        Field(
            name = "source",
            field_type = aftermath.types.in_memory.am_source_location,
            comment = "Location of the source code for this task type")]))

################################################################################

am_openmp_task_instance = InMemoryCompoundType(
    name = "am_openmp_task_instance",
    entity = "OpenMP task instance",
    comment = "An OpenMP task instance",
    ident = "am::openmp::task_instance",

    fields = FieldList([
        Field(
            name = "task_type",
            field_type = am_openmp_task_type,
            is_pointer = True,
            comment = "Type of this task instance")]))

################################################################################

am_openmp_task_period = InMemoryCompoundType(
    name = "am_openmp_task_period",
    entity = "OpenMP task execution period",
    comment = "An OpenMP task execution period",
    ident = "am::openmp::task_period",

        fields = FieldList([
            Field(
                name = "task_instance",
                field_type = am_openmp_task_instance,
                is_pointer = True,
                comment = "Task execution instance this period belongs to"),
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the task execution period")]))

################################################################################

am_openmp_thread = InMemoryCompoundType(
    name = "am_openmp_thread",
    entity = "An OpenMP thread execution interval",
    comment = "An OpenMP thread execution interval",
    ident = "am::openmp::thread",
        fields = FieldList([
            Field(
                name = "kind",
                field_type = am_openmp_thread_t,
                comment = "Type of the thread"),
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the execution")]))

################################################################################

am_openmp_parallel = InMemoryCompoundType(
    name = "am_openmp_parallel",
    entity = "An OpenMP parallel section execution interval",
    comment = "An OpenMP parallel section execution interval",
    ident = "am::openmp::parallel",
        fields = FieldList([
            Field(
                name = "requested_parallelism",
                field_type = aftermath.types.builtin.uint32_t,
                comment = "Number of threads or teams in the region"),
            Field(
                  name = "flags",
                  field_type = am_openmp_parallel_flag_t,
                  comment = "Type of the parallel region"),
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the execution")]))

################################################################################

am_openmp_task_create = InMemoryCompoundType(
    name = "am_openmp_task_create",
    entity = "An OpenMP task create event",
    comment = "An OpenMP task create event",
    ident = "am::openmp::task_create",
        fields = FieldList([
            Field(
                name = "timestamp",
                field_type = aftermath.types.base.am_timestamp_t,
                comment = "Time of the event"),
            Field(
                name = "flags",
                field_type = am_openmp_task_flag_t,
                comment = "Kind of task"),
            Field(
                name = "has_dependence",
                field_type = aftermath.types.base.am_bool_t,
                comment = "True if task has dependences")]))

################################################################################

am_openmp_task_schedule = InMemoryCompoundType(
    name = "am_openmp_task_schedule",
    entity = "An OpenMP task schedule event",
    comment = "An OpenMP task schedule event",
    ident = "am::openmp::task_schedule",
        fields = FieldList([
            Field(
                name = "timestamp",
                field_type = aftermath.types.base.am_timestamp_t,
                comment = "Time of the event"),
            Field(
                name = "prior_task_status",
                field_type = am_openmp_task_status_t,
                comment = "Status of the task at the scheduling point")]))

################################################################################

am_openmp_implicit_task = InMemoryCompoundType(
    name = "am_openmp_implicit_task",
    entity = "An OpenMP implicit execution interval",
    comment = "An OpenMP implicit task execution interval",
    ident = "am::openmp::implicit_task",
        fields = FieldList([
            Field(
                name = "actual_parallelism",
                field_type = aftermath.types.builtin.uint32_t,
                comment = "Number of threads or teams in the region"),
            Field(
                name = "index",
                field_type = aftermath.types.builtin.uint32_t,
                comment = "Thread or team number of calling thread"),
            Field(
                name = "flags",
                field_type = am_openmp_implicit_task_t,
                comment = "Indication if the task is initial or implicit"),
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the execution")]))

################################################################################

am_openmp_sync_region_wait = InMemoryCompoundType(
    name = "am_openmp_sync_region_wait",
    entity = "An OpenMP sync region wait execution interval",
    comment = "An OpenMP sync region wait execution interval",
    ident = "am::openmp::sync_region_wait",
        fields = FieldList([
            Field(
                name = "kind",
                field_type = am_openmp_sync_region_t,
                comment = "Type of the sync region"),
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the execution")]))

################################################################################

am_openmp_mutex_released = InMemoryCompoundType(
    name = "am_openmp_mutex_released",
    entity = "An OpenMP mutex release event",
    comment = "An OpenMP mutex release event",
    ident = "am::openmp::mutex_released",
      fields = FieldList([
          Field(
              name = "timestamp",
              field_type = aftermath.types.base.am_timestamp_t,
              comment = "Time of the event"),
          Field(
              name = "wait_id",
              field_type = aftermath.types.builtin.uint64_t,
              comment = "ID of the object that is being locked"),
          Field(
              name = "kind",
              field_type = am_openmp_mutex_t,
              comment = "Type of the mutual exclusion")]))

################################################################################

am_openmp_work = InMemoryCompoundType(
    name = "am_openmp_work",
    entity = "An OpenMP worksharing region execution interval",
    comment = "An OpenMP worksharing region execution interval",
    ident = "am::openmp::work",
        fields = FieldList([
            Field(
                name = "type",
                field_type = am_openmp_work_t,
                comment = "Type of work"),
            Field(
                name = "count",
                field_type = aftermath.types.builtin.uint64_t,
                comment = "Measure of the quantity involved in work"),
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the execution")]))

################################################################################

am_openmp_dependences = InMemoryCompoundType(
    name = "am_openmp_dependences",
    entity = "An OpenMP dependences event",
    comment = "An OpenMP dependences event",
    ident = "am::openmp::dependences",
        fields = FieldList([
            Field(
                name = "timestamp",
                field_type = aftermath.types.base.am_timestamp_t,
                comment = "Timestamp of the event"),
            Field(
                name = "ndeps",
                field_type = aftermath.types.builtin.uint32_t,
                comment = "Number of dependences")]))

################################################################################

am_openmp_task_dependence = InMemoryCompoundType(
    name = "am_openmp_task_dependence",
    entity = "An OpenMP task dependence event",
    comment = "An OpenMP task dependence event",
    ident = "am::openmp::task_dependence",
        fields = FieldList([
            Field(
                name = "timestamp",
                field_type = aftermath.types.base.am_timestamp_t,
                comment = "Timestamp of the event")]))

################################################################################

am_openmp_master = InMemoryCompoundType(
    name = "am_openmp_master",
    entity = "An OpenMP master region execution interval",
    comment = "An OpenMP master region execution interval",
    ident = "am::openmp::master",
        fields = FieldList([
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the execution")]))

################################################################################

am_openmp_sync_region = InMemoryCompoundType(
    name = "am_openmp_sync_region",
    entity = "an OpenMP sync region execution interval",
    comment = "An OpenMP sync region execution interval",
    ident = "am::openmp::sync_region",
        fields = FieldList([
            Field(
                name = "kind",
                field_type = am_openmp_sync_region_t,
                comment = "Type of the sync region"),
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the execution")]))

################################################################################

am_openmp_lock_init = InMemoryCompoundType(
    name = "am_openmp_lock_init",
    entity = "An OpenMP lock init event",
    comment = "An OpenMP lock init event",
    ident = "am::openmp::lock_init",
      fields = FieldList([
          Field(
              name = "timestamp",
              field_type = aftermath.types.base.am_timestamp_t,
              comment = "Time of the event"),
          Field(
              name = "wait_id",
              field_type = aftermath.types.builtin.uint64_t,
              comment = "ID of the object that is being locked"),
          Field(
              name = "kind",
              field_type = am_openmp_mutex_t,
              comment = "Type of the lock")]))

################################################################################

am_openmp_lock_destroy = InMemoryCompoundType(
    name = "am_openmp_lock_destroy",
    entity = "An OpenMP lock destroy event",
    comment = "An OpenMP lock destroy event",
    ident = "am::openmp::lock_destroy",
      fields = FieldList([
          Field(
              name = "timestamp",
              field_type = aftermath.types.base.am_timestamp_t,
              comment = "Time of the event"),
          Field(
              name = "wait_id",
              field_type = aftermath.types.builtin.uint64_t,
              comment = "ID of the object that is being locked"),
          Field(
              name = "kind",
              field_type = am_openmp_mutex_t,
              comment = "Type of the lock")]))

################################################################################

am_openmp_mutex_acquire = InMemoryCompoundType(
    name = "am_openmp_mutex_acquire",
    entity = "An OpenMP mutex acquire event",
    comment = "An OpenMP mutex acquire event",
    ident = "am::openmp::mutex_acquire",
      fields = FieldList([
          Field(
              name = "timestamp",
              field_type = aftermath.types.base.am_timestamp_t,
              comment = "Time of the event"),
          Field(
              name = "wait_id",
              field_type = aftermath.types.builtin.uint64_t,
              comment = "ID of the object that is being locked"),
          Field(
              name = "kind",
              field_type = am_openmp_mutex_t,
              comment = "Type of the mutual exclusion"),
          Field(
              name = "hint",
              field_type = am_openmp_sync_hint_t,
              comment = "Hint provided at the lock initialization"),
          Field(
              name = "implementation",
              field_type = aftermath.types.builtin.uint32_t,
              comment = "Mechanism chosen by runtime to implement the lock")]))

################################################################################

am_openmp_mutex_acquired = InMemoryCompoundType(
    name = "am_openmp_mutex_acquired",
    entity = "An OpenMP mutex acquire event",
    comment = "An OpenMP mutex acquire event",
    ident = "am::openmp::mutex_acquired",
      fields = FieldList([
          Field(
              name = "timestamp",
              field_type = aftermath.types.base.am_timestamp_t,
              comment = "Time of the event"),
          Field(
              name = "wait_id",
              field_type = aftermath.types.builtin.uint64_t,
              comment = "ID of the object that is being locked"),
          Field(
              name = "kind",
              field_type = am_openmp_mutex_t,
              comment = "Type of the mutual exclusion")]))

################################################################################

am_openmp_nest_lock = InMemoryCompoundType(
    name = "am_openmp_nest_lock",
    entity = "An OpenMP nest lock execution interval",
    comment = "An OpenMP nest lock execution interval",
    ident = "am::openmp::nest_lock",
        fields = FieldList([
            Field(
                name = "wait_id",
                field_type = aftermath.types.builtin.uint64_t,
                comment = "ID of the locked object"),
            Field(
                name = "interval",
                field_type = aftermath.types.in_memory.am_interval,
                comment = "Interval of the execution")]))

################################################################################

am_openmp_flush = InMemoryCompoundType(
    name = "am_openmp_flush",
    entity = "An OpenMP flush event",
    comment = "An OpenMP flush event",
    ident = "am::openmp::flush",
        fields = FieldList([
            Field(
                name = "timestamp",
                field_type = aftermath.types.base.am_timestamp_t,
                comment = "Timestamp of the event")]))

################################################################################

am_openmp_cancel = InMemoryCompoundType(
    name = "am_openmp_cancel",
    entity = "An OpenMP cancel event",
    comment = "An OpenMP cancel event",
    ident = "am::openmp::cancel",
        fields = FieldList([
            Field(
                name = "timestamp",
                field_type = aftermath.types.base.am_timestamp_t,
                comment = "Timestamp of the event"),
            Field(
                name = "flags",
                field_type = am_openmp_cancel_flag_t,
                comment = "Cancellation source and cancelled construct information")
]))

################################################################################

all_types = TypeList([
    am_openmp_sync_region_t,
    am_openmp_work_t,
    am_openmp_thread_t,
    am_openmp_parallel_flag_t,
    am_openmp_implicit_task_t,
    am_openmp_mutex_t,
    am_openmp_sync_hint_t,
    am_openmp_task_status_t,
    am_openmp_task_flag_t,
    am_openmp_cancel_flag_t,
    am_openmp_for_loop_type,
    am_openmp_for_loop_instance,
    am_openmp_iteration_set,
    am_openmp_iteration_period,
    am_openmp_task_type,
    am_openmp_task_instance,
    am_openmp_task_period,
    am_openmp_thread,
    am_openmp_parallel,
    am_openmp_task_create,
    am_openmp_task_schedule,
    am_openmp_implicit_task,
    am_openmp_sync_region_wait,
    am_openmp_mutex_released,
    am_openmp_work,
    am_openmp_dependences,
    am_openmp_task_dependence,
    am_openmp_master,
    am_openmp_sync_region,
    am_openmp_lock_init,
    am_openmp_lock_destroy,
    am_openmp_mutex_acquire,
    am_openmp_mutex_acquired,
    am_openmp_nest_lock,
    am_openmp_flush,
    am_openmp_cancel
])

aftermath.config.addMemTypes(*all_types)
