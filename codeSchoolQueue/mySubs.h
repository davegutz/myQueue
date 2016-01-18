#define FLT_FRONT_LOC 		1
#define FLT_REAR_LOC			2
#define FLT_MAX_SIZE_LOC 	3

void newCode(const unsigned long tim, const unsigned long cod);
void newImpend(const unsigned long tim, const unsigned long cod);
Queue* loadCodeNVM(const int start, const bool reset);
Queue* loadImpendNVM(const int start, const bool reset);
int storeCodeNVM(const int start);
int storeImpendNVM(const int start);
