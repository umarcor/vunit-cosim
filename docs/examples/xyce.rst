.. _xyce_egs:

Xyce
####

Xyce developers provide two versions of the same three examples (`utils/XyceCInterface <https://github.com/Xyce/Xyce/tree/master/utils/XyceCInterface>`_):

**Python_examples**
  Use ``xyce_interface.py`` to manage a Xyce simulation from Python. No HDL is used.

**VPI_examples**
  Use Verilog to trigger the Xyce simulations. However, all the logic is implemented in C.
  As explained in Section 7 of `Mixed Signal Simulation with Xyce 6.11 <https://xyce.sandia.gov/downloads/_assets/documents/AppNote-MixedSignal_6.11.pdf>`_:

      "The primary issue with the VPI capability is the lack of standards compliance. The example (...) uses the C++
      features of the ``XyceCInterface`` directly. Wrapper functions, that only use ANSI C and the native PLI
      data-types in their function calls, still need to beimplemented."

Our examples are an adaptation of the ones from Xyce's repo, to use VHDL through VHPIDIRECT bindings, instead of
Verilog. At the same time, the management of the execution flow is handled from VHDL, so that developers don't need
to code in C/C++.

On the one hand, `runACircuit <https://github.com/umarcor/cosim/tree/master/examples/xyce/src/runACircuit>`_ is the
most simple approach, where a single VHDL test bench and a single C file are used.

On the other hand, `runACircuitInSteps <https://github.com/umarcor/cosim/blob/master/examples/xyce/src/runACircuitInSteps>`_
and `runWithDACs <https://github.com/umarcor/cosim/tree/master/examples/xyce/src/runWithDACs>`_ are based on bridge
:ref:`bridges:xyce`. That is, the simplified VHDL API (types ``xyce_t`` and ``xyce_arr_t``) described in
:ref:`xyce_pkg` is used. Note that this allows to control the execution flow from VHDL, unlike ``runACircuit`` or the
versions in Xyce's repo; where all the relevant logic is implemented in C and HDL is only used as a trigger.

.. NOTE:: Neither ``runACircuit`` nor ``runACircuitInSteps`` are proper digital/analog co-simulations, because there
  is no communication between digital and analog domains. These examples are provided for illustrative purposes only.
  The former shows how to trigger a Xyce simulation from VHDL through VHPIDIRECT; and the latter shows how to handle
  the execution steps from VHDL.

.. NOTE:: As explained in AppNote `Digital/Analog Cosimulation using CocoTB and Xyce <https://www.osti.gov/biblio/1488489-digital-analog-cosimulation-using-cocotb-xyce>`_,
  cocotb can be used to co-simulate GHDL and Xyce through VPI, as an alternative to using VHPIDIRECT. Such an approach
  might be preferred when Python is to be used as the orchestrator. Moreover, the report introduces an interesting use
  case where digital and analog versions of the same module are used. Unfortunately, sources of that example are not
  publicly available.

Usage
=====

Two VUnit scripts are provided: `run_minimal.py <https://github.com/umarcor/cosim/blob/master/examples/xyce/run_minimal.py>`_
builds and executes ``runACircuit``, and `run.py <https://github.com/umarcor/cosim/blob/master/examples/xyce/run.py>`_
takes care of the others. See :ref:`dev_env` for information about how to install the dependencies.

.. NOTE:: Currently, XyceCInterface requires circuit models to be passed as a path to a file. Providing a pointer to a
  string or using ``stdin`` is not supported yet. However, the feature has been requested to developers of Xyce and
  it might be available in future versions.

.. NOTE:: The YADC and YDAC device models in Xyce, which are used in example ``runWithDACs``, are not realistic device
  models. Important issues like rise/fall times and drive/sink currents are not modelled. Hence, they are adequate
  for artificial transferring signal values between digital and analog domain, but those models need to be improved.
  See Section 5 of `Mixed Signal Simulation with Xyce 6.11 <https://xyce.sandia.gov/downloads/_assets/documents/AppNote-MixedSignal_6.11.pdf>`_.

.. NOTE:: See Section 7.1 of `Mixed Signal Simulation with Xyce 6.11 <https://xyce.sandia.gov/downloads/_assets/documents/AppNote-MixedSignal_6.11.pdf>`_
  for information regarding *known issues with coordinated timestepping*.

Container
=========

Since Xyce's Mixed Signal Interface is an experimental feature yet, specific versions of the tools are required, and
configuration options need to be chosen to produce shared libraries. In order to make it easier to get a working
environment where these examples can be tried, a `Dockerfile <https://github.com/umarcor/cosim/blob/master/examples/xyce/Dockerfile>`_
is provided. Moreover, a Docker image is built periodically and published as `umarcor/cosim:xyce <https://hub.docker.com/r/umarcor/cosim/tags>`_.
The image is based on `ghdl/vunit:llvm-master <https://github.com/ghdl/docker>`_ and includes GHDL (``master``),
Python 3, VUnit (``master``) and Xyce. However, note that, currently, a specific branch of VUnit is required (see
:ref:`dev_env`).
