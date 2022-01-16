library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
    port (
    a, b, c, d, e, f: in std_logic;
    y: out std_logic);
end vhdl1;
architecture sop of vhdl1 is
begin
    y <= (a and b) or (c and d) or (e and f);
end sop;
