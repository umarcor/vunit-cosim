.. _bridges:xyce:

Xyce
####

.. attention:: Both this bridge and :ref:`bridges:ghdlc` are in early development and are expected to change frequently
  in the following weeks/months.

..

    "Xyce is an open source, SPICE-compatible, high-performance analog circuit simulator, capable of solving extremely
    large circuit problems by supporting large-scale parallel computing platforms.
    It also supports serial execution on all common desktop platforms, and small-scale parallel runs on Unix-like
    systems".

    -- https://xyce.sandia.gov

Xyce provides two mechanisms for external tools/simulation codes to use Xyce:

**General External Interface (GenExt)**
  Comprenhensive interface aimed at developers who wish to couple other external codes written in C++. See AppNote
  `Coupled Simulation with the Xyce General External Interface <https://xyce.sandia.gov/documentation/AppNote-GenExt.pdf>`_.

**Mixed Signal Interface (MixedSignal)**
  Interface to use Xyce as a shared library/object, either from C/C++ codes or through Python's ``ctypes``. Class
  `XyceCInterface <https://github.com/Xyce/Xyce/tree/master/utils/XyceCInterface>`_ provides methods to work with a
  pointer to the topmost object in a Xyce simulation; and corresponding Python bindings are provided. See AppNote
  `Mixed Signal Simulation with Xyce 6.11 <https://xyce.sandia.gov/downloads/_assets/documents/AppNote-MixedSignal_6.11.pdf>`_
  (or `previous version <https://www.osti.gov/biblio/1483152-application-note-mixed-signal-simulation-xyce>`_.

Regarding integration with GHDL, both interfaces might be suitable for different targets. On the one hand, GenExt
might allow GHDL to provide VHDL-AMS support to Xyce, as the interface allows Xyce to execute callbacks defined in
a foreign tool. On the other hand, MixedSignal provides a higher abstraction level mechanism, which allows for
co-execution of otherwise atomic modules.

This bridge provides a simplified C API based on the Mixed Signal Interface, along with VHDL bindings for it:

.. toctree::
   :maxdepth: 2
   :glob:

   ./xyce/*

The main differences between the simplified API and ``XyceCInterface`` are:

* ``XyceCInterface`` is designed to use a variable of type ``void**`` as the handler of a simulation object.
  Unfortunately, such a type is not cleanly mapped to VHDL. Instead, identifiers of type ``string`` are used in VHDL,
  and this bridge provides a storage mechanism to keep track of the correspondence.
* VHDL's type system, which is based on Ada's, avoids the requirement to use additional function/procedure parameters
  to pass array constraints.

This simplified API is targeted at developers who use VHDL as the main language to orchestrate the co-execution. Hence,
existence of C sources is expected to be transparent. :ref:`xyce_pkg` provides the public API for end-users. Note that
it is possible to run multiple analog simulations by handling multiple instances of ``xyce_t`` from different VHDL
modules. Find usage examples in :ref:`xyce_egs`.

.. note:: The C implementation is based on bridge :ref:`bridges:ghdlc`; hence, it is specific to GHDL. Nevertheless,
  it should be straightforward to adapt it for simulators that support FLI. Contributions are welcome!

.. note:: Although the C implementation is not expected to be used directly, it can be useful to access it when the
  simulation executable is to be dynamically loaded and started. This is, for example, how bridge :ref:`bridges:websim`
  works. In these contexts, it might be handy to initialize and close the Xyce simulations from Python, while VHDL is
  used to run steps and handle I/O. Even though specific examples are not available yet, this is a supported use case.

.. note:: The current implementation of the storage mechanism is likely to be replaced as a result of the discussions
  in `VUnit/vunit#603 <https://github.com/VUnit/vunit/issues/603>`_. Such a replacement should be transparent to
  end-users. However, since this is a very experimental project yet, note that disruptive changes might be required.

GUI features
============

Neither GHDL nor Xyce provide built-in features for graphical schematic capture or plotting/viewing of simulation
signals. Fortunately, both of them support generating traces that can be visualised with specific free and open source
tools. If GHDL is used to generate waveforms, which might include digitalised analog signals, `GTKWave <http://gtkwave.sourceforge.net/>`_
can be used. Note that GTKWave provides an *analog* visualization type. Regarding Xyce, see AppNote `Using Open Source Schematic Capture Tools With Xyce <https://xyce.sandia.gov/downloads/_assets/documents/AppNote-GedaAndXyce.pdf>`_ for information about schematic capture and plotting tools.

Analog modelling
================

Xyce supports Verilog-A through, Automatic Device Model Synthesizer (ADMS), an open-source translator. Xyce/ADMS is a
set of XML templates for use with ADMS, which allows to emit C++ for a device model. See `Xyce/ADMS Users Guide <https://xyce.sandia.gov/documentation/XyceADMSGuide.html>`_.

Unfortunately, for VHDL-AMS there is neither built-in support nor a similar translation tool (yet). See related
discussion in `ghdl/ghdl#1052 <https://github.com/ghdl/ghdl/issues/1052>`_.
