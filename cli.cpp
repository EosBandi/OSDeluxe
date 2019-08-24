/*
    OSDeluxe - Color PIP Mavlink OSD
    Copyright (C) 2018  Andras Schaffer - Dronedoktor.eu

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    With Grateful Acknowledgements to the projects:
        AlceOSD by Luis Alves
        PlayUAV OSD
        Brain FPV Flight Controller(https://github.com/BrainFPV/TauLabs) by Tau Labs

 */

#include "cli.h"
#include "osdeluxe.h"

char cli_buffer[80];      // CLI buffer
char cli_history[12][80]; // CLI history

uint8_t cli_position; // Current position in the cli buffer
uint8_t cli_history_position;

// Init cli variables
void init_cli()
{

    memset(cli_buffer, 0, sizeof(cli_buffer));
    memset(cli_history, 0, sizeof(cli_history));
    cli_position = 0;
    cli_history_position = 0;
}

void read_cli()
{

    // Serial is the usb port
    while (Serial.available() > 0)
    {
        uint8_t c = Serial.read(); // Read character
        // check for special characters
        // up pervious from history
        // down next in history if we not at the end;

        // del
        // right
        // left

        // enter

		if (c == 'z') { Serial.print("\r\n");

             for (int i = 0; i < 12; i++)
            {
                Serial.printf("History line %d : *%s*\r\n", i, cli_history[i]);
            }
			 }

		else
        if (c == 13)
        {
            Serial.printf("\r\n");
            if (cli_position > 0)
            {
                for (int i = 10; i >= 0; i--)
                {
                    memcpy(cli_history[i + 1], cli_history[i], sizeof(cli_buffer));
                }
                memcpy(cli_history[0], cli_buffer, sizeof(cli_buffer));
                process_cli(cli_buffer);

				memset(cli_buffer, 0, sizeof(cli_buffer));
                cli_position = 0;
                cli_history_position = 0;
                
            }
            Serial.print("OSDeluxe-CLI>");
        }
        else if (c == 27) // ESC character
        {
			if (Serial.peek() == '[')
			{
                Serial.read();
				if (Serial.available() == 1)
				{
                    c = Serial.read();
                    if (c == 'A')
					{
							if ((cli_history_position < 11) ) {
                                for (int i = 0; i < cli_position; i++) Serial.print("\b \b");
								memcpy(cli_buffer, cli_history[cli_history_position], sizeof(cli_buffer));
								Serial.print(cli_buffer);
								cli_position = strlen(cli_buffer);
                                if (cli_history[cli_history_position+1][0] != 0) cli_history_position++;

							}
					}

                    if (c == 'B')
                    {
                            for (int i = 0; i < cli_position; i++) Serial.print("\b \b");
                            memcpy(cli_buffer, cli_history[cli_history_position], sizeof(cli_buffer));
                            Serial.print(cli_buffer);
                            cli_position = strlen(cli_buffer);
                            if (cli_history_position > 0) cli_history_position--;
							
                    }
                        
					


				}
			}
			//Drop all chars still in the buffer
            Serial.clear();
        }
		else if (c == 127)	//Backspace
		{
			if (cli_position > 0)
			{
                cli_position--;
                cli_buffer[cli_position] = 0;
                Serial.print("\b \b");
			}
		}
            else
            {
                cli_buffer[cli_position++] = c;
                Serial.printf("%c", c);
            }
    }
}

void process_cli(char *cmdline)
{
    // get the first part till the first space or end of string, strsep modify cmdline pointer to the next non separator position
    char *command = strsep(&cmdline, " ");

    if (strcmp(command, "help") == 0)
    {
        cli_do_help();
        return;
    }

    if (strcmp(command, "set") == 0)
    {
        char *subcommand = strsep(&cmdline, " "); // Get the second token (set what)

        if (strcmp(subcommand, "param") == 0)
        {
            cli_do_set_param(cmdline);
            return;
        }
        Serial.printf("Invalid command: set %s\r\n", subcommand);
        return;
    } // End of "set commands"

    if (strcmp(command, "show") == 0)
    {
        char *subcommand = strsep(&cmdline, " "); // Get the second token (show what)

        if (strcmp(subcommand, "param") == 0)
        {
            cli_do_list_params(cmdline);
            return;
        }
        Serial.printf("Invalid command: show %s\r\n", subcommand);
        return;
    }

    Serial.printf("Command not recognised *%s*\r\n", command);
}

bool match(const char *pattern, const char *candidate, int p, int c)
{
    if (pattern[p] == '\0')
    {
        return candidate[c] == '\0';
    }
    else if (pattern[p] == '*')
    {
        for (; candidate[c] != '\0'; c++)
        {
            if (match(pattern, candidate, p + 1, c)) return true;
        }
        return match(pattern, candidate, p + 1, c);
    }
    else if (pattern[p] != '?' && pattern[p] != candidate[c])
    {
        return false;
    }
    else
    {
        return match(pattern, candidate, p + 1, c + 1);
    }
}

void cli_do_list_params(char* name)
{

	if (strlen(name) == 0) return;

	if (total_params == 0) get_parameter_count();
	for (int idx = 0; idx < total_params; idx++)
	{
        if (match(name,parameters[idx].name,0,0)) Serial.printf("%s\t\t\t%f\r\n",parameters[idx].name, cast2float(parameters[idx].value, parameters[idx].type));
	}

}


void cli_do_set_param(char *cmdline)
{ 
	char *name;
    float value;

	name = strsep(&cmdline, " ");
    value = atof(strsep(&cmdline, " "));

	if (strlen(name) == 0) return;

	params_set_value(name, value, 1);

	Serial.printf("param %s set to %f\r\n", name, value);

}


void cli_do_help()
{

	Serial.println(match("batv1*", "batv1_min", 0, 0));
    Serial.println(match("batv1*", "batv2_min", 0, 0));


  /*  Serial.println("\r\nset program <program-id> <hour:min> <route-id>    - Set program timer and route\r\n"
                    "enable <program-id>                               - Enable program\r\n"
                    "set route <route-id> <route-flight-time>          - Set route flight time\r\n"
                    "set trigger <trigger-input> <route>               - Set trigger\r\n"
                    "show triggers                                     - Display all triggers\r\n"
                    "show programs                                     - Display all programs\r\n"
                    "show routes                                       - Display all routes\r\n"
                    "time                                              - Show date and time\r\n"
                    "set time <hours:minutes>                          - Set time\r\n"
                    "set date <year-month-day>                         - Set date\r\n"
                    "echo 1/0                                          - Enable terminal echo\r\n");
					*/
}