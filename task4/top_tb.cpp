#include "Vtop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int i;
    int clk;


    Verilated::commandArgs(argc, argv);
    //init top verilog instance
    Vtop* top = new Vtop;
    //init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("top.vcd");

    //init Vbuddy
    if (vbdOpen()!=1) return(-1);
    vbdHeader("Lab 1: Counter");
    vbdSetMode(1);

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    //run simulation for many clock cycles
    for (i=0; i<300; i++){

        // dump variables into VCD file and toggle clock
        for (clk=0; clk<2; clk++){
            tfp->dump (2*i+clk); // unit is in ps!!!
            top->clk = !top->clk;
            top->eval ();
        }

    // *** Send count value to Vbuddy
    vbdHex(4, (int(top->count) >> 16) & 0xF);
    vbdHex(3, (int(top->count) >> 8) & 0xF);
    vbdHex(2, (int(top->count) >> 4) & 0xF);
    vbdHex(1, int(top->count) & 0xF);
    vbdCycle(i+1);
    // -- end of Vbuddy output section

    //change of input
    if (vbdFlag()){
        top->count = vbdValue();
    }


        top->rst = (i <2) | (i == 15);
        top->en = (i > 4);
    if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}