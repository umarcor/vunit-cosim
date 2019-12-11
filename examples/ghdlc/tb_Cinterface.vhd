entity tb_Cinterface is
end;

architecture arch of tb_Cinterface is

  type rec_t is record
    r_char: character;
    r_int : integer;
  end record;

  type enum_t is (standby, start, busy, done);

  type int_vec_t is array(natural range <>) of integer;
  type real_vec_t is array(natural range <>) of real;
  type bool_vec_t is array(natural range <>) of boolean;
  type time_vec_t is array(natural range <>) of time;
  type rec_vec_t is array(natural range <>) of rec_t;
  type enum_vec_t is array(natural range <>) of enum_t;

  type real_2vec_t is array (natural range <>, natural range <>) of real;

begin
  process

  procedure testCinterface(
      v_char      : character   := 'k';
      v_int       : integer     := -6;
      v_nat       : natural     := 9;
      v_pos       : positive    := 3;
      v_real      : real        := 3.34;
      v_bool      : boolean     := true;
      v_bit       : bit         := '1';
      v_time      : time        := 20 ns;
      v_rec       : rec_t       := ('y', 5);
      v_enum      : enum_t      := busy;
      v_str       : string      := "hellostr";
      v_vec_int   : int_vec_t   := (11, 22, 33, 44, 55);
      v_vec_real  : real_vec_t  := (0.5, 1.75, 3.33, -0.125, -0.67, -2.21);
      v_vec_bool  : bool_vec_t  := (false, true, true, false);
      v_vec_bit   : bit_vector  := ('1', '0', '1', '0');
      v_vec_time  : time_vec_t  := (1 ns, 50 ps, 1.34 us);
      v_vec_rec   : rec_vec_t   := (('x', 17),('y', 25));
      v_vec_enum  : enum_vec_t  := (start, busy, standby);
      v_2vec_real : real_2vec_t := ((0.1, 0.25, 0.5),(3.33, 4.25, 5.0))
  ) is
  begin
    assert false report "VHPIDIRECT testCinterface" severity failure;
  end;
    attribute foreign of testCinterface : procedure is "VHPIDIRECT testCinterface";

    function getString return string is
    begin
      assert false report "VHPIDIRECT getString" severity failure;
    end;
      attribute foreign of getString : function is "VHPIDIRECT getString";

    --constant g_str: string := getString;

  begin
    testCinterface(
      v_char      => 'k',
      v_int       => -6,
      v_nat       => 9,
      v_pos       => 3,
      v_real      => 3.34,
      v_bool      => true,
      v_bit       => '1',
      v_time      => 20 ns,
      v_rec       => ('y', 5),
      v_enum      => busy,
      v_str       => "hellostr",
      v_vec_int   => (11, 22, 33, 44, 55),
      v_vec_real  => (0.5, 1.75, 3.33, -0.125, -0.67, -2.21),
      v_vec_bool  => (false, true, true, false),
      v_vec_bit   => ('1', '0', '1', '0'),
      v_vec_time  => (1 ns, 50 ps, 1.34 us),
      v_vec_rec   => (('x', 17),('y', 25)),
      v_vec_enum  => (start, busy, standby),
      v_2vec_real => ((0.1, 0.25, 0.5),(3.33, 4.25, 5.0))
    );
    --report "string: " & getString severity note;
    wait;
  end process;
end;