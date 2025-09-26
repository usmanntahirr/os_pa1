#include <driver/serial.h>
#include <init/tty.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <driver/keyboard.h>
#include <testmain.h>
#include <init/tests.h>

/* All commands received from the orchestrator are read in this buffer */

#define CMD_BUF_SIZE	128
static  char 	cmd_buf [CMD_BUF_SIZE];

/* List of all the test name to test function mappings. All commands are sent 
	as a single word, and the corresponding test is ran in the kernel. Support
	for params can be added later by using simple strtok calls. */

static struct test_case test_cases[] = {

	{ "vga_entry", 				test_vga_entry },
	{ "vga_cursor", 			test_vga_cursor },
	{ "vga_entry_overwrite", 	test_vga_entry_overwrite },
	{ "vga_color", 				test_vga_entry_colors },
	{ "vga_entry_boundaries", 	test_vga_entry_boundaries },

	{ "intr_reg", 				test_intr_reg },
	{ "intr_unreg", 			test_intr_unreg },
	{ "intr_multi", 			test_intr_multi },

	// { "kbd_basic", 				test_kbd_basic },
	// { "kbd_multi", 				test_kbd_multi },
	// { "kbd_capslock", 			test_kbd_capslock },
	// { "kbd_shift", 				test_kbd_shift },

	{"terminal_getc", 			test_terminal_getc},
	{"terminal_read", 			test_terminal_read},
	{"terminal_putc", 			test_terminal_putc},
	{"terminal_write", 			test_terminal_write},
	{"terminal_cursor", 		test_terminal_cursor},
	{"terminal_clear", 			test_terminal_clear},
	{"terminal_column", 		test_terminal_column},
	

	// {"shell_echo",				test_shell_echo},
	// {"shell_repeat",			test_shell_repeat_n},
	// {"shell_clear",				test_shell_clear},
	// {"shell_colour",			test_shell_text_colour},
	// {"shell_bgcolour",			test_shell_bg_colour},

	// {"syscall_register", 		test_syscall_register},
	// {"syscall_read", 			test_syscall_read},
	// {"syscall_write", 			test_syscall_write},

	// {"shell_echo",				test_shell_echo},
	// {"shell_repeat",			test_shell_repeat_n},
	// {"shell_clear",				test_shell_clear},
	// {"shell_colour",			test_shell_text_colour},
	// {"shell_bgcolour",			test_shell_bg_colour},

	{ NULL, NULL } // marks the end of the array

};

/* Responsible for test orchestration inside the kernel. */

void start_tests () {

	serial_init (false); // initialize the serial port in polling mode

	/* testing cycle is simple, read command, dispatch to function, repeat */
	while (1) {

		size_t cmd_size = read_command ();

		if (cmd_size == 0) {
			continue; // empty command, ignore
		}

		bool found = false;

		for (size_t i = 0; test_cases[i].name != NULL; i++) {

			if (strcmp (cmd_buf, test_cases[i].name) == 0) {

				found = true;
				test_cases[i].func (); // call the test function
				break;

			}

		}

		if (!found) {
			send_msg ("Unknown command");
		}

	}

}

/* Reads a command from the serial port into cmd_buf and returns
   the number of characters read. The command is terminated by a any of these:
   '\0', '\n', '\r'. */

size_t read_command () {

	size_t start = 0;
	memset (cmd_buf, 0, CMD_BUF_SIZE);

	while (1) {
		char c = serial_getc ();

		if (c == '\0' || c == '\n' || c == '\r') {

			cmd_buf [start] = '\0';
			break;
		
		} else {

			if (start < CMD_BUF_SIZE - 1) {
				cmd_buf [start++] = c;
			}
		
		}	
	}

	return start;
}

/* Send a message string back to the server. A "*" is used to mark the end
	of the message. */

void send_msg (const char *msg) {
	serial_puts (msg);
	serial_putc ('*'); // end of message marker
}
