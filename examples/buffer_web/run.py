"""
Buffer WebSim
-------------
"""

from sys import argv
from os.path import join, dirname
from subprocess import check_call
from vunit import VUnit
from cosim.vhpidirect import VHPIDIRECT

COSIM = VHPIDIRECT()

SRC = join(dirname(__file__), "src")

BUILD_ONLY = False
if "--build" in argv:
    argv.remove("--build")
    BUILD_ONLY = True

# Compile C application to an object
C_OBJ = join(SRC, "main.o")

check_call(
    [
        "gcc",
        "-fPIC",
        "-DTYPE=uint8_t",
        "-I%s" % COSIM.include,
        "-c",
        join(SRC, "main.c"),
        "-o",
        C_OBJ,
    ]
)

# Enable the external feature for strings/byte_vectors and integer_vectors
vu = VUnit.from_argv(vhdl_standard="2008", compile_builtins=False)
vu.add_builtins(COSIM.bridge({"string": True, "integer_vector": False}))

LIB = vu.add_library("lib")
LIB.add_source_files(join(SRC, "tb_ext_*.vhd"))

# Add the C object to the elaboration of GHDL
vu.set_sim_option(
    "ghdl.elab_flags", ["-Wl," + C_OBJ, COSIM.verscript()], overwrite=True
)

if BUILD_ONLY:
    vu.set_sim_option("ghdl.elab_e", True)
    vu._args.elaborate = True  # pylint: disable=protected-access
    vu.main(post_run=COSIM.post_func)
else:
    vu.main()
