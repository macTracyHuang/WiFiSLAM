
#include "Ap.h"
#include <opencv2/core/core.hpp>
#include <mutex>

namespace ORB_SLAM3
{

long unsigned int Ap::nNextId=0;
std::mutex Ap::mApGlobalMutex;

Ap::Ap():mApPos(Eigen::Vector3f()), mBssid(""),mObservations(std::set<KeyFrame*>()),isInitial(false)
{
    nObs = 0;
    mnId=nNextId++;
}
Ap::Ap(const std::string &Bssid):mApPos(Eigen::Vector3f()),mBssid(Bssid),mObservations(std::set<KeyFrame*>()),isInitial(false)
{
    nObs = 0;
    mnId=nNextId++;
}
Ap::Ap(const Eigen::Vector3f &Pos, const std::string &Bssid):mApPos(Pos),mBssid(Bssid),mObservations(std::set<KeyFrame*>()),isInitial(false)
{
    nObs = 0;
    mnId=nNextId++;
}

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
    std::unique_lock<std::mutex> lock2(mMutexObservations);
    return nObs;
}



void Ap::AddObservation(KeyFrame* pKF)
{
    std::unique_lock<std::mutex> lock(mMutexFeatures);
    std::unique_lock<std::mutex> lock2(mMutexObservations);
    mObservations.insert(pKF);
    nObs++;
}

void Ap::EraseObservation(KeyFrame* pKF)
{
    std::unique_lock<std::mutex> lock(mMutexFeatures);
    std::unique_lock<std::mutex> lock2(mMutexObservations);
    mObservations.erase(pKF);
    nObs--;
}

set<KeyFrame*>  Ap::GetObservations()
{
    unique_lock<mutex> lock(mMutexFeatures);
    std::unique_lock<std::mutex> lock2(mMutexObservations);
    return mObservations;
}

float Ap::average(std::vector<float> const& v){
    if(v.empty()){
        return 0;
    }

    auto const count = static_cast<float>(v.size());
    return std::accumulate(v.begin(), v.end(), 0) / count;
}

void Ap::InitializePose()
{

    if (isInitial)
    {
        cout << "AP already Initial!" <<endl;
        return;
    }
    // else
    // {
    //     cout << "Ap::InitializePose" <<endl;
    // }
    std::unique_lock<std::mutex> lock(mMutexApPos);
    std::unique_lock<std::mutex> lock2(mMutexObservations);

    vector<KeyFrame*> obs = vector<KeyFrame*>(mObservations.begin(), mObservations.end());

    int num_obs = obs.size();

    // vector<double> r(nObs, 0);
    // vector<Eigen::Vector3f> nomalizedCam(nObs);
    vector<float> vx;
    vector<float> vy;
    vector<float> vz;

    for (int i = 0 ; i < num_obs; i++)
    {
        Sophus::SE3f Twc = obs[i]->GetPoseInverse();
        Eigen::Vector3f t = Twc.translation();
        vx.push_back(t[0]);
        vy.push_back(t[1]);
        vz.push_back(t[2]);
    }


    double x = 0, y = 0,z = 0;
    
 
    x = average(vx);
    y = average(vy);
    z = average(vz);

    x = isnan(x)? 0:x;
    y = isnan(y)? 0:y;
    z = isnan(z)? 0:z;

    Eigen::Vector3f initPose(x,y,z);
    mApPos = initPose;
    // cout << "Initialized " <<  mBssid << " as: " << endl << initPose << endl;
    isInitial = true;
}

} //namespace ORB_SL