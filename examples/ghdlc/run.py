"""
Mixed-Signal (Digital/Analog) simulation with GHDL and Xyce through VHPIDIRECT
"""

from os.path import join, dirname
from subprocess import check_call
from vunit import VUnit
from cosim.ghdlc import GHDLC


vu = VUnit.from_argv(vhdl_standard="2008")

src_path = join(dirname(__file__), "src")

# Compile C application to an object
c_obj = join(src_path, "main.o")

check_call(
    ["gcc", "-fPIC"] +
    ["-I%s" % x for x in GHDLC().include] +
    [
        "-c",
        join(dirname(__file__), "src", "main.c"),
        "-o",
        c_obj,
    ]
)

lib = vu.add_library("lib")
lib.add_source_files(join(src_path, "*.vhd"))

for tb in lib.get_test_benches(pattern="*", allow_empty=False):
    tb.set_sim_option("ghdl.elab_flags", ["-Wl," + c_obj])

vu.main()
