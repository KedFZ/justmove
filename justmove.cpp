#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int assign_parameter(fs::path& param, std::string_view arg, std::string_view param_name_string) {
    std::error_code ec;
    param = fs::canonical(arg, ec);
    
    if (ec) {
        std::cerr << "\nERROR: " << param_name_string << " : " << arg << " does not exist or is invalid\n\n" << ec.message() << " (os error " << std::to_string(ec.value()) << ")\n\n";
        return 1;
    }
    if (param_name_string == "[destination_path]" && !fs::is_directory(param)) {
        std::cout << "\nERROR: " << param_name_string << ": " << arg << " is not a directory or may not be a valid path\n\n";
        return 1;
    }
    
    return 0;
};

int move_item(fs::path& source_path, fs::path& destination_path, bool& verbose) {
    std::error_code ec;
    std::string original_source_path = source_path.string();
    fs::path moved_source_path = destination_path/(source_path.filename());
    
    if (fs::exists(moved_source_path)) {
        std::cerr << "ERROR: [moved_source_path]: " << moved_source_path.string() << " already exists. Aborting operation to avoid overwriting...\n";
        return 1;
    }
    if (destination_path.string().find(source_path.string()) == 0) {
        std::cerr << "\nERROR: copying directories into their subdirectories is not allowed";
        return 1;
    }
    
    fs::rename(source_path, moved_source_path, ec);
    
    if (ec) {
        std::cerr << "\nERROR: moving unsuccessful\n\n" << ec.message() << " (os error " << std::to_string(ec.value()) << ")\n\n";
        return 1;
    }
    if (verbose) {
        std::cerr << "\nSUCCESS: moving successful\n" << "[source_path]:       " << original_source_path << "\n[destination_path]:  " << destination_path.string() << "\n[moved_source_path]: " << moved_source_path.string() << "\n\n";
    }
    
    std::cout << moved_source_path.string() << "\n";
    
    return 0;
}

void help_message() {
    std::cerr << "\nUsage:\njustmove [source_path] [destination_path] [options]\n[source_path] | justmove [destination_path] [options]\n\nOptions:\n-h --help     Show help message\n-v --verbose  Show success message after successful move operation\n\n";
}

int main(int argc, char* argv[]) {
    fs::path source_path;
    fs::path destination_path;
    std::error_code ec;
    bool verbose = false;
    const char* paths[2] = { nullptr, nullptr };
    int paths_count = 0;
    
    for (int i = 1; i < argc; i += 1) {
        if (std::string_view(argv[i]) == "-h" || std::string_view(argv[i]) == "--help") {
            help_message();
            return 0;
        } else if (std::string_view(argv[i]) == "-v" || std::string_view(argv[i]) == "--verbose") {
            verbose = true;
        } else {
            if (paths_count < 2) {
                paths[paths_count] = argv[i];
                paths_count += 1;
            }
        }
    }
    
    if (paths_count < 2) {
        if (paths_count < 1) {
            std::cerr << "\nERROR: missing argument for [destination_path]\n";
            help_message();
            return 1;
        }
        
        if (assign_parameter(destination_path, paths[0], "[destination_path]")) {
            return 1;
        }

        std::cerr << "\n[destination_path]: " << destination_path.string() << "\nEnter argument for [source_path]... (Press Ctrl+C to exit)\n";
        std::string input_line;
        
        while(std::getline(std::cin, input_line)) {
            if (input_line.empty() || assign_parameter(source_path, input_line, "[source_path]") || move_item(source_path, destination_path, verbose)) {
                continue;
            }
        }
    } else {
        return assign_parameter(source_path, paths[0], "[source_path]") || assign_parameter(destination_path, paths[1], "[destination_path]") || move_item(source_path, destination_path, verbose);
    }
}