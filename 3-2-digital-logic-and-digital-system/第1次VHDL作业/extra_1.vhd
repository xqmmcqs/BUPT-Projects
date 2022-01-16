library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity vhdl1 is
    port (
    a, b: in std_logic_vector(3 downto 0);
    c: in std_logic;
    sum: out std_logic_vector(3 downto 0);
    cout: out std_logic);
end vhdl1;

architecture add of vhdl1 is
    signal sumt: std_logic_vector(4 downto 0);
begin
    sumt <= ('0'&a)+b+c;
    cout <= sumt(4);
    sum <= sumt(3 downto 0);
end add;
