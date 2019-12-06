.. _bridges:vhpidirect:

VHPIDIRECT
##########

This is a bridge between VUnit's external VHDL API and C, through GHDL's VHPIDIRECT
features (see :ref:`ghdl:USING:Foreign`). A Python class (named ``VHPIDIRECT``) provides
helper functions to (re)use a C API (``vhpidirect_user.h``, ``grt.ver`` and/or
``stubs.c``) with VUnit's ``add_builtins`` and ``set_sim_option("ghdl.elab_flags", ...)``.
Examples of how to use this bridge are shown in :ref:`examples:vhpidirect`.

.. NOTE:: In the latest stable release of GHDL (v0.36), mcode backend does not support
  VHPIDIRECT. Moreover, using LLVM or GCC backends is suggested, as these generate
  executable files and are supported in a wider range of platforms.

.. NOTE:: For GHDL to generate PIE executable files, it needs to be configured with option
  ``--default-pic``. Moreover, loading ELF binaries dynamically is a *hackish* procedure
  that takes advantage of PIE ELF binaries and shared libraries having a common structure
  on GNU/Linux. However, this does not work on Windows (see `ghdl/ghdl#803 <https://github.com/ghdl/ghdl/issues/803>`_).
  As a result:

  * GHDL needs to be enhanced to allow building designs as shared libraries instead of
    executable binaries (see `ghdl/ghdl#800 <https://github.com/ghdl/ghdl/issues/800>`_).
  * or, build features in VUnit need to be extended to use ``ghdl --bind``,
    ``ghdl --list-link`` and gcc/clang in order to generate a ``*.dll`` on Windows.
  * or, a helper method need to be added to this class to use gcc/clang.

Implementation details

``vhpidirect_user.h`` is the C implementation of the interface. It uses an array of
pointers (``uint8_t *D[256];``) to keep a reference to all the buffers that are shared
between C and VHDL.

.. NOTE:: Users that need to share more than 256 pointers, can and should include their
  own copy of this header file. Actually, in example :ref:`examples:copy` the provided
  header file is not used. Instead, functions are implemented in ``main.c``.

``stubs.c`` is to be used when multiple tests are handled in a single ``run.py`` file
(i.e. by the same VUnit object), but some of them do NOT use external modes. Since
builtins are compiled once only, every test (binary) needs to have placeholders for the C
functions that VHDL expects to use. ``stubs.c`` provides *dummy* definitions of the C API.
Note that these will produce errors if executed at runtime.

``grt.ver`` is to be used when the ELF file built with GHDL is to be loaded dynamically.
By default, GHDL hides most of the global symbols. Providing this file as an elaboration
option ensures that the functions of the C API are visible. Script ``corun.py`` in example
:ref:`examples:buffer` shows how to dynamically load and execute the simulation from Python.

extfnc mode
-----------

As explained in :ref:`vunit:data_types_library:external`, mode *extfnc* is expected to be
used when data is not available in the same memory space as the VHDL simulation. For
example, when libraries such as `gRPC <https://grpc.io/>`_, `ZeroMQ <https://zeromq.org/>`_
or `netpp <https://section5.ch/index.php/netpp/>`_ are used to co-execute simulations in
remote targets. Hence, ``write_*/read_*`` function bodies provided in ``vhpidirect_user.h``
are for testing purposes. In practice, developers willing to use this mode are expected
to provide their own custom header file.

Note that this is the difference between *extfnc* and *extacc*: with *extacc*, there is
no explicit callback when VHDL modifies a value in a shared buffer. Conversely, with
*extfnc*, VHDL cannot modify a value without a callback.


.. vhpidirect_user.h is the default C implementation of the interface. Users, specially those that are new, are expected to include it and use *D[256] as the shared type between C and VHDL. However, this header file is not used in VUnit's codebase at all. I.e., users needs to explicitly include it in their main.c file. This is because I/we expect intermediate and advanced users to require custom versions of it. Say you want to share 768 pointers between C and VHDL. You copy the header file along with your own testing VHDL sources, you modify whatever, and you import it instead of the default. All you need to guarantee is that the functions that are used in external_* VHDL sources (/vunit/vhdl/data_types/src/external@master ) are actually implemented in your main or in the headers you import.

