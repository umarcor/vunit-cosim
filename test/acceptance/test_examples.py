"""
Verify that all example run scripts work correctly
"""

import sys
from os import environ
from os.path import join, dirname
from subprocess import call
import unittest
import pytest
from vunit.sim_if.common import has_simulator, simulator_check

ROOT = join(dirname(__file__), "..", "..")


@unittest.skipIf(
    not has_simulator()
    or simulator_check(lambda simclass: not simclass.supports_vhpi()),
    "A simulator/backend that supports interfacing with external C code is required",
)
class TestExamples(unittest.TestCase):
    """
    Verify that example projects run correctly
    """

    def test_buffer(self):
        for item in ["run", "cp"]:
            self.check(
                join(ROOT, "examples", "buffer", "%s.py" % item),
                args=["--clean", "--xunit-xml=%s" % self.report_file],
            )

    @pytest.mark.xfail
    def test_buffer_dyn(self):
        self.check(
            join(dirname(__file__), "..", "..", "examples", "buffer", "run.py"),
            args=["--build", "--xunit-xml=%s" % self.report_file],
        )
        self.check(
            join(dirname(__file__), "..", "..", "examples", "buffer", "cosim.py"),
            args=["tb_external_string"],
        )
        self.check(
            join(dirname(__file__), "..", "..", "examples", "buffer", "cosim.py"),
            args=["tb_external_byte_vector"],
        )
        self.check(
            join(dirname(__file__), "..", "..", "examples", "buffer", "cosim.py"),
            args=["tb_external_integer_vector"],
        )

    @pytest.mark.xfail
    def test_buffer_web(self):
        self.check(
            join(dirname(__file__), "..", "..", "examples", "buffer_web", "run.py"),
            args=["--clean", "--xunit-xml=%s" % self.report_file],
        )

    @pytest.mark.xfail
    def test_octave(self):
        # in_400.csv 20x20
        # in_262144.csv 13108x2
        self.check(
            join(dirname(__file__), "..", "..", "examples", "octave", "run.py"),
            args=["--clean", "--xunit-xml=%s" % self.report_file],
        )

    def test_py_callback(self):
        self.check(
            join(dirname(__file__), "..", "..", "examples", "py_callback", "run.py"),
            args=["--clean", "--xunit-xml=%s" % self.report_file],
        )

    def test_xyce(self):
        self.check(
            join(dirname(__file__), "..", "..", "examples", "xyce", "run_minimal.py"),
            args=["--clean", "--xunit-xml=%s" % self.report_file],
        )
        self.check(
            join(dirname(__file__), "..", "..", "examples", "xyce", "run.py"),
            args=["--clean", "--xunit-xml=%s" % self.report_file],
        )

    def setUp(self):
        self.output_path = join(dirname(__file__), "examples_run_out")
        self.report_file = join(self.output_path, "xunit.xml")

    def check(self, run_file, args=None, vhdl_standard="2008", exit_code=0):
        """
        Run external run file and verify exit code
        """
        args = args if args is not None else []
        new_env = environ.copy()
        new_env["VUNIT_VHDL_STANDARD"] = vhdl_standard
        retcode = call(
            [sys.executable, run_file, "--output-path=%s" % self.output_path]
            + (args if args else []),
            env=new_env,
        )
        self.assertEqual(retcode, exit_code)
