# ICP-Algorithm-on-FPGA

We’ve designed RTL code using both AXI4 communication protocol and two Bram. The AXI4 communication protocol is for data transfer between Processing System(PS) and Programmable Logic(PL). Reference and Source points data, which consist of 8 bytes including number of data, x, and y position as fixed point, sent to PL from PS. During the data processing, both reference and source data are stored into Bram, which has a better speed of transferring data for high memory usage than registers on an FPGA. Bram has 40 bits for storing data and 11 bits for addresses. Therefore, the memory size of Bram is 2048 * 40 and used two Bram for both reference and source(aligned) data. After processing, the last source(aligned) data will be transferred to PS from PL.

![image](https://github.com/lonhb0124/ICP-Algorithm-on-FPGA/assets/111609834/4ae1a863-5f62-4bc3-a563-c48e68e4f5eb)


Control module controls the state, process, memory, output and many more. Every actual data is flown/calculated in the datapath module, which is made up of KD-tree-insertion, KD-Tree-Search. In the KD-Tree-Insertion module, reference data arrive and start insertion. In order to make KD-tree in Bram, the address starts from 1 and shifts the whole bits in the next depth. Depending on the left or right branch (Smaller values go to the left branch and bigger value goes to the right branch), add 1 bit on the shifted bit. Below figure is an example of how KD-tree was built in Bram. Address value does not only let users know if they are right branch or left branch, but also shows the depth of the tree. 

![image](https://github.com/lonhb0124/ICP-Algorithm-on-FPGA/assets/111609834/fdcf647c-257e-4057-b268-5bd4658c00ce)

KD-Tree-Search module used both data from reference data point (KD-tree) and source data point to find the closest point that passed threshold values which is 0.3. We designed a module that uses 10 source points at once and repeats the process until every source point is evaluated.
