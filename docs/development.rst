Development
###########

.. _dev_env:

Testing environment
===================

On the one hand, the Python sources in this repo depend on a specific branch/fork of VUnit: `dbhi/vunit@cosim-dev <https://github.com/dbhi/vunit/tree/cosim-dev>`_.
On the other hand, this Python package is not published to PyPI yet. Therefore, the suggested approach to run the
examples/tests is:

* Clone VUnit to ``../vunit```, relative to the root of this repo.
* Use ``tox`` to run the examples/tests.
* Optionally, set ``PYTHONPATH=/path/to/vunit:/path/to/cosim`` and install ``requirements.txt`` with ``pip``.

Demos
=====

The following is a list of demos that we aim to provide:

* Sharing memory between VHDL and a SW application.

    * Shared ``string``, ``byte_vector``, ``integer_vector``.
    * Shared virtual ``memory`` model.
        * Co-simulation of the HW and SW in a Xilinx SoC.
        * Co-simulation of Vivado HLS projects exported to VHDL.
* Python callbacks to do things like using a regexp from VHDL.
* Use message passing/queues to interact with an external verification component.

    * Octave/Matlab *monitor* plotting transactions/samples rather than using simple text logging.
* Integration with the external API of specific tools:

    * Use a simulation model as a *regular* function/feature in Octave.
    * Digital/Analog simulation with Xyce.
* Performing a *multicore* simulation by passing data between simulators using VUnit queues.
    * Multiple GHDL simulations.
    * VHDL simulations with GHDL, along with other HDL using different tools. For example verilator for Verilog, or SystemVerilog DPI.

Tasks
=====

Done
----

* ``add sim_opt 'ghdl.elab_e' to run 'ghdl -e' instead of 'ghdl --elab-run --no-run'`` (#467)
* ``fix: nonzero return values should produce a fail`` (#469)
* ``Add option 'overwrite' to set_sim_option`` (#471)
* ``Add method simulator_check to test/common.py`` (#488)
* ``style: merge data_types/src/types/* into data_types/src/types.vhd`` (#543)
* ``[1/3] Add external models to string_ptr; add byte_vector* as an alias`` (#507)
* ``[2/3] Add external modes to string_ptr and integer_vector_ptr (through VHPIDIRECT)`` (#476)
* ``post_run: add report data to 'Results' object/class`` (#578)

Work in progress
----------------

* vunit:

    * Remove VHPIDIRECT related sources (implementation and example) from VUnit/vunit (#606)
    * Add section 'External VHDL API' to the docs of VUnit (#606)
* cosim:

    * Fix/complete ``setup.py``
    * Add unit tests
    * Add bridge ``ghdlc``
    * Fix Xyce bridge/examples to null-terminate unconstrained strings (fat pointers); use ghdlc
    * Add example "py callback"
    * Add example "Octave function"
    * Complete "dummy" bridge

To do
-----

* Add external queues
* Add external actors
* ``[3/3] Support up to N external memory models`` (#470)
* Xyce

    * Extend API to accept arrV and arrT for more than one DAC
    * Check the bit-width features of ADCs.
    * Drive the DACs from some concurrent VHDL process, rather than building the vector synthetically.

        Is anyone aware of (or is willing to contribute) an open source toy example of a digital module being
        co-simulated with a SPICE model? I think that the examples in XyceCInterface are artificial or synthetic
        because there is no synchronization requirements between the loop that handles Xyce in VHDL and other
        concurrent processes/modules. Therefore, the advantages of having init, run, read and close in separate
        functions/procedures are not seen.


Backlog
-------

* Allow Stream VCs to read/write from external buffers
* Visualizing waveforms (GtkWave)

References
==========

* VUnit/vunit

    * @umarcor

        * ``Support up to N memory models for verification components`` (#462)
        * ``[PoC] Interfacing GHDL to other languages: declaration of foreign C functions`` (#465)
        * ``[GHDL] run.py --elaborate should not execute the design`` (#466)
        * ``Programatic procedure to retrieve the output path of a test`` (#478)
        * ``Add method 'set_objects'`` (#481)
        * ``Verification components make GHDL crash when too large objects are allocated`` (#494)
        * ``Tests that log heavily are killed due to memory starvation`` (#498)
        * ``VUnitCoSim`` (#568)
        * ``Do not add builtins by default`` (#559)
        * ``Get run CLI args from Results`` (#579)
        * ``Save GHDL args to JSON file`` (#581)
    * @bradleyharden

        * ``Update stream pkg`` (#513)
        * ``New feature proposal: a codified dynamic type system`` (#521)
        * ``New queue implementation`` (#522)
        * ``Discussion on ptr_pkg structure and naming`` (#583)
        * ``Proposal for a more comprehensive approach to VUnit co-simulation`` (#603)
* ghdl/ghdl

    * ``Work around current limitations of 'ghdl --list-link'`` (#640)
    * ``Unused, undefined foreign function makes elaboration fail`` (#793)
    * ``VHPIDIRECT access to unconstrained array`` (#797)
    * ``Allow to provide a custom 'grt.ver' file as an argument to 'ghdl -e'`` (#800)
    * ``co-simulation: dynamically loading designs/artifacts built with ghdl`` (#803)
    * ``Add configure option '--default-pic' to enable '-fPIC' by default`` (#805)
    * ``sysmalloc assertion error: inconsistent size limit depending on the backend`` (#822)
    * ``Digital/Analog simulation`` (#1052)
