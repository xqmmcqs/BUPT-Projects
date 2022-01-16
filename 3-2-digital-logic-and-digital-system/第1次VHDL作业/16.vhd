library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
    port (
    a: in std_logic_vector(3 downto 0);
    y: out std_logic_vector(3 downto 0));
end vhdl1;
architecture decodeBCD of vhdl1 is
begin
    process(a)
        variable t: std_logic_vector(3 downto 0);
    begin
        t(3) := a(3);
        t(2) := a(2) xor t(3);
        t(1) := a(1) xor t(2);
        t(0) := a(0) xor t(1);
        y <= t;
    end process;
end decodeBCD;