.. Furthermore, if you have multiple tests in a run.py file (i.e. handled by the same VUnit object), and some of them do not use external modes, you do need to provide some dummy C code to those. /vunit/vhdl/data_types/src/external/ghdl/stubs.c@master is provided for that purpose. It is another default implementation of the C interface. This is a very particular implementation that allows to compile VHDL designs but which will produce errors if external modes are used.

.. Last, although reference VHDL packages are provided in /vunit/vhdl/data_types/src/external@master , it is possible to provide custom versions through vu.add_builtins. This feature is mostly meant for users that want to implement external modes with other simulators (say QuestaSim and FLI).

.. These external modes are currently only supported with GHDL, with some limitations:

.. With the latest stable version of (v0.36), it does not work with mcode backend. It is fixed in master, but we need to wait until v0.37 is released in order to update the CI jobs and test it with any backend. I think that Tristan wants to wait until synth features are out of beta to do so.
        Even with v0.37, VUnitCoSim Python scripts require LLVM or GCC. This is because the execution flow expects an executable binary to be created. In order to use mcode, this needs to be changed. The same might apply to other simulators which support VHPIDIRECT/FLI.
    Apart from that, although C-VHDL co-simulation will work on either GNU/Linux or Windows, the Python cosim.py

.. example in #568 will not work on Windows.

.. The build option introduced in #467 produces an executable binary, not a shared library. The fact that it can be dynamically loaded from Python (or C) is a hackish effect of using --default-pic when building GHDL; which produces ELF binaries on GNU/Linux. On windows, a binary cannot be loaded as if it was a DLL. See ghdl/ghdl#803.
    Fortunately, it is possible to use gcc instead of ghdl -e to generate a proper shared library on either GNU/Linux or Windows. Unfortunately, Tristan is not fond of adding a -shared option (see ghdl/ghdl#800 (comment)) and using gcc from sim_if.ghdl would require to modify it to an extent that might not be acceptable. @kraigher has previsouly been reasonably hesitant to introducing too many GHDL-specific features in VUnit's codease (#481).

.. the shared buffer between C and VHDL is defined as *D[256].

.. Currently, users are expected to copy and modify vhpidirect_user.h to replace extfnc value get/set functions with their own implementation, which would be gRPC calls. This is precisely the difference between extacc and extfnc. With extacc there is no explicit C callback when VHDL modifies a value in a shared buffer. However, with extfnc VHDL cannot modify a value in a shared buffer without using a callback.

.. Actually, using these functions as in vhpidirect_user.h is stupid (not useful). When extacc is suitable, users should use it. When it is not, they should copy the reference header file and adapt it. Examples of these advanced uses are not contributed yet.

.. constant eids: byte_vector_ptr_t := new_byte_vector_ptr( 256, extacc, 0);
  constant eid_lengths: integer_vector_ptr_t := new_integer_vector_ptr( 256, extacc, 1);

.. -- external modes enabled
  constant eid_first_vector := natural := eids(0);
  constant eid_fifo_input   := natural := eids(1);
  constant eid_RAM_content  := natural := eids(2);

.. -- external modes disabled
  constant eid_first_vector := natural := -1;
  constant eid_fifo_input   := natural := -1;
  constant eid_RAM_content  := natural := -1;

.. D[0] = (uint8_t *) malloc(3);
  D[0][0] = x; //assign id that will be accessed from VHDL as eid_first_vector
  D[0][1] = y; //assign id that will be accessed from VHDL as eid_fifo_input
  D[0][2] = z; //assign id that will be accessed from VHDL as eid_RAM_content

.. D[1] = (uint8_t *) malloc(256*4);
  D[1][0] = xx; //assign maximum length of eid_first_vector
  D[1][1] = yy; //assign maximum length of eid_fifo_input
  D[1][2] = zz; //assign maximum length of eid_RAM_content

.. D[x] = (uint8_t *) malloc(xx * sizeof(X_TYPE));
  D[y] = (uint8_t *) malloc(yy * sizeof(Y_TYPE));
  D[z] = (uint8_t *) malloc(zz * sizeof(Z_TYPE));

Python interface
================

.. autoclass:: cosim.vhpidirect.VHPIDIRECT()
