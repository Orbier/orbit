/*
    Orbit - A simple CMake build system wrapper for C++ projects
    Copyright (C) 2026 Cyphrixz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

*/

/*
    Additional Information: PLEASE SEE LICENSE_NOTICE.md 
    Type `orbit --legal-notice` for more legal information and license details.
*/

#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>
#include <vector>
#include <filesystem>

// heehee

enum class Action { None, Config, Make, Build, RmBuild };
enum class Compiler { Gcc = 1, Clang = 2, ClangCl = 3, Msvc = 4 };

struct Options {
    Action action = Action::None;
    bool useNinja = false;
    std::optional<Compiler> compiler = Compiler::Clang; // std::optional :wilted_rose: (i love clang, so its default)
    bool unity = false; // Shoutout to TheFatRat
};

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <command> [options]\n\n"
              << "Commands:\n"
              << "  config   Configure the cmake project\n"
              << "  make     Build the project\n"
              << "  build    Configure and build the project\n"
              << "  rmbuild  Remove the build directory\n\n"
              << "Options:\n"
              << "  --ninja              Use Ninja generator\n"
              << "  --compiler-id=<id>   Set compiler (1=gcc, 2=clang, 3=clang-cl, 4=msvc)\n"
              << "  --use-compiler=<name> Set compiler by name (gcc, clang, clang-cl, msvc)\n"
              << "                          By default set to `clang` because i love it\n"
              << "  --unity              Build with Unity (single file compilation)\n"
              << "  --help               Show this help message\n"
              << "  --legal-notice       Shows License (Under GNU GPL v3.0) and other legal information\n"
              << "\n\n Please join my discord server! https://discord.gg/kx8km2V7kz";
    // its still niche were `std::cout`ing this way
}

std::optional<Compiler> parseCompilerArg(const std::string& arg) {
    if (arg == "gcc") return Compiler::Gcc; // i love gnu
    if (arg == "clang") return Compiler::Clang; // i love clang
    if (arg == "clang-cl") return Compiler::ClangCl; // i love clang-cl
    if (arg == "msvc") return Compiler::Msvc; // i love gd cologne :)
    if (arg == "1") return Compiler::Gcc;
    if (arg == "2") return Compiler::Clang;
    if (arg == "3") return Compiler::ClangCl;
    if (arg == "4") return Compiler::Msvc; // is gd cologne a furry? 
    return std::nullopt; // i still dont know what this does (nullptr better)

    // this all niche
}

Options parseArgs(int argc, char* argv[]) {
    Options opts;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "config") opts.action = Action::Config;
        else if (arg == "make") opts.action = Action::Make;
        else if (arg == "build") opts.action = Action::Build;
        else if (arg == "rmbuild") opts.action = Action::RmBuild;
        else if (arg == "--ninja") opts.useNinja = true;
        else if (arg == "--unity") opts.unity = true;
        else if (arg == "--help" || arg == "-h") {
            printUsage("ORBIT");
            std::exit(0);
        }
        else if (arg == "--legal-notice") {
            std::cout << "ORBIT Build System\n"
                      << "Licensed under GNU GPL v3.0\n"
                      << "Copyright (c) 2026 Cyphrixz\n" 
                      << "This program comes with ABSOLUTELY NO WARRANTY; see LICENSE for details.\n"
                      << "This is Free Software, and you are welcome to redistribute it under certain conditions"
                      << std::endl; 
            std::exit(0); // what is difference b/w this and return 0 btw? 

        }
        else if (arg.rfind("--compiler-id=", 0) == 0) {
            std::string val = arg.substr(14);
            opts.compiler = parseCompilerArg(val);
        }
        else if (arg.rfind("--use-compiler=", 0) == 0) {
            std::string val = arg.substr(15);
            opts.compiler = parseCompilerArg(val);
        }
    }
    return opts;
    // i had to make this with th help of opencode 
}

std::string getBuildDir() { return "build"; }
std::string getSourceDir() { return "."; }

int runCommand(const std::string& cmd) {
    std::cout << "Running: " << cmd << "\n";
    return std::system(cmd.c_str());
} //  ninja what

int configureProject(const Options& opts) {
    std::string cmd = "cmake -S " + getSourceDir() + " -B " + getBuildDir();

    if (opts.useNinja) {
        cmd += " -G Ninja"; // I LOVE NINJA (im from instagrem, so i meant software)
                            // ninja, not insta ninja
    }

    if (opts.compiler.has_value()) {
        switch (opts.compiler.value()) {
            case Compiler::Gcc:
                cmd += " -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc";
                break; // i love gnu
            case Compiler::Clang:
                cmd += " -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang";
                break; // clang is my gurlfwend
            case Compiler::ClangCl:
                cmd += " -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_C_COMPILER=clang-cl";
                break; // no, not here copilot, sadly
            case Compiler::Msvc:
                cmd += " -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl";
                break; // and here is capitalist propaganda of wierdness
                // and copilot, you cant defend this. Its weird, but useful in 
                // Visual Studio. 
        }
    }

    if (opts.unity) {
        cmd += " -DCMAKE_UNITY_BUILD=ON";
    }

    return runCommand(cmd);
}

int buildProject() {
    std::string cmd = "cmake --build " + getBuildDir();
    return runCommand(cmd); // gawd
}

int removeBuildDir() {
    std::error_code ec;
    if (std::filesystem::exists(getBuildDir(), ec)) {
        std::cout << "Removing build directory...\n";
        std::filesystem::remove_all(getBuildDir(), ec);
        if (ec) {
            std::cerr << "Failed to remove build directory: " << ec.message() << "\n";
            return 1;
        }
        std::cout << "Build directory removed.\n";
    } else {
        std::cout << "Build directory does not exist! \n"; // son.....
    }
    return 0;
}

int main(int argc, char* argv[]) { // char* argv[]
    if (argc < 2) {
        printUsage("ORBIT");
        return 1;
    }

    Options opts = parseArgs(argc, argv);

    if (opts.action == Action::None) {
        std::cerr << "Error: No command specified. Use 'config', 'make', 'build', or 'rmbuild'\n";
        return 1;
    }

    int result = 0;

    if (opts.action == Action::Config || opts.action == Action::Build) {
        result = configureProject(opts);
        if (result != 0) {
            std::cerr << "Configuration failed!\n";
            return result;
        }
    }

    if (opts.action == Action::Make || opts.action == Action::Build) {
        result = buildProject();
        if (result != 0) {
            std::cerr << "Build failed!\n";
            return result;
        }
    }

    if (opts.action == Action::RmBuild) {
        result = removeBuildDir();
        if (result != 0) {
            return result;
        }
    }

    return 0;
}

// clang++ -o ORBIT orbit.cpp -std=c++23