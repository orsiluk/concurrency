#include "talk.h"

void talk1() {
	int i = 0;
	int massage;
	while (1) {
		if (i == 0) {
			massage = 18;
			printS("Tell the other your age! ");
			writeC(0, massage);
			i++;
		} else {
			yield();
		}
	}
}

void talk2() {
	int i = 0;
	int massage;
	while (1) {
		if (i == 0) {

			printS("Read her age! ");
			massage = readC(0);
			printInt(massage);
			printS("\n");
			i++;
		} else {
			yield();

		}

	}
}
