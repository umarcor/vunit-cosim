"""
Mixed-Signal (Digital/Analog) simulation with GHDL and Xyce through VHPIDIRECT
------------------------------------------------------------------------------

Description...
"""

from os.path import join, dirname
from subprocess import check_call
from vunit import VUnit


vu = VUnit.from_argv(vhdl_standard="2008")

SRC = join(dirname(__file__), "src", "runACircuit")

# Compile C application to an object
C_OBJ = join(SRC, "main.o")

check_call(["gcc", "-fPIC", "-I%s" % SRC, "-c", join(SRC, "main.c"), "-o", C_OBJ])

vu.add_library("lib").add_source_files(join(SRC, "*.vhd"))
vu.set_sim_option("ghdl.elab_flags", ["-Wl," + C_OBJ, "-Wl,-lxycecinterface"])

vu.main()
