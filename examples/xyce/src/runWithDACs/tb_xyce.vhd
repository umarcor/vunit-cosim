library vunit_lib;
context vunit_lib.vunit_context;

use work.xyce_pkg.xyce_t;
use work.xyce_pkg.xyce_arr_t;

entity tb_xyce_eg_with_dacs is
  generic (
    runner_cfg : string;
    tb_path    : string;
    circuit    : string := "../circuit_DACs.cir"
  );
end entity;

architecture tb of tb_xyce_eg_with_dacs is begin

  main: process
    constant steps: natural := 10;
    constant total_sim_time : real := 20.0e-4;
    constant arrV     : xyce_arr_t(0 to 8) := (0.0, 0.0, 3.0, 3.0, 0.0, 0.0, 3.0, 3.0, 0.0);
    constant arrTbase : xyce_arr_t(0 to 8) := (0.0, 0.1e-4, 0.2e-4, 0.4e-4, 0.5e-4, 0.7e-4, 0.8e-4, 1.0e-4, 1.1e-4);

    variable xyce : xyce_t;
    variable arrT     : xyce_arr_t(arrV'range);
    variable response : real;
  begin
    test_runner_setup(runner, runner_cfg);
    while test_suite loop
      if run("test") then

        xyce.init("run a circuit with dacs", tb_path & circuit);

        for i in arrT'range loop
          arrT(i) := arrTbase(i);
        end loop;

        for i in 0 to steps-1 loop
          xyce.run(0.0 + real(i+1) * 0.1 * total_sim_time, arrT, arrV);

          response := xyce.read("YMEMRISTORRES");
          info("response: " & to_string(response));

          -- update arrT to repeat pulse
          for j in arrT'range loop
            arrT(j) := arrTbase(j) + xyce.reqT;
          end loop;
        end loop;

        xyce.close;

      end if;
    end loop;
    test_runner_cleanup(runner);
    wait;
  end process;

end architecture;