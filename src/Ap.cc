
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

} //namespace ORB_SLAM