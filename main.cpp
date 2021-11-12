#include <cstdlib>
#include <iostream>

#include "common/helpers.h"
#include "common/tests.h"
#include "common/colors.h"
#include "common/config.h"
#include "common/parser.h"


#define MODULE(KIND, HEADER_FILE) MODULE_(KIND, HEADER_FILE)
#define MODULE_(KIND, HEADER_FILE) STRINGIFY_(KIND ## _ ## HEADER_FILE)

#include MODULE(FRONTEND, frontend)
#include MODULE(OPTIMIZER, optimizer)
#include MODULE(BACKEND, backend)
#include MODULE(TARGET, target)

#define MODULE_CLASS(KIND_VAR, KIND_STR) MODULE_CLASS_(KIND_VAR, KIND_STR)
#define MODULE_CLASS_(KIND_VAR, KIND_STR) KIND_VAR ## _ ## KIND_STR

using namespace tiny;

using Frontend = MODULE_CLASS(FRONTEND, frontend);
using Optimizer = MODULE_CLASS(OPTIMIZER, optimizer);
using Backend = MODULE_CLASS(BACKEND, backend);
using Target = MODULE_CLASS(TARGET, target);

/** A simple pretty printer for an error within its location
 */
void printErrorLocation(std::istream & str, SourceLocation loc) {
    size_t l = 0;
    std::string line;
    std::cout << color::lightRed << "> " << color::gray;
    while (++l <= loc.line())
        std::getline(str, line, '\n');
    std::cout << line << std::endl;
    std::cout << color::lightRed << "| ";
    for (size_t i = 1; i < loc.col(); ++i)
        std::cout << " ";
    std::cout << "^" << std::endl;
}

void printErrorLocationFromFile(std::string const & filename, SourceLocation loc) {
    std::ifstream f;
    f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f.open(filename);
    printErrorLocation(f, loc);
}

void interactive() {
//#if (defined FRONTEND_DUMMY) || (defined FRONTEND_TINYC)
    Frontend frontend;
    Optimizer optimizer;
    Backend backend;
    Target target;
    std::cout << color::reset << "IM IN YR PROMPTZ, WUT DO U WANTS?" << std::endl;
    while (true) {
        try {
            std::cout << color::lightBlue << "> " << color::white;
            std::string input;
            while (true) {
                std::string line;
                std::getline(std::cin, line);
                if (! input.empty())
                    input += '\n';
                input += line;
                try {
                    auto ast = frontend.parseFile(config.input());
                    auto il = frontend.compileToIl(std::move(ast));
                    optimizer.optimize(il);
                    auto obj = backend.compileToTarget(std::move(il));
                    target.execute(std::move(obj));
                    break; // start new prompt
                } catch (ParserError const & e) {
                    if (! e.eof())
                        throw;
                    std::cout << color::blue << "| " << color::white;
                }
            }
        } catch (ParserError const & e) {
            std::cout << color::lightRed << "! " << e.what() << std::endl;
            std::cout << color::gray << "  at " << e.location() << std::endl;
        } catch (std::exception const & e) {
            std::cout << color::lightRed << "! " << e.what() << std::endl;
        }
    }
//#else
//    throw std::runtime_error("Interactive mode is not supported for given frontend, backend and target");
//#endif
}

/** Compile must be provided for all targets, it's really compile & run. 
 
    TODO the name is *bad*.
 */
void compile() {
    Frontend frontend;
    Optimizer optimizer;
    Backend backend;
    Target target;
    try {
        auto ast = frontend.parseFile(config.input());
        auto il = frontend.compileToIl(std::move(ast));
        optimizer.optimize(il);
        auto obj = backend.compileToTarget(std::move(il));
        target.execute(std::move(obj));
    } catch (ParserError const & e) {
        std::cout << color::lightRed << "! " << e.what() << std::endl;
        std::cout << color::gray << "  at " << e.location() << std::endl;
        printErrorLocationFromFile(config.input(), e.location());
    } catch (std::exception const & e) {
        std::cout << color::lightRed << "! " << e.what() << std::endl;
    } catch (char const * e) {
        std::cout << color::lightRed << "!str: " << e << std::endl;
    }
}

int main(int argc, char * argv []) {
    initializeTerminal();
    std::cout << "OH HAI, CAN I HAZ TINYVERZ?" << std::endl;
    std::cout << color::gray << "    frontend:  " << color::reset << STRINGIFY(FRONTEND) << std::endl;
    std::cout << color::gray << "    optimizer: " << color::reset << STRINGIFY(OPTIMIZER) << std::endl;
    std::cout << color::gray << "    backend:   " << color::reset << STRINGIFY(BACKEND) << std::endl;
    std::cout << color::gray << "    target:    " << color::reset << STRINGIFY(TARGET) << std::endl;    
    try {
        // parse command line arguments
        config.parse(argc, argv);
        /* If we are in debug mode, execute all tests first to make sure that the implementation works a bit.
         */
#if (! defined NDEBUG)
        std::cout << color::reset << "IM IN YR TESTS, RUNNIN:" << color::gray << std::endl;
        if (Tests::RunAll(argc, argv) == EXIT_SUCCESS)
            std::cout << "DUN: " << color::green << "AWSUM" << std::endl;
        else
            std::cout << "DUN: " << color::red << "FAILZ" << std::endl;
#endif
        /* Determine if we are to run repl, or simply compile a single file
         */
        if (config.interactive())
            interactive();
        else
            compile();
        std::cout << color::green << "KTHXBYE!" << color::reset << std::endl;
        return EXIT_SUCCESS;
    } catch (std::exception const & e) {
        std::cout << color::red << "O NOEZ: " << e.what() << color::reset << std::endl;
    } catch (...) {
        std::cout << color::red << "O NOEZ: unknown exception raised." << color::reset << std::endl;
    }
    return EXIT_FAILURE;
}