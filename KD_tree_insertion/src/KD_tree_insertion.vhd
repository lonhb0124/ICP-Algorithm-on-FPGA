----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 03/15/2024 12:01:53 AM
-- Design Name: 
-- Module Name: KD_tree_insertion - Behavioral
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
use IEEE.STD_LOGIC_SIGNED.ALL;


-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity KD_tree_insertion is
  generic (
        MAX_DEPTH : positive := 10  -- Maximum depth of the tree
    );
    Port (
        clk : in STD_LOGIC;
        reset : in STD_LOGIC;
        insert_enable : in STD_LOGIC;
        x_data : in std_logic_vector(19 downto 0); -- Data to insert
        y_data : in std_logic_vector(19 downto 0);
        insert_complete : out std_logic; -- Signal to indicate insertion completion
        error : out std_logic   
    );
end KD_tree_insertion;

architecture Behavioral of KD_tree_insertion is

    --signal current_node_index : natural := 0;
    --signal current_depth : natural := 0;
    --signal insertion_in_progress : boolean := false;
    --signal insertion_completed : boolean := false;
    
    
    signal we : std_logic := '0';
    signal addr : std_logic_vector(10 downto 0);
    signal di : std_logic_vector(39 downto 0);
    signal do : std_logic_vector(39 downto 0);
    signal current_depth : unsigned(3 downto 0);
    signal insertion_in_progress : std_logic := '0';
    signal insertion_completed : std_logic := '0';

begin
     Bram_inst: entity work.Bram
        port map (
            clk => clk,
            we => we,
            addr => addr,
            di => di,
            do => do
        );
    
    process (clk, reset)
    begin
        if reset = '1' then
            we <= '0';
            insert_complete <= '0';
            error <= '0';
            addr <= (others => '0');
            current_depth <= (others => '0');
            -- Perform other reset actions as needed
        elsif rising_edge(clk) then
            if insert_enable = '1' and insertion_in_progress = '0' then 
                insertion_in_progress <= '1';
                addr <= "00000000001";
                current_depth <= (others => '0');
                we <= '0';
                insert_complete <= '0';
            
            elsif insertion_in_progress = '1' then
                if do(39 downto 0) = "0000000000000000000000000000000000000000" or current_depth > 10 then
                    di <= x_data & y_data;
                    we <= '1'; -- Write operation
                    insertion_completed <= '1';
                    insert_complete <= '1';
                    insertion_in_progress <= '0';
                else
                    we <= '0'; -- Stop write operation
                    if current_depth mod 2 = 0 then 
                        if x_data <= do(39 downto 20) then
                            addr <= addr(9 downto 0) & '0';
                        else
                            addr <= (addr(9 downto 0) & '0') + "00000000001";
                        end if;
                    else
                        if y_data <= do(19 downto 0) then
                            addr <= addr(9 downto 0) & '0';
                        else
                            addr <= (addr(9 downto 0) & '0') + "00000000001";
                        end if;      
                    end if;
                    current_depth <= current_depth + 1;
                    insertion_completed <= '0';
                    insert_complete <= '0';
                end if;
            end if;
        end if;
    end process;

end Behavioral;
