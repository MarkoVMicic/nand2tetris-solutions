void parse_c_instruction(char * current_line);

int * reverse_array_order(int * array);

void parse_a_instruction(char * current_line);

void parse_current_line(char * current_line);

const char * parse_line_and_update_to_next_line(const char *current_line);

void parse_asm_string(const char *asm_string, const char * output_file_path);
