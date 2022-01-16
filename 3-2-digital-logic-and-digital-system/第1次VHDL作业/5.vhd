library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
    port (
    a, b, c, d: in std_logic;
    y: out std_logic);
end vhdl1;
architecture notand of vhdl1 is
begin
    y <= not(a and b and c and d);
end notand;
