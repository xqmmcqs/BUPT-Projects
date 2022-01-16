library ieee;
use ieee.std_logic_1164.all;

entity vhdl1 is
	port (
	i: in bit_vector(1 to 9);
	o: out bit);
end vhdl1;

architecture art of vhdl1 is
	component notand3 is
		port (
		in1, in2, in3: in bit;
		o: out bit);
	end component;
	signal o1, o2, o3: bit;
begin
	u1: notand3 port map (i(1), i(2), i(3), o1);
	u2: notand3 port map (i(4), i(5), i(6), o2);
	u3: notand3 port map (i(7), i(8), i(9), o3);
	u4: notand3 port map (o1, o2, o3, o);
end art;
	
