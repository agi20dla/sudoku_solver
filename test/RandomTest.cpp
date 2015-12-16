//
// Created by jason on 12/15/15.
//


#include "gmock/gmock.h"
#include "../src/Random.h"

using namespace ::testing;
using namespace std;

TEST(RandomTest, GetRandomUUID) {
    Random &random = Random::getInstance();
    random.getNewUUID();
}

TEST(RandomTest, GetTwoRandomUUIDsUsingInstance) {
    Random &random = Random::getInstance();
    boost::uuids::uuid uuid1 = random.getNewUUID();
    boost::uuids::uuid uuid2 = random.getNewUUID();
    ASSERT_NE(uuid1, uuid2);
}

TEST(RandomTest, GetTwoRandomUUIDsUsingDirectCall) {
    boost::uuids::uuid uuid1 = Random::getInstance().getNewUUID();
    boost::uuids::uuid uuid2 = Random::getInstance().getNewUUID();
    ASSERT_NE(uuid1, uuid2);
}

TEST(RandomTest, Get1000RandomUUID) {
    boost::uuids::uuid lastUuid;
    for (int idx = 0; idx < 1000; idx++) {
        Random &random = Random::getInstance();
        boost::uuids::uuid uuid = random.getNewUUID();
        ASSERT_NE(uuid, lastUuid);
        lastUuid = uuid;
    }
}
