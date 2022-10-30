## Task1 - Simulating a basic 8-bit binary counter
---


**Step 1: Create a folder tree** on your local disk for this module.


**Step 2:** Run VS Code and open the Lab1-Counter folder with:

*File -> Open Folder* (then select the task1 folder, inside the Lab1-Counter folder)

**Step 3:** Create a new System Verilog file (counter.sv), and enter the code shown on the right. You should include all the comments for future reference.  Colour highlight is automatic if you have installed the System Verilog package for VSC. The schematic representation of this basic counter is shown below. It counts on the positive edge of clk if enable is ‘1’.  It is synchronously reset to 0 if rst is asserted.  Save this file as counter.sv.

![counter](images/counter.jpg)

Note the following:
1.	The file name and the module name must be the same.
2.	The number of bits in the counter is specified with the parameter WIDTH. It is currently set to 8-bit. 
3.	The always_ff @ (posedge clk) is the way that one specifies a clocked circuit. 
4.	‘<=’ in line 12 and 13 are non-block assignments which should be used within an always_ff block.
5.	{WIDTH{1’b0}} in line 12 uses the concatenation operator { } to form WIDTH bits of ‘0’. (Can you explain the construct in line 13?)

Here is the mapping between System Verilog and the counter circuit "synthesized" via Verilator:

![counter interal](images/counter_inners.jpg)

**Step 4:** Create the testbench file **_counter_tb.cpp_** in C++ using VS Code.  

We need to do this before we can combine everything to make the executable model.  The listing for **_counter_tb.cpp_** is shown below.  Again, you are required to type this in (instead of copying) to make sure that you think about each line of this program.  

This testbench file is a template for all other testbench files. It consists of various sections, which are mandatory (except for the trace dump section if you don’t want to see the waveforms).  Make sure that you understand what each section is for!

![Testbench](images/counter_tb.jpg)

**Step 5: Compile the System Verilog model with the testbench** 

Using doit.sh:

This makes **_Vcounter_**, which is the executable model of our counter!  

We are now ready to simulate by simply running **_Vcounter_**, which is again in the **_obj_dir/._** directory.

This runs very fast, and you may think that nothing happened.  However, if you examine the folder task1, you will see a file **_Vcounter.vcd_** has been generated.  This is the trace waveform file and contains the simulation results.

**Step 6:**  Plot the counter waveforms with GTKwave: 

window will appear.  Click *Top -> counter*, followed by the signals: **_clk, rst, en_** and **_count[7:0]_**.  
Use the + and - icons to adjust the zoom level.  You should see the followStart the GTKwave program on your laptop. Select *File -> Open New Tab* and select **_Vcounter.vcd_** file.  A GTKwave ing. 

![gtkWave](images/waveform.jpg)

Make sure you understand all waveform signals.  

Why is the time axis in ps? Does it matter?

Congratulations! You have successfully simulated your first System Verilog digital hardware module.

![Verilator Generated](images/verilator_output.jpg)

## TEST YOURSELF CHALLENGES:  
1.	Modify the testbench so that you stop counting for 3 cycles once the counter reaches 0x9, and then resume counting.  You may also need to change the stimulus for _rst_.
2.	The current counter has a synchronous reset. To implement asynchronous reset, you can change line 11 of counter.sv to detect change in _rst_ signal.  (See notes.) 
 
Make these modification, compile, and run.  Examine the waveform with GTKwave and explain what you see.

---

## Task 2: Linking Verilator simulation with Vbuddy
---

In this task, you will learn how to modify the testbench to interface with the Vbuddy board.  Before you start, copy  **_counter.sv_** and **_counter_tb.cpp_** to the task2 folder.  

You also need to make a copy of the files **_Vbuddy.cpp_** and **_Vbuddy.cfg_** from your Lab 1 folder to here.

**Step 1: Set up the Vbuddy interface** 

Connect Vbuddy to your computer's USB port using a USB cable provided.  Find out the name of the USB device used.  How? This depends on whether you are using a MacBook or a PC.

---

**_Macbook Users_**
 For Mac users, enter: 
```bash
ls /dev/tty.u*
```
You should see device name as something similar this:
```bash
/dev/tty.usbserial-110
```

---
**_Window Users_**

For PC users, you now need to find the COM Port used to communicate to Vbuddy. To do this, open Device Manager from the Start Menu and look for the `Ports (COM & LPT)` section. Expand this section and then connect Vbuddy to your computer. When the device is connected, you should see a new entry in this list with the COM port number associated with Vbuddy. This may change every so often, so make sure to check this when connecting your device.

The respective device name is `/dev/ttyS{number}`

For example, in the case of the picture below, the device name would be `/dev/ttyS3`. Make sure the `S` is a capital letter when you later use this value.

