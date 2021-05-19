#pragma once

extern enum keys key;
extern struct itimerval nval;
extern int accumulator;
extern int inst_counter;
extern int com;
extern int op;
extern int cursor;
extern int temp;

void memoryShow();
void CPUshow();
void flagShow();
void keysShow();
void printBoxes();
void printBigChars();
void console();
void settimer(struct itimerval *nval);
void timer();
void reset();

int alu(int command, int operand);
int cu();