#include "reader.h"
#include "executer.h"

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

using namespace testing;

TEST(bulk_simple, empty_stream) { 
  std::stringstream ssTestIn;
  std::stringstream ssTestOut;

  auto pReader = std::make_shared<Otus::Reader>(2, ssTestIn);
  auto pExecuter = Otus::Excuter::Create(pReader, ssTestOut);

  pReader->Exec();

  ASSERT_EQ(ssTestOut.str(), "");
}

TEST(bulk_simple, not_empty_stream) { 
  std::stringstream ssTestIn;
  std::stringstream ssTestOut;

  auto pReader = std::make_shared<Otus::Reader>(3, ssTestIn);
  auto pExecuter = Otus::Excuter::Create(pReader, ssTestOut);

  ssTestIn << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "cmd3" << std::endl
  ;

  pReader->Exec();

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1, cmd2, cmd3\n");
}

TEST(bulk_simple, not_complete) { 
  std::stringstream ssTestIn;
  std::stringstream ssTestOut;

  {
    auto pReader = std::make_shared<Otus::Reader>(3, ssTestIn);
    auto pExecuter = Otus::Excuter::Create(pReader, ssTestOut);

    ssTestIn << "cmd1" << std::endl
      << "cmd2" << std::endl
    ;

    pReader->Exec();
  }

  ASSERT_EQ(ssTestOut.str(), "");
}

TEST(bulk_blocks, single_block) { 
  std::stringstream ssTestIn;
  std::stringstream ssTestOut;

  auto pReader = std::make_shared<Otus::Reader>(3, ssTestIn);
  auto pExecuter = Otus::Excuter::Create(pReader, ssTestOut);

  ssTestIn << "{" << std::endl
    << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "cmd3" << std::endl
    << "cmd4" << std::endl
    << "cmd5" << std::endl
    << "}" << std::endl
  ;

  pReader->Exec();

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1, cmd2, cmd3, cmd4, cmd5\n");
}

TEST(bulk_blocks, single_block_in_default_block) { 
  std::stringstream ssTestIn;
  std::stringstream ssTestOut;

  auto pReader = std::make_shared<Otus::Reader>(3, ssTestIn);
  auto pExecuter = Otus::Excuter::Create(pReader, ssTestOut);

  ssTestIn << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "{" << std::endl
    << "cmd3" << std::endl
    << "cmd4" << std::endl
    << "cmd5" << std::endl
    << "cmd6" << std::endl
    << "cmd7" << std::endl
    << "}" << std::endl
  ;

  pReader->Exec();

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1, cmd2\nbulk: cmd3, cmd4, cmd5, cmd6, cmd7\n");
}

TEST(bulk_blocks, multi_block) { 
  std::stringstream ssTestIn;
  std::stringstream ssTestOut;

  auto pReader = std::make_shared<Otus::Reader>(3, ssTestIn);
  auto pExecuter = Otus::Excuter::Create(pReader, ssTestOut);

  ssTestIn << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "{" << std::endl
    << "cmd1.1" << std::endl
    << "cmd1.2" << std::endl
    << "cmd1.3" << std::endl
    << "{" << std::endl
    << "cmd2.1" << std::endl
    << "cmd2.2" << std::endl
    << "cmd2.3" << std::endl
    << "cmd2.4" << std::endl
    << "cmd2.5" << std::endl
    << "}" << std::endl
    << "cmd1.4" << std::endl
    << "cmd1.5" << std::endl
    << "}" << std::endl
  ;

  pReader->Exec();

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1, cmd2\n"
    "bulk: cmd1.1, cmd1.2, cmd1.3, "
      "cmd2.1, cmd2.2, cmd2.3, cmd2.4, cmd2.5, "
    "cmd1.4, cmd1.5\n");
}

TEST(bulk_blocks, not_complete) { 
  std::stringstream ssTestIn;
  std::stringstream ssTestOut;
  {
    auto pReader = std::make_shared<Otus::Reader>(3, ssTestIn);
    auto pExecuter = Otus::Excuter::Create(pReader, ssTestOut);

    ssTestIn << "cmd1" << std::endl
      << "cmd2" << std::endl
      << "{" << std::endl
      << "cmd1.1" << std::endl
      << "cmd1.2" << std::endl
      << "cmd1.3" << std::endl
      << "{" << std::endl
      << "cmd2.1" << std::endl
      << "cmd2.2" << std::endl
      << "cmd2.3" << std::endl
      << "cmd2.4" << std::endl
      << "cmd2.5" << std::endl
      << "}" << std::endl
      << "cmd1.4" << std::endl
      << "cmd1.5" << std::endl
    ;

    pReader->Exec();
  }

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1, cmd2\n");
}

TEST(bulk_blocks, not_correct_sequence) { 
  std::stringstream ssTestIn;
  std::stringstream ssTestOut;

  auto pReader = std::make_shared<Otus::Reader>(3, ssTestIn);
  auto pExecuter = Otus::Excuter::Create(pReader, ssTestOut);

  ssTestIn << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "}" << std::endl
  ;

  ASSERT_THROW(pReader->Exec(), std::logic_error);
}
