.. _bridges:

Bridges
#######

Bridges contain VHDL packages, matching C headers and/or Python classes. Each bridge provides the glue logic between a VHDL API and some other API in VHDL and/or C (bindings).

.. NOTE:: `GHDL <https://github.com/ghdl/ghdl>`_, the only free and open source simulator for VHDL supported by VUnit, features VPI and a limited subset of VHPI named VHPIDIRECT. Since co-simulation of VHDL and Python through VPI is already supported and documented by `cocotb <https://github.com/cocotb/cocotb>`_, this project is mostly focused on VHPIDIRECT.

Available modules:

:ref:`bridges:ghdlc`
    C bindings for GHDL's VHPIDIRECT *fat pointers* (see `ghdl.readthedocs.io: Interfacing to other languages <https://ghdl.readthedocs.io/en/latest/using/Foreign.html>`_).

:ref:`bridges:vhpidirect`
    C bindings for VUnit's external VHDL API (see :ref:`vunit:data_types_library`).

:ref:`bridges:websim`
    Web GUI to provide custom interactive visualisations, based on bridge :ref:`bridges:vhpidirect`.

:ref:`bridges:xyce`
    Simplified version of `XyceCInterface <https://github.com/Xyce/Xyce/tree/master/utils/XyceCInterface>`_ and VHDL bindings, based on bridge :ref:`bridges:ghdlc`.

:ref:`bridges:grpc`
    Hooks for bridge :ref:`bridges:vhpidirect` to interact with remote *agents* or *virtual memories* through `gRPC Remote Procedure Calls <https://grpc.io/>`_ (see `dbhi/grpc <https://github.com/dbhi/grpc>`_).

:ref:`bridges:fli`
    Compatibility with :ref:`bridges:vhpidirect` for simulators that support FLI.

.. toctree::
   :maxdepth: 2
   :hidden:
   :glob:

   ghdlc
   vhpidirect
   websim
   xyce
   grpc
   fli
