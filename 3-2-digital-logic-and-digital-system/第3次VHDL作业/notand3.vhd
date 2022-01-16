library ieee;
use ieee.std_logic_1164.all;

entity notand3 is
	port (
	in1, in2, in3: in bit;
	o: out bit);
end notand3;

architecture art1 of notand3 is
begin
	o <= not(in1 and in2 and in3);
end art1;
