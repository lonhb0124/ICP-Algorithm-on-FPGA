library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;
use ieee.std_logic_textio.all;
library std;
use std.textio.all; -- Include textio library for file I/O

entity KD_Tree_tb is
end KD_Tree_tb;

architecture Behavioral of KD_Tree_tb is
    constant CLK_PERIOD : time := 10 ns;
    
    signal clk_tb : std_logic := '0';
    signal reset_tb : std_logic := '0';
    signal insert_enable_tb : std_logic := '0';
    signal x_data_tb : std_logic_vector(19 downto 0) := (others => '0');
    signal y_data_tb : std_logic_vector(19 downto 0) := (others => '0');
    signal insert_complete_tb : std_logic;
    signal error_tb : std_logic;
    
    constant MAX_DEPTH : positive := 10;
    signal data_tb: std_logic_vector(39 downto 0) := (others => '0');
   
    
begin

    dut : entity work.KD_tree_insertion
        generic map (
            MAX_DEPTH => MAX_DEPTH
        )
        port map (
            clk => clk_tb,
            reset => reset_tb,
            insert_enable => insert_enable_tb,
            x_data => x_data_tb,
            y_data => y_data_tb, 
            insert_complete => insert_complete_tb,
            error => error_tb
        );
        
    clk_process : process
    begin
        while now < 300 ns loop
            clk_tb <= not clk_tb;
            wait for 1 ns;
        end loop;
        wait;
    end process clk_process;

    reset_process : process
    begin
        reset_tb <= '1';
        wait for 5 ns;
        reset_tb <= '0';
        wait;
    end process reset_process;
    
    -- Process for reading input vectors from the file
        insertion_process : process
    begin
        -- xy (109.1333, 17.3333)
        data_tb <= "0001101101001000100000000100010101010101";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (91, 24.5)
        data_tb <= "0001011011000000000000000110001000000000";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0);  
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (113.75, 20.75)
        data_tb <= "0001110001110000000000000101001100000000";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (72, 26.5)
        data_tb <= "0001001000000000000000000110101000000000";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (118, 18.9)
        data_tb <= "0001110110000000000000000100101110011001";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0);
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
       
        -- xy (176.5, 21.5)
        data_tb <= "0010110000100000000000000101011000000000";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0);   
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (185, 35)
        data_tb <= "0010111001000000000000001000110000000000";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0);
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (144.666, 25.333)
        data_tb <= "0010010000101010100100000110010101010100";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (36, 20.1)
        data_tb <= "0000100100000000000000000101000001100110";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (108, 26.333)
        data_tb <= "0001101100000000000000000110100101010100";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0);
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (115, 19.7)
        data_tb <= "0001110011000000000000000100111011001100";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
       
        -- xy (32, 24.1)
        data_tb <= "0000100000000000000000000110000001100110";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
       
        -- xy (124, 20.44)
        data_tb <= "0001111100000000000000000101000111000010";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (104, 23.4)
        data_tb <= "0001101000000000000000000101110110011001";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        -- xy (51.333, 36.333)
        data_tb <= "0000110011010101010000001001000101010100";
        wait for 2 ns;
        x_data_tb <= data_tb(39 downto 20); 
        y_data_tb <= data_tb(19 downto 0); 
        insert_enable_tb <= '1';
        wait until rising_edge(insert_complete_tb);
        insert_enable_tb <= '0';
        
        
        
        

        
        wait;
    end process insertion_process;

end Behavioral;
