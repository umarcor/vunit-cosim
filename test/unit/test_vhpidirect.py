"""
Test the VHPIDIRECT bridge
"""

import unittest
import os
from os.path import join, dirname

# from shutil import rmtree
# from time import sleep
# import itertools
# from unittest import mock

# from vunit.exceptions import CompileError
# from vunit.ostools import renew_path, write_file
# from vunit.project import Project
# from vunit.source_file import file_type_of


class TestVHPIDIRECT(unittest.TestCase):  # pylint: disable=too-many-public-methods
    """
    Test the VHPIDIRECT bridge
    """

    def setUp(self):
        self.output_path = join(dirname(__file__), "test_vhpidirect_out")
        # renew_path(self.output_path)
        # self.project = Project()
        self.cwd = os.getcwd()
        # os.chdir(self.output_path)

    def tearDown(self):
        os.chdir(self.cwd)
        # if exists(self.output_path):
        #     rmtree(self.output_path)

    @staticmethod
    def test_hello():
        print("HELLO WORLD!")


# vhpidirect
#   vhdl()
#   vhdl({"string": True})
#   vhdl({"integer_vector": True})
#   vhdl({"string": True, "integer_vector": True})
#   vhdl({"string": True, "integer_vector": False})
#   vhdl({"string": True, "integer_vector": None})


#    @unittest.skipIf(
#        simulator_check(lambda simclass: not simclass.supports_vhpi()),
#        "This simulator/backend does not support interfacing with external C code",
#    )
#    def test_vhdl_external_buffer_project(self):
#        self.check(join(ROOT, "examples", "vhdl", "external_buffer", "run.py"))
#        self.check(join(ROOT, "examples", "vhdl", "external_buffer", "cp.py"))
