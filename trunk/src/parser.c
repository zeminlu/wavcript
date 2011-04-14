/**
 * \file parser.c
 *
 *  \date Created on: 11-apr-2011
 *  \author Pablo Giorgi, Santiago Pérez de Rosso & Luciano R. Zemin
 */
 
#include <string.h>
#include "../inc/parser.h"

*t_input parseInput(int argc, char *argv[]) {
	t_input *input = malloc(sizeof(t_input));
	
	int i = 0;
	for(i = 0; i < argc; i++) {
		char *currentOpt = argv[i];
		if(strcmp(currentOpt, "-in") == 0) {
			if(i + 1 < argc) {
				if(input->input == null) {
					input->input = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-out") == 0) {
			if(i + 1 < argc) {
				if(input->output == null) {
					input->output = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-pass") == 0) {
			if(i + 1 < argc) {
				if(input->pass == null) {
					input->pass = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-K") == 0) {
			if(i + 1 < argc) {
				if(input->key == null) {
					input->key = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-iv") == 0) {
			if(i + 1 < argc) {
				if(input->iv == null) {
					input->iv = argv[i+1];
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-a") == 0) {
			if(i + 1 < argc) {
				if(input->algorithm == 0) {
					input->algorithm = parseAlgorithm(argv[i+1]);
				} else {
					// TODO: Manage error, duplicated opcode.
				}
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-m") == 0) {
			if(i + 1 < argc) {
				if(input->mode == 0) {
					input->mode = parseMode(argv[i+1]);
				} else {
					// TODO: Manage error, duplicated opcode.
				}		
			} else {
				// TODO: Manage error, incorrect input.
			}
		} else if(strcmp(currentOpt, "-d") == 0) {
			if(input->operation == 0) {
				input->operation = DEC;
			} else{
				// TODO: Manage error, duplicated opcode.
			}
		} else if(strcmp(currentOpt, "-e") == 0) {
			if(input->mode) {
				input->operation = ENC;
			} else {
				// TODO: Manage error, duplicated opcode.
			}
		} else {
			// TODO: Manage error, inexistant option.
		}
	}
	
	if(wrongInput(input)) {
		input = null;
		// TODO: Manage error, missing parameters.
	}
	
	return input;
}

enum t_algorithm parseAlgorithm(char *algorithm) {
	char *algorithms[] = {"aes128", "aes192", "aes256", "des"};
	int elems = sizeof(algorithms) / sizeof(char*);
	
	int i = 0;
	for(i = 0; i < elems; i++) {
		if(strcmp(algorithm, algorithms[i]) == 0) {
			return i;
		}
	}
	
	return 0;
}

enum t_mode parseMode(char *mode) {
	char *modes[] = {"ecb", "cfb", "ofb", "cbc"};
	int elems = sizeof(modes) / sizeof(char*);
	
	int i = 0;
	for(i = 0; i < elems; i++) {
		if(strcmp(mode, modes[i]) == 0) {
			return i;
		}
	}
	
	return 0;
}

boolean wrongInput(t_input* input) {
	if(input->input == null) {
		return TRUE;
	} else if(input->output != null) {
		return TRUE;
	} else if(input->operation != 0) {
		return TRUE;
	} else if(input->operation != 0) {
		return TRUE;
	} else if(input->pass != null) {
		return TRUE;
	} else if(input->key != null) {
		return TRUE;
	} else if(input->iv != null) {
		return TRUE;
	} else if(input->algorithm != 0) {
		return TRUE;
	} else if(input->validMode != 0) {
		return TRUE;
	}
	return FALSE;
}
