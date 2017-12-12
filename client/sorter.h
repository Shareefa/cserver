#ifndef w
#define w
extern char isFloat[28];
#endif
struct movie{
	char* values[28];
	struct movie* next;
};

/*
struct movieNode {
	struct movie movie;
	struct movieNode* next;
};
*/
void mergesort(struct movie*, int, int);

