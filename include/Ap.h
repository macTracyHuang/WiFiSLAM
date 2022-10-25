/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef AP_H
#define AP_H


#include <mutex>
#include<Eigen/Dense>
#include "KeyFrame.h"

// using namespace std;


namespace ORB_SLAM3
{

class Ap
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    Ap():mApPos(Eigen::Vector3f()), mBssid(""),mObservations(std::set<KeyFrame*>()),isInitial(false){}
    Ap(const std::string &Bssid):mApPos(Eigen::Vector3f()),mBssid(Bssid),mObservations(std::set<KeyFrame*>()),isInitial(false){}
    Ap(const Eigen::Vector3f &Pos, const std::string &Bssid):mApPos(Pos),mBssid(Bssid),mObservations(std::set<KeyFrame*>()),isInitial(false){}

    // Copy constructor.
    Ap(const Ap &ap):mApPos(ap.mApPos), mBssid(ap.mBssid), mObservations(ap.mObservations),isInitial(ap.isInitial){}

    // move constructor
    Ap(Ap&& ap) : mApPos(std::move(ap.mApPos)), mBssid(ap.mBssid),mObservations(ap.mObservations),isInitial(ap.isInitial){}

    // copy assignment
    Ap& operator=(const Ap& rhs) {
        if (this != &rhs) { // 檢查自我賦值
            mApPos = rhs.mApPos;
            mBssid = rhs.mBssid;
            mObservations = rhs.mObservations;
            isInitial = rhs.isInitial;
        }
        return *this;
    }

    // move assignment
    Ap& operator=(Ap&& rhs) {
        if (this != &rhs) { // 檢查自我賦值
            mApPos = std::move(rhs.mApPos);
            mBssid = rhs.mBssid;
            mObservations = rhs.mObservations;
            isInitial = rhs.isInitial;
        }

        return *this;
    }


    ~Ap(){}

    

    void SetApPos(const Eigen::Vector3f &Pos);
    Eigen::Vector3f GetApPos();

    void SetBssid(const std::string &Bssid);
    std::string GetBssid();
    static std::mutex mApGlobalMutex;
    int Observations();
    void AddObservation(KeyFrame* pKF);
    void EraseObservation(KeyFrame* pKF);
    std::set<KeyFrame*> GetObservations();
    bool isInitial;
    void InitializePose();
    float average(std::vector<float> const& v);


    int nObs;
    long unsigned int mnId;
protected:
    // Position in absolute coordinates
    Eigen::Vector3f mApPos;
    std::string mBssid;
    std::set<KeyFrame*> mObservations;
    
    // Mutex
    std::mutex mMutexApPos;
    std::mutex mMutexFeatures;
    std::mutex mMutexObservations;
    
    


// private:
};
}
#endif // AP_H
