/* TEAM MEMBER */
// Hyeon Seungjoon (21800788)
// Song San (22000375)

#include <stdio.h>

#include "lib/system.h"
#include "lib/ui.h"
#include "lib/network.h"

int main(int argc, char **argv) {
	iptlen = argc;
	iptvec = argv;

	if (argc == 1) terminate(WRN_PM);
	rungame();

	return 0;
}
