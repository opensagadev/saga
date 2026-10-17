#pragma once

#include "decomp.h"

// Declared here for the other units that use them.
struct NuIOS_InAppProduct;
i32 NuIOS_GetInAppProductByID(char *, NuIOS_InAppProduct *);
i32 NuIOS_GetPurchaseResult();
i32 NuIOS_IsProductPurchased(char *);
int NuIOS_PurchaseInAppProduct(char *);
void NuIOS_RestoreInAppPurchases();
