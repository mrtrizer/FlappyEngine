#pragma once

#define REFLECT __attribute((annotate("reflect")))
#define PROP __attribute((annotate("property")))
#define PROP_SET(name) __attribute((annotate("property_set:" ## name)))
#define PROP_GET(name) __attribute((annotate("property_get:" ## name)))
#define FUNC __attribute((annotate("function")))
