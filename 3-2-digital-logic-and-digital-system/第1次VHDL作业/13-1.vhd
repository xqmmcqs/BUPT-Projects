library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
    port (
    a: in std_logic_vector(2 downto 0);
    y: out std_logic_vector(7 downto 0));
end vhdl1;
architecture decode38 of vhdl1 is
begin
    with a select
        y <= "11111110" when "000",
             "11111101" when "001",
             "11111011" when "010",
             "11110111" when "011",
             "11101111" when "100",
             "11011111" when "101",
             "10111111" when "110",
             "01111111" when "111",
             "11111111" when others;
end decode38;
