#include <stdio.h>

#include "externs.h"
#include "command.h"

/* Global var is non-zero when end of file has been reached. */
int EOF_Reached = 0;

/* Non-zero is the recursion depth for commands. */
int indirection_level = 0;

/* Non-zero means that at this moment, the shell is interactive.  In
   general, this means that the shell is at this moment reading input
   from the keyboard. */
int interactive = 0;

/* Non-zero means end of file is after one command. */
int just_one_command = 0;

int reader_loop()
{
	int our_indirection_level;
	COMMAND *volatile current_command;

	current_command = (COMMAND *)NULL;

	while (EOF_Reached == 0)
	{
		if (read_command() == 0)
		{
				// dispose_command(global_command);
				global_command = (COMMAND *)NULL;
				// execute_command(current_command);
		}
		else
		{
			/* Parse error, maybe discard rest of stream if not interactive. */
			if (interactive == 0)
				EOF_Reached = EOF;
		}
		if (just_one_command)
			EOF_Reached = EOF;
	}
}

/* Read and parse a command, returning the status of the parse.  The command
   is left in the globval variable GLOBAL_COMMAND for use by reader_loop.
   This is where the shell timeout code is executed. */
int read_command()
{
//   SHELL_VAR *tmout_var;
//   int tmout_len, result;
//   SigHandler *old_alrm;

//   set_current_prompt_level(1);
//   global_command = (COMMAND *)NULL;

//   /* Only do timeouts if interactive. */
//   tmout_var = (SHELL_VAR *)NULL;
//   tmout_len = 0;
//   old_alrm = (SigHandler *)NULL;

//   if (interactive)
//   {
//     tmout_var = find_variable("TMOUT");

//     if (tmout_var && var_isset(tmout_var))
//     {
//       tmout_len = atoi(value_cell(tmout_var));
//       if (tmout_len > 0)
//       {
//         old_alrm = set_signal_handler(SIGALRM, alrm_catcher);
//         alarm(tmout_len);
//       }
//     }
//   }

//   QUIT;

//   current_command_line_count = 0;
//   result = parse_command();

//   if (interactive && tmout_var && (tmout_len > 0))
//   {
//     alarm(0);
//     set_signal_handler(SIGALRM, old_alrm);
//   }

//   return (result);
}
