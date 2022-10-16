#include "../include/Fingerprint.h"

#include<mutex>

namespace ORB_SLAM3
{

std::mutex Fingerprint::mFingerprintGlobalMutex;
}