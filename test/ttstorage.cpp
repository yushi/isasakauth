#include <gtest/gtest.h>
#include "../src/ttstorage.h"
#include <string>
using namespace std;

namespace
{
  class TTStorageTest : public ::testing::Test
  {
  protected:
    TTStorageTest()
    {
    }

    virtual ~TTStorageTest()
    {
    }

    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

  };

  TEST_F( TTStorageTest, all )
  {
    TTStorage storage;
    storage.set_host_port("localhost",1978);

    string val;
    string key = string("test_ttstorage");
    string test_str = string("this is test");
    string empty = stirng("");
    bool result = false;
    
    string val = storage.get(key);
    EXPECT_EQ(empty,val);

    result = storage.set(key,test_str);
    EXPECT_EQ(true,result);
  }
}

int main( int argc, char **argv )
{
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
