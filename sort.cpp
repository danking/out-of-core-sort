#include <iostream>
#include <fstream>

int sort_file(char * input_filename,
              char * output_filename,
              int block_size);
int sort_stream(std::ifstream& input,
                std::ofstream& output,
                int block_size);

int main(int argc, char ** argv) {
  if (argc < 3 || argc > 4) {
    std::cout << "USAGE: sort INPUT OUTPUT [BLOCK_SIZE]" << std::endl
              << std::endl
              << "    BLOCK_SIZE defaults to 1kB" << std::endl;
    return 1;
  }
  char * input = argv[1];
  char * output = argv[2];
  int block_size = 1024;
  if (argc > 3) {
    block_size = atoi(argv[3]);
  }
  if (block_size % 4 != 0) {
    std::cout << "block_size must be a multiple of four" << std::endl;
    return 4;
  }
  return sort_file(input, output, block_size);
}


int sort_file(char * input_filename,
              char * output_filename,
              int block_size) {
  std::ifstream input(input_filename, std::ios::binary);
  std::ofstream output(output_filename, std::ios::binary);
  if (!input.is_open()) {
    printf("could not open %s", input_filename);
    return 2;
  }
  if (!output.is_open()) {
    printf("could not open %s", output_filename);
    return 3;
  }
  return sort_stream(input, output, block_size);
}

int sort_stream(std::ifstream& input,
                std::ofstream& output,
                int block_size) {
  char buf[block_size];
  while (!input.eof()) {
    input.read(buf, block_size);
    int n_bytes = input.gcount();
    if (n_bytes % 4 != 0) {
      std::cout << "bad file, size is not multiple of four, had "
                << n_bytes << " left over." << std::endl;
      return 5;
    }
    int n_ints = n_bytes / 4;
    unsigned int * ints = (unsigned int *) buf;
    std::sort(ints, ints + n_ints);
    output.write(buf, 4 * n_ints);
  }
  return 0;
}
