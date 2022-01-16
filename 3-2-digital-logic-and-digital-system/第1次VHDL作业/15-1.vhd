library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
    port (
    a: in std_logic_vector(7 downto 0);
    y: out std_logic_vector(2 downto 0));
end vhdl1;
architecture encode83 of vhdl1 is
begin
    y <= "000" when a(7)='0' else
         "001" when a(6)='0' else
         "010" when a(5)='0' else
         "011" when a(4)='0' else
         "100" when a(3)='0' else
         "101" when a(2)='0' else
         "110" when a(1)='0' else
         "111";
end encode83;
