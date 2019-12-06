package xyce_xhdl_pkg is

  type arr_t is array (natural range <>) of real;
  constant null_arr_t : arr_t(0 downto 1) := (others => 0.0);

  function xyce_init(
    id      : string;
    circuit : string
  ) return integer;
    attribute foreign of xyce_init : function is "VHPIDIRECT xhdl_init";

  function xyce_run(
    id        : string;
    reqTime   : real;
    arrTime   : arr_t := null_arr_t;
    arrVolt   : arr_t := null_arr_t
  ) return integer;
    attribute foreign of xyce_run : function is "VHPIDIRECT xhdl_run";

  function xyce_read(
    id   : string;
    name : string
  ) return real;
    attribute foreign of xyce_read : function is "VHPIDIRECT xhdl_read";

  procedure xyce_close(
    id : string
  );
    attribute foreign of xyce_close : procedure is "VHPIDIRECT xhdl_close";

end xyce_xhdl_pkg;

package body xyce_xhdl_pkg is

  function xyce_init(
    id      : string;
    circuit : string
  ) return integer is
  begin
    assert false report "VHPIDIRECT xyce_init" severity failure;
  end;

  function xyce_run(
    id        : string;
    reqTime   : real;
    arrTime   : arr_t := null_arr_t;
    arrVolt   : arr_t := null_arr_t
  ) return integer is
  begin
    assert false report "VHPIDIRECT xyce_run" severity failure;
  end;

  function xyce_read(
    id   : string;
    name : string
  ) return real is
    begin
      assert false report "VHPIDIRECT xyce_read" severity failure;
    end;

  procedure xyce_close(
    id : string
  ) is
  begin
    assert false report "VHPIDIRECT xyce_close" severity failure;
  end;

end xyce_xhdl_pkg;
