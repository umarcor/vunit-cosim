.. _cocotb:

cocotb
######

- https://github.com/cocotb/cocotb/pull/1051
  - https://external-builds.readthedocs.io/html/cocotb/1051/examples.html#mixed-signal-analog-digital
- https://github.com/cocotb/cocotb/pull/1217
- https://github.com/cocotb/cocotb/issues/1210

Nonetheless, the target users/developers of cocotb are not the same as those of VUnit, since the former is Python-centric, while VUnit is focused on providing a rich set of VHDL features.

The main difference between VUnit/VHPIDIRECT and cocotb/VPI is the focus on VHDL or Python, respectively. The concept of "external VHDL API" is interesting precisely because someone might want to plug VUnit's queues to Python by hijacking the body of the default "dummy" templates with VPI. This would apply to verification components too. There is no need to duplicate them in Python if cocotb allows to manage VUnit's VHDL components.

I might not want to control the execution from Python, or to use Python at all. VUnit and GHDL allow to generate a binary that you can later use/call from any other tool. I might just want to use GHDL as a function in Octave, to use the workspace to allocate, build and process test data. I believe that cocotb provides access to every and each signal in the design, but it needs the Python interface at runtime. Actually, that's the main feature: cocotb.readthedocs.io/en/latest/introduction.html#overview

The question should not be about who controls the execution flow, but about what data you want to share. Users that want to share/transfer test data/messages will find VUnit to be a better fit (it is a test runner). Users that want to inspect specific signals in the design to do fault injection or any other targeted test, will find cocotb to be better (because it is a Python based VPI client).