#include "co2/IScope.h"

#define O_SUPER Interface()

O_OBJECT(IScope, Interface);
O_END_OBJECT

struct IScope *current_scope = NULL;
struct IScope *global_scope = NULL;
