
#include "Ap.h"
#include <opencv2/core/core.hpp>
#include <mutex>

namespace ORB_SLAM3
{

std::mutex Ap::mApGlobalMutex;

/** 
 * @brief 构造函数
 */


void Ap::SetApPos(const Eigen::Vector3f &Pos)
{
    std::unique_lock<std::mutex> lock2(mApGlobalMutex);
    std::unique_lock<std::mutex> lock(mMutexApPos);
    mApPos = Pos;
}

Eigen::Vector3f Ap::GetApPos()
{
    std::unique_lock<std::mutex> lock(mMutexApPos);
    return mApPos;
}

void Ap::SetBssid(const std::string &Bssid)
{
    std::unique_lock<std::mutex> lock2(mApGlobalMutex);
    mBssid = Bssid;
}

std::string Ap::GetBssid()
{
    return mBssid;
}

int Ap::GetNumOfObserved()
{
    std::unique_lock<std::mutex> lock(mMutexFeatures);
    return mNumOfObserved;
}

void Ap::AddNumOfObserved()
{
    std::unique_lock<std::mutex> lock(mMutexFeatures);
    mNumOfObserved++;
}

void Ap::ResetNumOfObserved()
{
    std::unique_lock<std::mutex> lock(mMutexFeatures);
    mNumOfObserved = 0;
}
} //namespace ORB_SLAM