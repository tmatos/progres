
// valid file

module finish;
    initial begin
        $display("Message 1");
        #10;
        $display("Message 2");
        #10;
        $display("Message 3");
        #10;
        $display("Message 4");
        #10;
        $display("Message 5");
        #10;
        $finish; // finish will stop the simulation immediately,
                 // so the following messages will not be displayed
        $display("Message 6");
        #10;
        $display("Message 7");
        #10;
        $display("Message 8");
        #10;
        $display("Message 9");
        #10;
        $display("Message 10");
    end
endmodule
