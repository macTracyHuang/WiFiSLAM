
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
    std::unique_lock<std::mutex> lock(mMutexApPos);
    std::unique_lock<std::mutex> lock2(mMutexObservations);

    vector<KeyFrame*> obs = vector<KeyFrame*>(mObservations.begin(), mObservations.end());
    assert(nObs == obs.size());
    // vector<double> r(nObs, 0);
    // vector<Eigen::Vector3f> nomalizedCam(nObs);
    vector<float> vx;
    vector<float> vy;
    vector<float> vz;

    for (int i = 0 ; i < nObs; i++)
    {
        Sophus::SE3f Twc = obs[i]->GetPoseInverse();
        Eigen::Vector3f t = Twc.translation();

        vx.push_back(t[0]);
        vy.push_back(t[1]);
        vz.push_back(t[2]);
        // double apx = 0, apy = 0, apz = 0;

        // RSSI Prediction Based on P(r) = P(r0) - 10 * alpha * log(r/r0)
        // P(r) = C - 10 * alpha * log(r)
        // C = P(r0) - log(r0) , r = 10^((C-P(r) / (10 * alpha))
        // Assume r0 = 1 P(1) = -20
        
        // To Do: dis calculated by pos or rssi?

        // int C = -20, alpha = 5;
        // auto fp = obs[i]->mpFingerprint;
        // int Pr = 0;
        // for (int i =0; i< int(fp->mvAp.size()); i++)
        // {
        //     if (fp->mvAp[i]->mnId == mnId)
        //     {
        //         Pr = fp->mvRssi[i];
        //         break;
        //     }
        // }
        // double r_i = pow(10, ((C-Pr)/ (10 * alpha)));


        // double sqare = pow((t[0] - apx),2) + pow((t[1] - apy),2) + pow((t[2] - apz),2);
        // double r_i = sqrt(sqare);

        // r[i] = r_i;
        // cout << "before norm: "<<endl << t << endl;
        // nomalizedCam[i] = t/sqare;
        // cout << "cam i " << nomalizedCam[i] <<endl;
    }

    // double R =  std::inner_product(r.begin(),r.end(),r.begin(),0);
    double x = 0, y = 0,z = 0;
    
    // for (auto &cam:nomalizedCam)
    // {
    //     x += cam[0];
    //     y += cam[1];
    //     z += cam[2];
    // }
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