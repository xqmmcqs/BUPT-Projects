library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity vhdl1 is
	port (
	cr, ld, cp: in std_logic;
	d: in std_logic_vector(3 downto 0);
	q: out std_logic_vector(3 downto 0);
	co: out std_logic);
end vhdl1;

architecture counter74160 of vhdl1 is
	signal temp: std_logic_vector(3 downto 0);
begin
	process (cr, ld, cp, d)
	begin
		if (cr='0') then
			temp <= "0000";
		elsif (cp'event and cp='1') then
			if (ld='0') then
				temp <= d;
			elsif (temp=9) then
				temp <= "0000";
			else
				temp <= temp + 1;
			end if;
		end if;
	end process;
	q <= temp;
	co <= '1' when temp=9 else
		'0';
end counter74160;