
#include "Ap.h"
#include <opencv2/core/core.hpp>
#include <mutex>

namespace ORB_SLAM3
{

std::mutex Ap::mApGlobalMutex;



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


/**
 * @brief return number of observations
 * @return nObs
 */
int Ap::Observations()
{
    std::unique_lock<std::mutex> lock(mMutexFeatures);
    return nObs;
}



void Ap::AddObservation(KeyFrame* pKF)
{
    std::unique_lock<std::mutex> lock(mMutexFeatures);
    mObservations.insert(pKF);
    nObs++;
}

void Ap::EraseObservation(KeyFrame* pKF)
{
    std::unique_lock<std::mutex> lock(mMutexFeatures);
    mObservations.erase(pKF);
    nObs--;
}

set<KeyFrame*>  Ap::GetObservations()
{
    unique_lock<mutex> lock(mMutexFeatures);
    return mObservations;
}
} //namespace ORB_SL