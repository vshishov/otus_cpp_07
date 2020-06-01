#include "reader.h"
#include "executer.h"
#include "logger.h"

#include <string>
#include <memory>

int main(int argc, const char** argv) 
{  
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <N>" << std::endl;
    return 1;
  }

  std::size_t szBlockSize{0};
  try {
    szBlockSize = std::stoi(argv[1]);
  }
  catch (const std::invalid_argument& ex) {
    std::cerr << "Error: Block size is incorrect!" << std::endl;
    return 1;
  }
  catch (const std::out_of_range& ex) {
    std::cerr << "Error: Block size is out of range!" << std::endl;
    return 1;
  }

  if (szBlockSize == 0) {
    std::cerr << "Error: Block size must be greater then zero!" << std::endl;
    return 1;
  }

  auto pReader = std::make_shared<Otus::Reader>(szBlockSize);
  auto pExecuter = Otus::Excuter::Create(pReader);
  auto pLogger = Otus::Logger::Create(pReader);
  pReader->Exec();
  return 0;
}
