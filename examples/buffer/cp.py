"""
Buffer: copy
------------

Copy from a vector with mode extacc to another vector with mode extfnc
"""

from os.path import join, dirname
from subprocess import check_call
from vunit import VUnit
from cosim.vhpidirect import VHPIDIRECT


SRC = join(dirname(__file__), "src")

C_OBJ = join(SRC, "cp.o")
# Compile C application to an object
check_call(["gcc", "-fPIC", "-c", join(SRC, "cp.c"), "-o", C_OBJ])

# Enable the external feature for strings
vu = VUnit.from_argv(vhdl_standard="2008", compile_builtins=False)
vu.add_builtins(VHPIDIRECT().bridge({"string": True}))

vu.add_library("lib").add_source_files(join(SRC, "tb_extcp_*.vhd"))

# Add the C object to the elaboration of GHDL
vu.set_sim_option("ghdl.elab_flags", ["-Wl," + C_OBJ])

vu.main()
