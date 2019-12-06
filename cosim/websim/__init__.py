"""
A Flask server as bridge between VHPIDIRECT and a web frontend
"""

from flask import Flask, jsonify, request
from ..utils import byte_buf, int_buf


class WebSim(object):
    """
    Web simulator server
    """

    def __init__(
        self, dist, load_cb=None, unload_cb=None, update_cb=None
    ):  # pylint: disable=too-many-locals
        self._load_cb = load_cb
        self._unload_cb = unload_cb
        self._update_cb = update_cb

        self._ghdl = None

        app = Flask("VUnitCoSim", static_folder=dist)

        self._app = app

        def index():
            return app.send_static_file("index.html")

        def favicon():
            return app.send_static_file("favicon.ico")

        def serve_js(path):
            return app.send_static_file("js/" + path)

        def serve_img(path):
            return app.send_static_file("img/" + path)

        def serve_css(path):
            return app.send_static_file("css/" + path)

        def serve_fonts(path):
            return app.send_static_file("fonts/" + path)

        def step():
            response = {"exitcode": 1}
            ghdl = self._ghdl
            if ghdl:
                mode = request.json["mode"]
                val = request.json["val"]
                step = 0
                if mode in "for":
                    val = ghdl.read_integer(0, 0) + int(val)
                elif mode in "every":
                    step = val
                    val = ghdl.read_integer(0, 0) + int(val)
                elif mode in "until":
                    val = -(2 ** 31) + int(val)

                ghdl.write_integer(0, 2, int(step))
                if val != 0:
                    # TOFIX We should also consider values which are larger than 32bits
                    ghdl.write_integer(0, 0, int(val))
                response = {"exitcode": 0}
            return jsonify(response)

        def load():
            response = {"exitcode": 0}
            if self._load_cb is not None:
                response = {"exitcode": self._load_cb()}
            return jsonify(response)

        def unload():
            response = {"exitcode": 0}
            if self._unload_cb is not None:
                response = {"exitcode": self._unload_cb()}
            return jsonify(response)

        def update():
            ghdl = self._ghdl
            response = {"update": 0}
            if ghdl:
                sts = ghdl.read_integer(0, 3)
                if sts != 0:
                    if self._update_cb is not None:
                        response = self._update_cb()
                response["update"] = sts
                ghdl.write_integer(0, 3, 0)
            return jsonify(response)

        self.add_url_rules(
            [
                ["/favicon.ico", "favicon", favicon],
                ["/", "index", index],
                ["/js/<path>", "js", serve_js],
                ["/img/<path>", "img", serve_img],
                ["/css/<path>", "css", serve_css],
                ["/fonts/<path>", "fonts", serve_fonts],
                ["/api/step", "step", step, ["GET", "POST"]],
                ["/api/load", "load", load],
                ["/api/unload", "unload", unload],
                ["/api/update", "update", update],
            ]
        )

    def add_url_rules(self, lst):
        """
        Add URL rules to the web server
        """
        for item in lst:
            if len(item) > 3:
                self._app.add_url_rule(item[0], item[1], item[2], methods=item[3])
            else:
                self._app.add_url_rule(item[0], item[1], item[2])

    def run(self, host="0.0.0.0"):
        self._app.run(host=host)

    def handler(self, hnd=None):
        """
        FIXME: should be a property
        """
        if hnd is not None:
            self._ghdl = hnd
        return self._ghdl
