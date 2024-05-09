----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/14/2024 11:34:44 PM
-- Design Name: 
-- Module Name: Bram - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity Bram is
port(
clk : in std_logic;
we : in std_logic;
addr : in std_logic_vector(10 downto 0);
di : in std_logic_vector(39 downto 0);
do : out std_logic_vector(39 downto 0)
);
end Bram;

architecture syn of Bram is
type ram_type is array (2047 downto 0) of std_logic_vector (39 downto 0);
signal RAM : ram_type := (others => (others => '0'));

begin
process(clk)
begin
if falling_edge(clk) then
if we = '1' then
RAM(to_integer(unsigned(addr))) <= di;
end if;
do <= RAM(to_integer(unsigned(addr)));
end if;
end process;

end syn;
