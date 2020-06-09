#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <ncurses.h> 

void display_percent(int id, char *meaning, int value)
{
	mvprintw(2+id,0,"%s: %.2f %%\n",meaning,(float)value/100);
}

void display_mer(int id, char *meaning, int value)
{
	mvprintw(2+id,0,"%s: %.1f dB\n",meaning,(float)value/10);
}

void display_yes_no(int id, char *meaning, int value)
{
	mvprintw(2+id,0,"%s: %s\n",meaning,value==0?"no":"yes");
}

void display_state(int id, char *meaning, int value)
{
	static char *state[] = {
		"initialising",
		"searching",
		"found headers",
		"DVB-S lock",
		"DVB-S2 lock"
	};

	if (value <  sizeof(state)/sizeof(*state)) {
		mvprintw(2+id,0,"%s: %s\n",meaning,state[value]);
	} else {
		mvprintw(2+id,0,"%s: %d\n",meaning,value);
	}
}

void display_modcod_s(int id, char *meaning, int value)
{
	static char *modcod[] = {
		"QPSK 1/2",
		"QPSK 2/3",
		"QPSK 3/4",
		"QPSK 5/6",
		"QPSK 6/7",
		"QPSK 7/8"
	};

	if (value <  sizeof(modcod)/sizeof(*modcod)) {
		mvprintw(2+id,0,"%s: %s\n",meaning,modcod[value]);
	} else {
		mvprintw(2+id,0,"%s: %d\n",meaning,value);
	}
}

void display_modcod_s2(int id, char *meaning, int value)
{
	static char *modcod[] = {
		"DummyPL",
		"QPSK 1/4",
		"QPSK 1/3",
		"QPSK 2/5",
		"QPSK 1/2",
		"QPSK 3/5",
		"QPSK 2/3",
		"QPSK 3/4",
		"QPSK 4/5",
		"QPSK 5/6",
		"QPSK 8/9",
		"QPSK 9/10",
		"8PSK 3/5",
		"8PSK 2/3",
		"8PSK 3/4",
		"8PSK 5/6",
		"8PSK 8/9",
		"8PSK 9/10",
		"16APSK 2/3",
		"16APSK 3/4",
		"16APSK 4/5",
		"16APSK 5/6",
		"16APSK 8/9",
		"16APSK 9/10",
		"32APSK 3/4",
		"32APSK 4/5",
		"32APSK 5/6",
		"32APSK 8/9",
		"32APSK 9/10"
	};

	if (value <  sizeof(modcod)/sizeof(*modcod)) {
		mvprintw(2+id,0,"%s: %s\n",meaning,modcod[value]);
	} else {
		mvprintw(2+id,0,"%s: %d\n",meaning,value);
	}
}

void display_status(int id, char *value)
{
	static int state = 0;

	static char *meaning[] = {
		"",
		"State",
		"LNA Gain",
		"Puncture Rate",
		"I Symbol Power",
		"Q Symbol Power",
		"Carrier Frequency",
		"I Constellation",
		"Q Constellation",
		"Symbol Rate",
		"Viterbi Error Rate",
		"BER",
		"MER",
		"Service Provider",
		"Service",
		"Null Ratio",
		"ES PID",
		"ES Type",
		"MODCOD",
		"Short Frames",
		"Pilot Symbols",
		"LDPC Error Count",
		"BCH Error Count",
		"BCH Uncorrected",
		"LNB Voltage Enabled",
		"LNB H Polarisation"
	};

	if (id < sizeof(meaning)/sizeof(*meaning)) {
		switch(id) {
			case 19:
			case 20:
			case 23:
			case 24:
			case 25:
				display_yes_no(id, meaning[id], atoi(value));
				break;

			case 1:
				state = atoi(value);
				display_state(id, meaning[id], state);
				break;

			case 10:
			case 11:
				display_percent(id, meaning[id], atoi(value));
				break;

			case 12:
				display_mer(id, meaning[id], atoi(value));
				break;

			case 18:
				switch(state) {
					case 3:
						display_modcod_s(id, meaning[id], atoi(value));
						break;

					case 4:
						display_modcod_s2(id, meaning[id], atoi(value));
						break;
				}
				break;

			default:
				mvprintw(2+id,0,"%s: %s\n",meaning[id],value);
		}
	} else {
		mvprintw(2+id,0,"%d: %s\n",id,value);
	}
}

int main() { 
	int n;
	char status_message[32], *n_str, *m_str;
	FILE *status_fifo; 
	WINDOW *scroll_win;

	/* Open status FIFO for read only  */
	status_fifo = fopen("longmynd_main_status", "r"); 
	if (status_fifo == NULL) {
		printf("Failed to open status fifo\n");
		return 1;
	}

	initscr();

	mvprintw(1,0,"LongMynd DATV Receiver Status\n");

	scroll_win = newwin(LINES - 70, COLS, 70, 0);
	scrollok(scroll_win, TRUE);

	wprintw(scroll_win,"Listening\n");
	wrefresh(scroll_win);

	while (1) {
		if (fgets(status_message, sizeof(status_message), status_fifo) != NULL) {
			if (status_message[0] == '$') {
				n_str = strtok(status_message + 1, ",");
				if (n_str != NULL) {
					n = atoi(n_str);
					m_str = strtok(NULL, "\n");
					if (m_str != NULL) {
						display_status(n, m_str);
						refresh();
					}
				}
				else
				{
					wprintw(scroll_win,"E:%s\n",status_message);
					wrefresh(scroll_win);
				}
			}
		}
	}
	fclose(status_fifo); 
	return 0; 
}