![image](https://user-images.githubusercontent.com/1413854/196824390-356e0531-be49-490c-ae63-46149d22bd87.png)

---

Create a file: **_vbuddy.cfg_**, which contains the device name as the only line (terminated with CR).  You may use VS Code or the Linux nano editor to do this. 

**Step 2: Modify testbench for Vbuddy**

Copy **_counter.sv_** and **_count_tb.cpp_** to your task2 folder. 

Modify the testbench file  **_count_tb.cpp_**  to include **Vbuddy function** as shown in the diagram below.  What they do are self-explanatory.  You can find a list of the current Vbuddy functions which you can include in the testbench file here.  These functions are of the form **_vbdXxxx_**.

![testbench with Vbuddy](images/vbd_function.jpg)

Recompile and test.

**Step 3: Explore flag feature on Vbuddy**

Vbuddy’s rotary encode has a push-button switch.  Vbuddy keeps an internal flag which, by default, will toggle between ‘0’ and ‘1’ each time the button is pressed. You can interrogate this toggle switch state with **_vbdFlag();_**, which will return its current state and then toggle.  A little postbox showing the flag state is drawn in the footer of the TFT display.  

You can use this feature to enable and disable the counter by modifying line 48 of the testbench with: 
```C++
    top->en = vbdFlag();
```

Re-compile and test.  

Instead of showing count values on 7-segment displays, you may also plot this on the TFT by replacing the **_vdbHex()_** section with the command **_vbdPlot()_**.  You may want to increase the number of clock cycles to simulate because plotting a dot is much faster than outputting to the 7-segment display.  You can start/stop the counter with the flag.
```C++
    vbdPlot(int(top->count), 0, 255);
```

## TEST YOURSELF CHALLENGE:  

Modify your counter and testbench files so that the **_en_** signal controls the direction of counting: ‘1’ for up and ‘0’ for down, via the **_vbdFlag()_** function.

___
WELL DONE!  YOU MANAGE TO SPECIFY AND VERIFY YOUR DESIGN AND USE VBUDDY TO SEE THE RESULTS OF YOUR CIRCUIT.  THE NEXT TWO TASK: TASK3 and TASK4 ARE OPTIONAL. YOU CAN SKIP THEM IF YOU RUN OUT OF TIME.
___

## Task 3: Vbuddy parameter & flag in one-shot mode (OPTIONAL)

The rotary encodes (EC11) provides input from Vbuddy to the Verilator simulation model.  Turning the encoder changes a stored parameter value on Vbuddy independently from the Verilator simulation.  This parameter value can be read using the **_vbdValue( )_** function, and is displayed on the bottom left corner of the TFT screen in both decimal and hexadecimal format.  

**Step 1: Loadable counter**

Copy across to the task3 foler the required files: **_counter.sv, counter_tb.cpp, vbuddy.cpp, doit.sh_** and **_vbuddy.cfg_**.  

Vbuddy’s flag register has two modes of operation.  The default mode is **TOGGLE**, which means that everything the rotary encoder switch is pressed, the flag will toggle as indicated at the bottom of the TFT screen.  

However, using the **_vbdSetMode(1)_** function, you can set the mode to ONE-SHOT behaviour. Whenever the switch is pressed, the flag register is set to ‘1’ as before – now the flag is **“ARMED”** ready to fire. However, when the flag register is read, it immediate resets to ‘0’.  

Modify **counter.sv** so that pressing the switch on EC11 forces the counter to pre-set to Vbuddy’s parameter value. (How?)  Compile and test your design.

**Step 2: Single stepping**

Using the one-shot behaviour of the Vbuddy flag, it is possible to provide one clock pulse each time you press the rotary encoder switch.  In other words, you can single step the counting action.  

Modify **counter.sv** so that you only increment the counter each time you press the switch.


---

## Task 4: Displaying count as Binary Coded Decimal (BCD) numbers (OPTIONAL)
---

So far, you have only simulated a single module.  In this task, you will create a top-level module (**top.sv**), which has the counter module, and a second module that converts the 8-bit binary number into three BCD digits. You can find out how the binary to BCD conversion algorithm works from the course webpage.  

Copy and paste the following code to **top.sv**:

```verilog
module top #(
  parameter WIDTH = 8,
  parameter DIGITS = 3
)(
  // interface signals
  input  wire             clk,      // clock 
  input  wire             rst,      // reset 
  input  wire             en,       // enable
  input  wire [WIDTH-1:0] v,        // value to preload
  output wire [11:0]      bcd       // count output
);

  wire  [WIDTH-1:0]       count;    // interconnect wire

counter myCounter (
  .clk (clk),
  .rst (rst),
  .en (en),
  .count (count)
);

bin2bcd myDecoder (
  .x (count),
  .BCD (bcd)
);

endmodule
```
Modify the testbench file **_top_tb.cpp_** accordingly.  
Modify the **_doit.sh_** file from task 3 to include all the modules (**_top.sv, counter.sv, bin2bcd.sv_** and **_top_tb.sv_**).  Compile and run the Verilated model.  Check that it works according to expectation.
