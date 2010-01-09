#include <gtest/gtest.h>
#include "../src/ttstorage.h"
#include <string>

namespace
{
  using namespace std;
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
    string empty = string("");
    bool result = false;
    
    val = storage.get(key);
    EXPECT_EQ(empty,val);

    result = storage.set(key,test_str);
    EXPECT_EQ(true,result);

    val = storage.get(key);
    EXPECT_EQ(test_str,val);

    result = storage.del(key);
    EXPECT_EQ(true,result);

    val = storage.get(key);
    EXPECT_EQ(empty,val);
  }
}

int main( int argc, char **argv )
{
  ::testing::InitGoogleTest( &argc, argv );
  return RUN_ALL_TESTS();
}
