suck my ass

check first steps

volgorde:
1: instruction decoder
        given an instruction word, determine the control signals that are
        necessary to control the components in the data path 
        (register, ALU, and so on). You will have to isolate different fields
        from the instructions. To do so, add fields to struct DecodedInstruction
        in inst-decoder.h in which you can store the results of a decoded instruction.
        illegal instruction (fi a non-existent or non-implemented opcode)
        throw the exception IllegalInstruction

2: mini disassembler
         A start can be found in inst-formatter.cc. To enable the printing of
         instructions to the terminal the command-line option -d must be supplied.
         
3: ALU
        execute instructions
        
4: Processor
        interconnect outputs from ALU