#pragma once

extern enum keys key;
extern struct itimerval nval;
extern int accumulator;
extern int counter;
extern int com;
extern int op;
extern int cursor;
extern int temp;

void print_memory();
void print_flags();
void print_keywords();
void print_frames();
void print_current_inst();
void print_gui();

void settimer(struct itimerval *nval);
void timer(int sig);
void reset(int sig);

int ALU(int command, int operand);
int CU();