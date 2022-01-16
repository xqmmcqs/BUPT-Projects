library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
    port (
    g1, g2a, g2b: in std_logic;
    a: in std_logic_vector(2 downto 0);
    y: out std_logic_vector(7 downto 0));
end vhdl1;
architecture decode38 of vhdl1 is
begin
    process(g1, g2a, g2b, a)
    begin
        if g1='0' or g2a='1' or g2b='1' then
            y<="11111111";
        elsif a="000" then
            y<="11111110";
        elsif a="001" then
            y<="11111101";
        elsif a="010" then
            y<="11111011";
        elsif a="011" then
            y<="11110111";
        elsif a="100" then
            y<="11101111";
        elsif a="101" then
            y<="11011111";
        elsif a="110" then
            y<="10111111";
        elsif a="111" then
            y<="01111111";
        end if;
    end process;
end decode38;
