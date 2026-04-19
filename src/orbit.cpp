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
    std::optional<Compiler> compiler = Compiler::Clang;
    bool unity = false;
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
                break;
            case Compiler::Clang:
                cmd += " -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang";
                break;
            case Compiler::ClangCl:
                cmd += " -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_C_COMPILER=clang-cl";
                break;
            case Compiler::Msvc:
                cmd += " -DCMAKE_CXX_COMPILER=cl -DCMAKE_C_COMPILER=cl";
                break;
        }
    }

    if (opts.unity) {
        cmd += " -DCMAKE_UNITY_BUILD=ON";
    }

    return runCommand(cmd);
}

int buildProject() {
    std::string cmd = "cmake --build " + getBuildDir();
    return runCommand(cmd);
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
        std::cout << "Build directory does not exist.\n";
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