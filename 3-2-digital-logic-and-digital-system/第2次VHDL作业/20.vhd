library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
    port (
    clk, y, clr: in std_logic;
    qout: out std_logic_vector(2 downto 0);
    co: out std_logic);
end vhdl1;

architecture counter of vhdl1 is
    signal temp: std_logic_vector(2 downto 0);
begin
    process (clk, clr)
    begin
        if (clr='0') then
            temp <= "000";
        elsif (clk'event and clk='1') then
            if (y='1') then
                case temp is
                    when "000" => temp <= "001";
                    when "001" => temp <= "011";
                    when "011" => temp <= "010";
                    when "010" => temp <= "110";
                    when "110" => temp <= "111";
                    when "111" => temp <= "101";
                    when "101" => temp <= "100";
                    when others => temp <= "000";
                end case;
            else
                case temp is
                    when "000" => temp <= "100";
                    when "001" => temp <= "000";
                    when "011" => temp <= "001";
                    when "010" => temp <= "011";
                    when "110" => temp <= "010";
                    when "111" => temp <= "110";
                    when "101" => temp <= "111";
                    when others => temp <= "101";
                end case;
            end if;
        end if;
    end process;
    qout <= temp;
    co <= '1' when ((temp="000" and y='0') or (temp="100" and y='1')) else
          '0';
end counter;
