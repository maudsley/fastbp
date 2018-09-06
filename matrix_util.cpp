#include "matrix_util.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

binary_matrix load_binary_matrix(const std::string& file_name) {
  std::ifstream file_stream(file_name);
  if (!file_stream.is_open()) {
    return binary_matrix();
  }

  std::vector<std::string> file_lines;
  std::string line;
  while (std::getline(file_stream, line)) {
    if (!line.empty()) {
      file_lines.push_back(line);
    }
  }

  if (file_lines.empty()) {
    return binary_matrix();
  }

  /* calc column count */
  size_t col_count = 0;
  std::stringstream first_row;
  first_row << file_lines[0];
  while (!first_row.eof()) {
    int bit = 0;
    first_row >> bit;
    col_count++;
  }

  binary_matrix mat(file_lines.size(), col_count);
  for (size_t i = 0; i < file_lines.size(); ++i) {
    std::stringstream row;
    row << file_lines[i];
    size_t col_index = 0;
    while (!row.eof()) {
      if (col_index > mat.cols()) {
        throw std::runtime_error("Matrix is malformed!");
      }
      int bit = 0;
      row >> bit;
      mat[i][col_index++] = bit;
    }
    if (col_index != col_count) {
      throw std::runtime_error("Matrix is malformed!");
    }
  }

  return mat;
}
