"""
Dummy VUnit co-simulation bridge
"""

from os.path import join, dirname


class Dummy:
    """
    Dummy VUnit co-simulation bridge class

    :param srcs: optional alternative location of the VHDL sources
    """

    def __init__(self, srcs=None):
        self._srcs = srcs if srcs else join(dirname(__file__))

    def vhdl(self, features=None):
        """
        Get lists of files to register the bridge with ``vu.add_builtins``
        """
        api = {"string": True, "integer_vector": True}
        for key in api:
            if key in features:
                val = features[key]
                if val is not True:
                    api[key] = val
                else:
                    api[key] = [join(self._srcs, "vhdl", "%s.vhd" % key)]
        return api
