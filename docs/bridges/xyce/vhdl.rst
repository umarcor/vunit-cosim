Xyce: VHDL interface
####################

.. _xyce_pkg:

*xyce* package
--------------

Provides the user interface to co-execute Xyce simulations from VHDL. See usage :ref:`examples <xyce_egs>`.

.. literalinclude:: ../../../cosim/xyce/vhdl/xyce_pkg.vhd
   :language: vhdl

.. _xyce_xhdl_pkg:

*xyce_xhdl* package
-------------------

Provides bindings between VHDL and a foreign language through VHPIDIRECT. This source needs to be included in the design, but users are not expected to use it explicitly.

.. literalinclude:: ../../../cosim/xyce/vhdl/xyce_xhdl_pkg.vhd
   :language: vhdl
