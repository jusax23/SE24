#include <fstream>
#include <iostream>
#include <vector>

#include "load_stb.hh"
#include "logger.hh"

// ------------------------------------------------------------------------ //
struct Color {
  int r = 0;
  int g = 0;
  int b = 0;
};

// ------------------------------------------------------------------------ //
bool openFile(std::ifstream &file) {
  Logger::print("open file: ", false);
  file.open("data.dat");
  if (!file.is_open()) {
    Logger::print("failed");
    return false;
  }
  Logger::print("successful");
  return true;
}

// ------------------------------------------------------------------------ //
bool checkFileType(std::ifstream &file) {
  std::string line{};
  std::getline(file, line);
  Logger::print("check filetype: ", false);
  if (line.compare("indeximage") != 0) {
    Logger::print("wrong");
    return false;
  }
  Logger::print("correct");
  return true;
}

// ------------------------------------------------------------------------ //
bool loadSize(std::ifstream &file, int &width, int &height) {
  Logger::print("reading image size: ", false);
  std::string line{};
  try {
    std::getline(file, line);
    width = std::atoi(line.c_str());
    std::getline(file, line);
    height = std::atoi(line.c_str());
  } catch (const std::exception &e) {
    Logger::print("failed");
    return false;
  }
  Logger::print("successful");
  return true;
}

// ------------------------------------------------------------------------ //
bool loadColorPalette(std::ifstream &file, std::vector<Color> &palette) {
  Logger::print("reading color palette: ", false);
  std::string line{};
  try {
    std::getline(file, line);
    int count = std::atoi(line.c_str());
    for (int i = 0; i < count; ++i) {
      std::getline(file, line);
      int p0 = 0;
      int p1 = line.find(" ");
      int r = std::atoi(line.substr(p0, p1).c_str());
      p0 = p1;
      p1 = line.find(" ", p0 + 1);
      int g = std::atoi(line.substr(p0, p1).c_str());
      p0 = p1;
      p1 = line.find(" ", p0 + 1);
      int b = std::atoi(line.substr(p0, p1).c_str());
      palette.push_back(Color{r, g, b});
    }
  } catch (const std::exception &e) {
    Logger::print("failed");
    return false;
  }
  Logger::print("successful");
  return true;
}

// ------------------------------------------------------------------------ //
void createImage(std::ifstream &file, int width, int height,
                 std::vector<Color> const &palette) {
  Logger::print("write data to image... ", false);
  char *data = new char[width * height * 3];
  int count = 0;
  for (int y = 0; y < height; ++y) {
    std::string line{};
    std::getline(file, line);
    int p0 = 0;
    int p1 = line.find(" ");
    for (int x = 0; x < width; ++x) {
      int idx = std::atoi(line.substr(p0, p1).c_str());
      p0 = p1;
      p1 = line.find(" ", p0 + 1);
      Color col = palette[idx];
      data[count++] = col.r;
      data[count++] = col.g;
      data[count++] = col.b;
    }
  }
  stbi_write_png("image.png", width, height, 3, data, 0);
  Logger::print("successful");
  delete[] data;
}

// ------------------------------------------------------------------------ //
bool generateImage() {
  std::ifstream file{};
  // ---
  if (!openFile(file)) {
    file.close();
    return false;
  }
  if (!checkFileType(file)) {
    file.close();
    return false;
  }
  // ---
  int width = -1;
  int height = -1;
  if (!loadSize(file, width, height)) {
    file.close();
    return false;
  }
  // ---
  std::vector<Color> palette{};
  if (!loadColorPalette(file, palette)) {
    file.close();
    return false;
  }
  // ---
  createImage(file, width, height, palette);
  // ---
  file.close();
  return true;
}

// ------------------------------------------------------------------------ //
int main(int argc, char const *argv[]) {
  Logger::printStartInfo();
  generateImage();

  return 0;
}
