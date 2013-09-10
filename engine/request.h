#ifndef __REQUEST
#define __REQUEST

#define CMD_READ 	0;
#define CMD_WRITE	0xff;

struct request
{
	int subject_id;
	int object_id;
	int cmd;
};

extern "C"
{
	
}

#endif