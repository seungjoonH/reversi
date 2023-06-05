#include <stdio.h>

#include "lib/enums.h"
#include "lib/system.h"
#include "lib/ui.h"
#include "lib/network.h"

int main(int argc, char **argv) {
	iptlen = argc;
	iptvec = argv;

	if (argc == 1) terminate(WRN_PM);

	const Role role = getRole();
	const char *ip = getIp();
	int port = getPort();

	showUI();

	return 0;
}
