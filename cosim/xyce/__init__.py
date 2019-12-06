"""
Mixed-Signal (Digital/Analog) simulation with GHDL and Xyce through VHPIDIRECT interface
"""

from os.path import join, dirname
from ..ghdlc import GHDLC


class Xyce:
    """
    Mixed-Signal co-simulation bridge class

    :param srcs: optional alternative location of the sources
    """

    def __init__(self, srcs=None):
        self._srcs = srcs if srcs else join(dirname(__file__))

    @property
    def vhdl(self):
        """
        Get lists of sources to be added to a simulation library
        """
        return [join(self._srcs, "vhdl", "*.vhd")]

    @property
    def include(self):
        """
        Get path to include directory containing C sources
        """
        return GHDLC().include + [join(self._srcs, "c")]


#    def verscript(self, file=None):
#        """
#        Get argument to add a custom version-script file to GHDL
#
#        :param file: provide a custom file instead of the one provided with the bridge
#        """
#        return "-Wl,-Wl,--version-script=%s" % (
#            file if file is not None else join(self._srcs, "c", "grt.ver")
#        )
