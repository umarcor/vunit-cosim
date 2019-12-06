use work.xyce_xhdl_pkg.all;

package xyce_pkg is

  alias xyce_arr_t is arr_t;

  type xyce_t is protected
    procedure init (
      id      : string;
      circuit : string
    );

    procedure run (
      reqT    : real;
      arrTime : arr_t := null_arr_t;
      arrVolt : arr_t := null_arr_t
    );

    impure function reqT return real;

    impure function read (
      name : string
    ) return real;

    procedure close;
  end protected;

end xyce_pkg;
