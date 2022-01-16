library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
    port (
    a: in std_logic_vector(7 downto 0);
    y: out std_logic_vector(2 downto 0);
    ei: in std_logic;
    eo, gs: out std_logic);
end vhdl1;
architecture encode83 of vhdl1 is
begin
    process(a, ei)
    begin
        if ei='1' then
            y <= "111";
            eo <= '1';
            gs <= '1';
        elsif a(7)='0' then
            y <= "000";
            eo <= '1';
            gs <= '0';
        elsif a(6)='0' then
            y <= "001";
            eo <= '1';
            gs <= '0';
        elsif a(5)='0' then
            y <= "010";
            eo <= '1';
            gs <= '0';
        elsif a(4)='0' then
            y <= "011";
            eo <= '1';
            gs <= '0';
        elsif a(3)='0' then
            y <= "100";
            eo <= '1';
            gs <= '0';
        elsif a(2)='0' then
            y <= "101";
            eo <= '1';
            gs <= '0';
        elsif a(1)='0' then
            y <= "110";
            eo <= '1';
            gs <= '0';
        elsif a(0)='0' then
            y <= "111";
            eo <= '1';
            gs <= '0';
        else
            y <= "111";
            eo <= '0';
            gs <= '1';
        end if;
    end process;
end encode83;
