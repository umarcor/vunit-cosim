"""
GHDL C interface for VHPIDIRECT types
"""

from os.path import join, dirname


class GHDLC:
    """
    GHDL C bridge class for VHPIDIRECT

    :param srcs: optional alternative location of the sources
    """

    def __init__(self, srcs=None):
        self._srcs = srcs if srcs else join(dirname(__file__))

    @property
    def include(self):
        """
        Get path to include directory containing C sources
        """
        return [join(self._srcs, "c")]


#    def verscript(self, file=None):
#        """
#        Get argument to add a custom version-script file to GHDL
#
#        :param file: provide a custom file instead of the one provided with the bridge
#        """
#        return "-Wl,-Wl,--version-script=%s" % (
#            file if file is not None else join(self._srcs, "c", "grt.ver")
#        )
