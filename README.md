# Aftermath

An extensible profiling and performance analysis tool with support for several programming models.

![Screenshot](/docs/screenshot.png)

This is an independently maintained* fork of the original [Aftermath](https://aftermath-tracing.com/)
that features the most up-to-date changes and is under an active development.

(*) With the original maintainer's blessing

## Download and Build

To obtain the code for the pre-release, please clone as follows:                                                                                                                                    

```
git clone https://github.com/pepperpots/aftermath.git
```

There are also a few dependencies that need to be installed on your build system. On
Debian-based systems, it should be sufficient to install the following packages:

```
sudo apt-get install build-essential autoconf automake git libcairo2 \
  libcairo2-dev libglib2.0-dev libtool pkg-config qt5-default qtbase5-dev \
  qtbase5-dev-tools libqt5core5a libqt5gui5 libqt5widgets5 python-jinja2
```

Once all the dependencies are installed, you may proceed with the configuration and installation
of Aftermath itself. The project is split into multiple sub-projects:

* Libaftermath-core (data model, algorithms for processing, etc.)
* Libaftermath-render (visualization of trace data)
* Libaftermath-trace (instrumentation library)
* Aftermath QT GUI (graphical user interface)
* Aftermath-dump (dumps traces into human-readable text)
* Aftermath-convert (converts traces in the old trace format to the new format)
* Language bindingsfor Python

Each of the sub-projects is located in a folder at the project root and can be built
separately. For covenience, there is a script at the root, which invokes the configuration and
compilation steps in the right order.

```
cd aftermath
./build-all.sh --local-install --env=env.sh -j N
```

where N is the number of cores of your machine. The `--local-install` option causes everything to
be installed directly into a folder `install` at the project root. The option `--env=env.sh`
causes the script to generate a shell script that automatically sets `PATH` and
`LD_LIBRARY_PATH`. In order to set these environment variables for the current shell, the script
needs to be "sourced", e.g., by executing:

```
source env.sh
```

Alternatively, you can set the environment variables manually, as indicated by the build script
at the end of the installation.

## Usage

To run Aftermath, simply invoke the aftermath binary with the trace file as the argument, i.e.,

```
aftermath tracefile.ost
```

This opens the default profile for the Aftermath GUI. You can specify a different profile by
using the `-p` option when invoking aftermath. For example, if you wanted to try the OpenMP
profile, you would do:

```
aftermath -p openmp tracefile.ost
```

Currently, there are 5 profiles:
* `default`
* `telamon` (Telamon code optimizer)
* `telamon-candidate-stats` (same as telamon, but with additional statistics)
* `openmp` (basic OpenMP support)
* `tensorflow` (very basic TensorFlow support)

## Python bindings

In addition to the Aftermath GUI and the C libraries, there is also a preliminary set of Python
bindings for Libaftermath-core. These bindings are automatically generated from the Aftermath
type system.

In addition to the previously required packages, you'll also need `python-cffi` and `pip`. The
configure scripts should properly check for this:

```
sudo apt-get install python-pip python-cffi
```

The generation of Python bindings must explicitly be enabled by passing the option
`--enable-python` to the build script:

```
./build-all.sh --local-install --enable-python --env=env.sh -j N
```

If everything compiles and installs correctly, you'll be able to use the a new Python package
named aftermath.core. Make sure that Python finds it by setting `PYTHONPATH` correctly. This can
be done by sourcing the script with environment variables generated above:

```
source env.sh
```

As a quick test, you could list all states and count how many state events each event collection
has:

```python
#!/usr/bin/env python

import aftermath.core as ac

t = ac.Trace("/path/to/trace/file.ost")

state_arr = t.getTraceArray("am::core::state_description")

print("States:")

for state in state_arr:
    print(" " + state.name)
    
for event_collection in t.getEventCollections():
    try:
        state_events = event_collection.getEventArray("am::core::state_event")
        num_state_events = len(state_events)
    except:
        num_state_events = 0
        
    print("Event collection has {} state events".format(num_state_events))
```

## Credits

The build instructions have been directly adapted from the original [Aftermath website](https://www.aftermath-tracing.com/prerelease).

