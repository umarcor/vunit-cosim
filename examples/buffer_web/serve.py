"""
Buffer: run web-based VUnitCoSim simulation
"""

from os.path import join, dirname
from cosim.websim import WebSim
from cosim.utils import (
    dlopen,
    dlclose,
    byte_buf,
    int_buf,
    enc_args,
    read_int_buf,
    read_byte_buf,
)


ROOT = dirname(__file__)


# Allocate and define shared data buffers

DATA = [3 * c for c in range(64)]

BUF = [[] for c in range(2)]
BUF[1] = byte_buf(DATA + [0 for x in range(2 * len(DATA))])

BUF[0] = int_buf(
    [-(2 ** 31) + 1, -(2 ** 31), 0, 1, len(DATA)]  # clk_step, update, block_length
)


# Load args and define simulation callbacks

SIM = None
ARGS = [line.rstrip("\n") for line in open(join(ROOT, "args.txt"))]


def load():
    """
    Callback from the frontend to load the simulation executable
    """
    ghdl = dlopen(ARGS[0])
    SIM.handler(ghdl)

    for idx, val in enumerate(BUF):
        ghdl.set_string_ptr(idx, val)

    xargs = enc_args(ARGS)
    return ghdl.ghdl_main(len(xargs) - 1, xargs)


def update_cb():
    """
    Callback from the frontend to get updated values from the simulation
    """
    params = read_int_buf(BUF[0])[0:3]
    params[0] -= -(2 ** 31)
    params[1] -= -(2 ** 31)
    return {
        "name": "external_buffer",
        "params": params,
        "data": {"mem": read_byte_buf(BUF[1])},
    }


def unload():
    """
    Callback from the frontend to unload the simulation executable
    """
    dlclose(SIM.handler())


# Instantiate WebSim and run server

SIM = WebSim(
    dist=join(ROOT, "..", "vue", "dist"),
    load_cb=load,
    unload_cb=unload,
    update_cb=update_cb,
)

SIM.run()
