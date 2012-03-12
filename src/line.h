typedef struct __pre_line_info {
	char * file;
	int line;
} LINE;

void line_initial(void);
void line_finish(void);
LINE * line_revert(int line);
int line_register_file(char * filename);
void line_register_line(int fid, int line_file);
