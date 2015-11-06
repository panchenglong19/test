#include"zbus.h"
struct fun {
	int msgid;
	void* send_buf;
	void** meta;
};
struct fun fun_msg[10];