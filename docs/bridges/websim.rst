.. _bridges:websim:

WebSim
######

.. autoclass:: cosim.websim.WebSim()


Flask and Vue
=============

Regarding clock signal management, I agree with you, it is powerful but not approachable to new users. It is specially confusing that a single array is used for multiple unrelated params. However, it is a workaround, not a definite solution. Precisely, as commented in ghdl/ghdl#803, when using VHPIDIRECT, GHDL does not currently support features such as ghdl_runfor(10) or ghdl_rununtil(100). Therefore, clk_pkg (doing clock gating) is the solution I found because I was not capable enough to extend GHDL. It is loosely based on @hackin's work, in the sense that it allows to select between diffrent clock frequencies to slow down or accelerate the simulation; but, in practice, all it does is simulate lots of clock cycles where eveything remains idle. From a CPU usage perspective, it would be really handy to have ghdl_stop, ghdl_continue, etc.
