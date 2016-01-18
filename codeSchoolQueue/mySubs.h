#define FLT_FRONT_LOC 		1
#define FLT_REAR_LOC			2
#define FLT_MAX_SIZE_LOC 	3

void newCode(const unsigned long tim, const unsigned long cod);
void newImpend(const unsigned long tim, const unsigned long cod);
Queue* loadNVM(const int start, const bool reset, Queue* II);
int storeNVM(const int start);
