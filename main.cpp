#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>

int main(int argc, char* argv[]) {
    // Check that an input file was specified.
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " input_file\n";
        return 1;
    }

    // Open the input file.
    std::ifstream input_file(argv[1], std::ios::binary);
    if (!input_file.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return 1;
    }

    // Convert the contents of the input file into a vector.
    std::vector<char> bytes((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());

    // Convert the bytes to hexadecimal strings
    std::stringstream hex_stream;
    hex_stream << std::hex << std::setfill('0');
    for (const char& byte : bytes) {
        hex_stream << std::setw(2) << static_cast<int>(static_cast<unsigned char>(byte));
    }
    std::string hex_string = hex_stream.str();

    // Print the original hexadecimal string
        std::cout << " " << std::endl;
        std::cout << "- - - - - ORIGINAL - - - - -" << std::endl;
        std::cout << " " << std::endl;
        std::cout << hex_string << std::endl;


    // Search for the consecutive symbols "49 4e 46 31" (INF1) and "56 54 58 31" (VTX1) in the hexadecimal string.
    // THIS IS CASE SENSITIVE DUE TO HOW WE HANDLE THE STRING!!!!

    std::string INF1 = "494e4631";
    std::string VTX1 = "56545831";
    std::size_t pos_1 = hex_string.find(INF1);
    std::size_t pos_2 = hex_string.find(VTX1);
    bool found_INF1 = pos_1 != std::string::npos;
    bool found_VTX1 = pos_2 != std::string::npos;

    // Replace the portion of the hex string between the found symbols with our own new string
    if (found_INF1 && found_VTX1) {
        std::string new_string = "000000800002ffff00000002000000a6000000180010000000010000001100000001000000120000000100000011000100010000001200010001000000100001001000020010000300020000000200000002000000020000000200000000000054686973206973207020505241454349505541205741532048455245";
        std::size_t replace_len = pos_2 - (pos_1 + INF1.length());
        hex_string.replace(pos_1 + INF1.length(), replace_len, new_string);

        // Print the modified hexadecimal string
        std::cout << " " << std::endl;
        std::cout << "- - - - - MODIFIED - - - - -" << std::endl;
        std::cout << " " << std::endl;
        std::cout << hex_string << std::endl;
    }

    // Print "found" or "not found" depending on whether INF1 or VTX1 were found
    std::cout << (found_INF1 ? "INF1 found" : "INF1 not found") << std::endl;
    std::cout << (found_VTX1 ? "VTX1 found" : "VTX1 not found") << std::endl;


    // Convert the modified hexadecimal string back to bytes
    std::vector<char> modified_bytes;
    for (std::size_t i = 0; i < hex_string.length(); i += 2) {
        std::string byte_string = hex_string.substr(i, 2);
        char byte = static_cast<char>(std::strtol(byte_string.c_str(), nullptr, 16));
        modified_bytes.push_back(byte);
    }

    // Get the suffix of the input file
    std::string input_filename(argv[1]);
    std::string suffix = "";
    std::size_t dot_pos = input_filename.rfind(".");
    if (dot_pos != std::string::npos) {
        suffix = input_filename.substr(dot_pos);
    }

    // Create the output file
    std::string output_filename = input_filename.substr(0, dot_pos) + "_fixed" + suffix;
    std::ofstream output_file(output_filename, std::ios::binary);
    if (!output_file.is_open()) {
        std::cerr << "Failed to create file: " << output_filename << "\n";
        return 1;
    }

    // Write the modified bytes to the output file
    output_file.write(modified_bytes.data(), modified_bytes.size());

    std::cout << "Output file created: " << output_filename << std::endl;
    
    // Close the input file
    input_file.close();

    // Wait for user input before exiting
    std::cout << "Press enter to exit..." << std::endl;
    getchar();

    return 0;
}