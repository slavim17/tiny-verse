#pragma once

#include "common/helpers.h"

#define MODULE(KIND, HEADER_FILE) MODULE_(KIND, HEADER_FILE)
#define MODULE_(KIND, HEADER_FILE) STRINGIFY_(KIND ## _ ## HEADER_FILE)

#include MODULE(FRONTEND, frontend)
#include MODULE(OPTIMIZER, optimizer)
#include MODULE(BACKEND, backend)
#include MODULE(TARGET, target)

#define MODULE_CLASS(KIND_VAR, KIND_STR) MODULE_CLASS_(KIND_VAR, KIND_STR)
#define MODULE_CLASS_(KIND_VAR, KIND_STR) KIND_VAR ## _ ## KIND_STR
//#define FRONTEND_CLASS MODULE_CLASS(FRONTEND, frontend)

namespace tiny {

using Frontend = MODULE_CLASS(FRONTEND, frontend);
using Optimizer = MODULE_CLASS(OPTIMIZER, optimizer);
using Backend = MODULE_CLASS(BACKEND, backend);
using Target = MODULE_CLASS(TARGET, target);

void repl() {
    Frontend frontend;
    Optimizer optimizer;
    Backend backend;
    Target target;

    auto ast = frontend.parse("This will be the source code");
    auto il = frontend.compileToIl(std::move(ast));
    optimizer.optimize(il);
    auto obj = backend.to_target(std::move(il));
    target.execute(std::move(obj));


}


}