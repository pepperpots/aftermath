nobase_include_HEADERS = \
	aftermath/trace/arch.h \
	aftermath/trace/base_types.h \
	aftermath/trace/buffered_event_collection.h \
	aftermath/trace/buffered_trace.h \
	aftermath/trace/convert.h \
	aftermath/trace/on_disk_default_type_ids.h \
	aftermath/trace/on_disk_structs.h \
	aftermath/trace/on_disk_write_to_buffer.h \
	aftermath/trace/safe_alloc.h \
	aftermath/trace/simple_hierarchy.h \
	aftermath/trace/state_stack.h \
	aftermath/trace/timestamp.h \
	aftermath/trace/tsc.h \
	aftermath/trace/write_buffer.h

aftermath/trace/base_types.h: $(top_builddir)/src/base_types.h
	$(MKDIR_P) aftermath/trace
	ln -srf $< $@

aftermath/trace/convert.h: $(top_builddir)/src/convert.h
	$(MKDIR_P) aftermath/trace
	ln -srf $< $@

aftermath/trace/on_disk_default_type_ids.h: $(top_builddir)/src/on_disk_default_type_ids.h
	$(MKDIR_P) aftermath/trace
	ln -srf $< $@

aftermath/trace/on_disk_structs.h: $(top_builddir)/src/on_disk_structs.h
	$(MKDIR_P) aftermath/trace
	ln -srf $< $@

aftermath/trace/on_disk_write_to_buffer.h: $(top_builddir)/src/on_disk_write_to_buffer.h
	$(MKDIR_P) aftermath/trace
	ln -srf $< $@

aftermath/trace/safe_alloc.h: $(top_builddir)/src/safe_alloc.h
	$(MKDIR_P) aftermath/trace
	ln -srf $< $@

aftermath/trace/write_buffer.h: $(top_builddir)/src/write_buffer.h
	$(MKDIR_P) aftermath/trace
	ln -srf $< $@
