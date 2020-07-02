# Spider
Spider is an esoteric programming language, inspired by Befunge and Fish.
It operates on two dimensions. You can move up, down, left and right using `^v<>` respectively. Every instruction along the way will be executed.

In order to do arithmetic, Spider uses a stack consisting of only integers. You can push values onto it, and some instructions "pop" a value off of the stack, do something with them, and make an action.

To push the number 5 we just simply need to write the number 5 in our code, and 5 will be on the stack. (Note: the program starts in the top left and moves to the right) We can print it using `o`, which will simply pop a value off of the stack (removing it from it), and printing the integer to the terminal. The stack will be empty after this operation, if you wanna keep the value after printing it, you can duplicate the top value on the stack using the `d` instruction.

What about the number 14? If you try to simply write 14, it will push 1 and 4 onto the stack, not the combined number. To do that, we have two options.

1. Push value 9 and 5, then call the `+` instruction, which pops **two** values off the stack, adds them up, and pushes the result onto the stack. This will result in 14 on the stack, because 9 + 5 = 14.
2. Use something called the "number mode". There are three "modes" in Spider. First, the default one, simply the Instruction mode. Each character is treated like a Spider instruction. Next, the number mode, which is turned on using the `(` instruction, and turned off using `)`. In this mode, each numerical instruction (0-9) will be pushed onto a temporary string inside the interpreter (all other characters are ignored, except of course the `)` instruction). Once the number mode gets toggled off, the temporary string will be converted into a number, and pushed onto the stack. This makes putting large numbers on the stack much simpler. The code `(14)o@` will print 14. (Yes, the `@` instruction ends the code execution). The third mode I'll discuss a bit later.

Of course you could also for example multiply 2 and 5, using `*`, the multiplication instruction, but you get the idea.
## Outputting text
The third mode in Spider is the "string mode". It's simply toggled using `"`.

Until the next `"`, string mode will push the ASCII value of the current instruction.

Keep in mind, "hello" will push the ASCII values in reverse order. There is an instruction in Spider that will reverse the stack's order to make the next step a bit easier, `r`.

The `O` instruction (the big letter not zero) will simply go over the whole stack, and print every value as if it was an ASCII character.

This means that this program prints Hello World!:

	"Hello World!"rO@
    
It uses the reverse stack instruction as well as the OUTPUT WHOLE STACK instruction.
## The Register
There is a special type of place where you can hold a single number, the register. You can set the value of it using the tilde instruction, which will pop a value and set the register to that. Then, you can retrieve the value again using `~`, pushing it back onto the stack.

This might be useful in a number of situations. Example: you wanna clear the stack but keep one specific value.
## Available instructions
    0 - 9:   	push the number to the stack
    < v > ^: 	change the direction the IP is heading in
    o:			pop and output as an integer
    p:	   		pop and output as an ASCII character
    @:			stop program execution
    +:			pop two values and push the sum
    *:			pop two values and push the product
    -:			pop two values and push the difference
    =:			pop two values, if they're equal, go up, else, go down
    (:			start number mode
    ):			stop number mode
    ":			toggle string mode
    O:			output whole stack as an ASCII string
    l:			push the length of the stack onto the stack
    g:			get user input, push the first character as an ASCII value
    G:			get user input, push ALL characters as ASCII values
    i:			get user input, push it as a number
    d:			duplicate the top value on the stack
    D:			pop a value and discard it
    r:			reverse the stack order
    s:			print the stack's values in the format of [1, 5, 2] etc, mainly used for debugging
    #:			move in a random direction (up down left or right)
    _:			skip the next instruction (really handy sometimes)
    N:			print new line (saves a couple of characters in code)
    C:			discard the whole stack (clear)
    %:			pop a value, then use it as a percentage chance to go up, otherwise go down
    `:			pop a value and push it onto the register
    ~:			push the value from the register onto the stack, and reset register to 0
    
## Example Program
The program below will simply print "Hello World!" half of the time. The other half of the time it'll print "Bye World!".

    v   >"Hello World!"v
	>  >#<    @      Or<
	    >"Bye World!"  ^
The `#` symbol can go in any direction, which is why in this code, if it goes left or right, it'll go back into the `#`, allowing it only to exit up or down. If it goes up, it'll print Hello World!, if not, it'll print Bye World!.
## Notes
* If the instruction pointer reaches an edge, it'll "wrap around" to the other side.

# Interpreter
## Building
Simply build using `g++ main.cpp -o spider`. Optionally use some optimization flags.
## Running Spider code
To run some Spider code, make a new file where you'll write your code.
When you're done, simply run `./spider <file name here>`.