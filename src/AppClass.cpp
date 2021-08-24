#include "AppClass.h"

/** Special */
AppClass::AppClass()
    : attr_(0) {}

/** Accessors */
const int &AppClass::attr() const { return attr_; }

void AppClass::setAttr(const int &newAttr) { attr_ = newAttr; }
