#include <fstream>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <syncstream>
#include <thread>
#include <unistd.h>
#include <vector>

size_t get_file_size(const std::string &input_file, const std::string &output_file) {
  FILE *in_file = fopen(input_file.c_str(), "rb");
  fseek(in_file, 0L, SEEK_END);
  long file_size = ftell(in_file);
  if (file_size == -1) {
    throw new std::ios::failure("Failed to get size of file.");
  }
  fclose(in_file);
  return file_size;
}

void reverse_lines(std::ifstream &m_file, std::pair<size_t, size_t> bounds, std::ofstream &o_file) {
  m_file.seekg(bounds.first, std::ios::beg);
  o_file.seekp(bounds.first, std::ios::beg);

  char endline = '\n'; // On Windows the line will include CR, as line endings are CRLF (\r\n).
  // todo: Ideally, we  would check  all types of  newlines and  mimic the  result in the  output file.

  std::string line;
  bool is_first = true;
  while (m_file.tellg() < bounds.second) {
    if (!std::getline(m_file, line, endline)) {
      break;
    }

    if (is_first && bounds.first != 0) {
      o_file.seekp(bounds.first + line.size());
    } else {
      if (line[line.size() - 1] == '\r') {
        std::reverse(line.begin(), line.end() - 1);  // Reverse the string excluding the CR at the end.
        o_file.write(line.c_str(), line.size() - 1); // Write the line minus the CR.
        o_file.write("\n", 1);                       // Add the CR and LF at the end.
      } else {
        std::reverse(line.begin(), line.end());
        o_file.write(line.c_str(), line.size());
      }
    }

    is_first = false;
  }
}

int reverse_file_lines(std::string input_file, std::string output_file) {
  std::vector<std::thread> processor_threads;
  size_t num_threads = 7;

  std::vector<std::ifstream> input_streams;
  std::vector<std::ofstream> output_streams;

  std::vector<std::thread> procs;
  size_t file_size = get_file_size(input_file, output_file);
  size_t chunk_size = file_size / num_threads;

  for (size_t i = 0; i < num_threads; ++i) {
    input_streams.emplace_back();
    input_streams.back().open(input_file, std::ios::binary);
    output_streams.emplace_back();
    output_streams.back().open(output_file, std::ios::out | std::ios::app);
  }

  for (size_t i = 0; i < num_threads; ++i) {
    auto bounds = std::make_pair(i * chunk_size, (i + 1) * chunk_size);
    procs.emplace_back([&input_streams, bounds, &output_streams, i]() { reverse_lines(input_streams[i], bounds, output_streams[i]); });
  }

  for (auto &proc : procs) {
    proc.join();
  }

  for (auto &in_stream : input_streams) {
    in_stream.close();
  }
  for (auto &out_stream : output_streams) {
    out_stream.close();
  }

  return EXIT_SUCCESS;
}

int do_main() {
  reverse_file_lines("./input.txt", "./output_1.txt");
  reverse_file_lines("./output_1.txt", "./output_2.txt");
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) { return do_main(); }