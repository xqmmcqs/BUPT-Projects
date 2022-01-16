library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
    port (
    clk, x: in std_logic;
    q: out std_logic_vector(1 downto 0));
end vhdl1;

architecture mac of vhdl1 is
    type stat is (s0, s1, s2, s3);
    signal curstat, nextstat: stat;
begin
    process (clk)
    begin
        if(clk'event and clk='1') then
            curstat <= nextstat;
        end if;
    end process;
    process (x, curstat)
    begin
        case curstat is
            when s0 =>
                if x='1' then
                    nextstat <= s0;
                else
                    nextstat <= s1;
                end if;
            when s1 =>
                if x='1' then
                    nextstat <= s2;
                else
                    nextstat <= s1;
                end if;
            when s2 =>
                if x='1' then
                    nextstat <= s2;
                else
                    nextstat <= s3;
                end if;
            when others =>
                if x='1' then
                    nextstat <= s0;
                else
                    nextstat <= s3;
                end if;
        end case;
    end process;
    with curstat select
        q <= "00" when s0,
             "01" when s1,
             "10" when s2,
             "11" when others;
end mac;
